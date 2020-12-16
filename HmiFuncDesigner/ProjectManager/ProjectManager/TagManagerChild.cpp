#include "TagManagerChild.h"
#include "NewComDeviceDialog.h"
#include "stringdata.h"
#include "writer.h"
#include "reader.h"
#include "variantdata.h"
#include "Helper.h"
#include "qsoftcore.h"
#include "DevicePluginLoader.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QModelIndex>
#include <QCloseEvent>
#include <QStringList>
#include <QHeaderView>
#include <QTableView>
#include <QModelIndex>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include "qsoftcore.h"
#include "qprojectcore.h"
#include "IDevicePlugin.h"
#include "TableviewDelegate.h"
#include "MainWindow.h"
#include <QVariant>
#include <QApplication>
#include <QHeaderView>
#include <QStackedWidget>
#include <QTableWidget>
#include <QDir>
#include <QFile>
#include <QPluginLoader>
#include <QFileDialog>
#include "tag.h"
#include "tageditdialog.h"
#include <QClipboard>
#include <QInputDialog>
#include <QScrollBar>
#include <QTime>
#include <QMessageBox>
#include <QMouseEvent>
#include <QJsonObject>
#include <QDebug>


//------------------------------------------------------------------------------

const int MaxTagIOTableColumns = TagTableModel::MaxColumns;


Qt::ItemFlags TagTableModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable|/*Qt::ItemIsEditable|*/Qt::ItemIsEnabled;
    }
    return theFlags;
}


QVariant TagTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() ||
            index.row() < 0 || index.row() >= m_tagRows.count() ||
            index.column() < 0 || index.column() >= MaxTagIOTableColumns)
        return QVariant();
    const QStringList &rowDat = m_tagRows.at(index.row());
    if (role == Qt::SizeHintRole) {
        QStyleOptionComboBox option;
        option.currentText = rowDat.at(index.column());
        QFontMetrics fontMetrics(data(index, Qt::FontRole).value<QFont>());
        option.fontMetrics = fontMetrics;
        QSize size(fontMetrics.width(option.currentText), fontMetrics.height());
        return qApp->style()->sizeFromContents(QStyle::CT_ComboBox, &option, size);
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return rowDat.at(index.column());
    }
    return QVariant();
}


QVariant TagTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case TagID: return tr("变量ID");
        case Name: return tr("变量名称");
        case Addr: return tr("变量地址");
        case DataType: return tr("数据类型");
        case ReadWrite: return tr("读写");
        case Unit: return tr("单位");
        case Remark: return tr("变量描述");
        default: Q_ASSERT(false);
        }
    }
    return section + 1;
}


int TagTableModel::rowCount(const QModelIndex &index) const {
    return index.isValid() ? 0 : m_tagRows.count();
}


int TagTableModel::columnCount(const QModelIndex &index) const {
    return index.isValid() ? 0 : MaxTagIOTableColumns;
}


bool TagTableModel::setData(const QModelIndex &index,
                            const QVariant &value,
                            int role) {
    if (!index.isValid() || role != Qt::EditRole ||
            index.row() < 0 || index.row() >= m_tagRows.count() ||
            index.column() < 0 || index.column() >= MaxTagIOTableColumns)
        return false;

    QStringList &rowDat = m_tagRows[index.row()];
    rowDat[index.column()] = value.toString();
    emit dataChanged(index, index);
    return true;
}


bool TagTableModel::insertRows(int row, int count, const QModelIndex&) {
    if(count < 1) return false;
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagRows.insert(row, QStringList());
    endInsertRows();
    return true;
}


bool TagTableModel::removeRows(int row, int count, const QModelIndex&) {
    if(count < 1) return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_tagRows.removeAt(row);
    endRemoveRows();
    return true;
}


void TagTableModel::AppendRow(QStringList rowDat) {
    int iRow = m_tagRows.size();
    beginInsertRows(QModelIndex(), iRow, iRow);
    QStringList it = rowDat;
    m_tagRows.append(it);
    endInsertRows();
}

void TagTableModel::AppendRows(QVector<QStringList> rowsDat) {
    if(rowsDat.size() < 1) return;
    int iRow = m_tagRows.size();
    beginInsertRows(QModelIndex(), iRow, iRow + rowsDat.size() - 1);
    for(int i=0; i<rowsDat.size(); i++) {
        QStringList row = rowsDat.at(i);
        m_tagRows.append(row);
    }
    endInsertRows();
}


void TagTableModel::InsertRow(int i, QStringList rowDat) {
    beginInsertRows(QModelIndex(), i, i);
    QStringList it = rowDat;
    m_tagRows.insert(i, it);
    endInsertRows();
}


QStringList TagTableModel::GetRow(int i) {
    QStringList it;
    if(i < m_tagRows.size())
        return m_tagRows.at(i);
    return it;
}


void TagTableModel::UpdateRow(int i, QStringList rowDat) {
    if(i<m_tagRows.size()) {
        m_tagRows.replace(i, rowDat);
        for(int c=0; c<MaxTagIOTableColumns; c++)
            emit dataChanged(this->index(i, c), this->index(i, c));
    }
}

void TagTableModel::UpdateRows(QVector<QStringList> rowsDat) {
    m_tagRows = rowsDat;
    emit dataChanged(this->index(0, 0), this->index(m_tagRows.size() - 1, MaxTagIOTableColumns - 1));
}


////////////////////////////////////////////////////////////////////



QTableWidgetEx::QTableWidgetEx(QWidget *parent) : QTableView(parent) {
    setContextMenuPolicy(Qt::DefaultContextMenu);
    m_pTagTableModel = new TagTableModel();
    // 初始化变量表控件
    initTagsTable();
    connect(this, &QAbstractItemView::doubleClicked, this, &QTableWidgetEx::onDoubleClicked);
}

QTableWidgetEx::~QTableWidgetEx() {
    if(m_pTagTableModel != Q_NULLPTR) {
        delete m_pTagTableModel;
        m_pTagTableModel = Q_NULLPTR;
    }
}

///
/// \brief QTableWidgetEx::initTagsTable
/// \details 初始化变量表控件
///
void QTableWidgetEx::initTagsTable() {
    this->setModel(m_pTagTableModel);
    this->horizontalHeader()->setSectionsClickable(false);
    this->horizontalHeader()->setStretchLastSection(true);
    //this->horizontalHeader()->setHighlightSections(true);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    //this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->horizontalHeader()->show();
    this->verticalHeader()->hide();
    this->setColumnWidth(0, 60); // 变量ID
    this->setColumnWidth(1, 100); // 变量名称
    this->setColumnWidth(2, 100); // 变量地址
    this->setColumnWidth(3, 100); // 数据类型
    this->setColumnWidth(4, 100); // 读写
    this->setColumnWidth(5, 60); // 单位
    this->setColumnWidth(6, 80); // 变量描述
    this->setWordWrap(false);
    this->clearSelection();

    // 根据程序实例字体大小调整列宽
    QFontMetrics fm(qApp->font());
    for(int col=0; col<TagTableModel::MaxColumns; col++) {
        QVariant headerData = m_pTagTableModel->headerData(col, Qt::Horizontal);
        int iWidth = fm.width(headerData.toString()) + 2*fm.averageCharWidth();
        if(iWidth > this->columnWidth(col)) this->setColumnWidth(col, iWidth);
    }
}

///
/// \brief QTableWidgetEx::clearTable
/// \details 清空变量表
///
void QTableWidgetEx::clearTable() {
    m_pTagTableModel->removeRows(0, m_pTagTableModel->rowCount());
}

///
/// \brief QTableWidgetEx::updateTable
/// \details 刷新变量表
///
void QTableWidgetEx::updateTable() {
    clearTable();
    QVector<QStringList> rowsDat;
    foreach (Tag *pTagObj, QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags) {
        QStringList rowDat;
        setRowData(rowDat, pTagObj);
        rowsDat.append(rowDat);
    }
    m_pTagTableModel->AppendRows(rowsDat);
}


///
/// \brief QTableWidgetEx::setRowData
/// \details 设置指定行数据
/// \param rowDat 行数据
/// \param pObj 变量描述对象
///
void QTableWidgetEx::setRowData(QStringList &rowDat, Tag *pObj) {
    rowDat.clear();
    rowDat << QString::number(pObj->m_iID); // 变量ID

    if(pObj->m_szDevType == "SYSTEM") rowDat << QString("$%1").arg(pObj->m_szName); // 变量名称
    else  rowDat << pObj->m_szName;

    if(pObj->m_szDevType == "MEMORY") { // 内存变量
        rowDat << tr("自动分配"); // 地址类型
    } else if(pObj->m_szDevType == "SYSTEM") { // 系统变量
        rowDat << tr("自动分配"); // 地址类型
    } else {
        QString szAddrType = "";
        IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pObj->m_szDevType);
        if (pDevPluginObj) {
            QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
            XMLObject xmlObj;
            if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                if(pRegAreasObj) {
                    QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                    foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                        if(pXmlObj->getProperty("Name") == pObj->m_szAddrType || pXmlObj->getProperty("Alias") == pObj->m_szAddrType) {
                            QString szAddrTypeAlias = pObj->m_szAddrType;
                            szAddrTypeAlias += pObj->m_szAddrOffset;
                            if(pObj->m_szAddrType2 != "") {
                                szAddrTypeAlias += ".";
                                szAddrTypeAlias += pObj->m_szAddrType2;
                                szAddrTypeAlias += pObj->m_szAddrOffset2;
                            }
                            szAddrType = szAddrTypeAlias; // 地址类型
                        }
                    }
                }
            }
        }
        rowDat << szAddrType;
    }
    rowDat << pObj->m_szDataType; // 数据类型
    QString szWriteable = "只读";
    if (pObj->m_iWriteable == 0) szWriteable = tr("只读");
    else if (pObj->m_iWriteable == 1) szWriteable = tr("可读可写");
    rowDat << szWriteable; // 读写

    rowDat << pObj->m_szUnit; // 单位
    rowDat << pObj->m_szRemark; // 变量描述
}


///
/// \brief QTableWidgetEx::onDoubleClicked
/// \details 单元格被双击
/// \param row 单元格所在行
/// \param column 单元格所在列
///
void QTableWidgetEx::onDoubleClicked(const QModelIndex &index) {
    if(index.row() < 0 || index.row() > m_pTagTableModel->m_tagRows.size())
        return;

    QStringList rowDat = m_pTagTableModel->m_tagRows.at(index.row());
    int iTagID = rowDat.at(0).toInt();
    for(int i=0; i<QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags.count(); i++) {
        Tag *pTagObj = QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags[i];
        if(pTagObj->m_iID == iTagID) {
            TagEditDialog dlg(this);
            dlg.setWindowTitle(tr("编辑变量"));
            QMap<QString, QStringList> mapDevToAddrType;
            QMap<QString, QStringList> mapAddrTypeToSubAddrType;
            QMap<QString, QStringList> mapAddrTypeToDataType;

            if(pTagObj->m_szDevType == "SYSTEM") { // 系统变量
                return;
            }
            else if(pTagObj->m_szDevType == "MEMORY") { // 内存变量
                QStringList szListAddrType;
                szListAddrType << tr("自动分配");
                mapDevToAddrType.insert("MEMORY", szListAddrType);
                mapAddrTypeToSubAddrType.insert(tr("自动分配"), QStringList());
                QStringList szListDataType;
                szListDataType << tr("bool")
                               << tr("int8")
                               << tr("uint8")
                               << tr("int16")
                               << tr("uint16")
                               << tr("int32")
                               << tr("uint32")
                               << tr("float32")
                               << tr("double")
                               << tr("bcd16")
                               << tr("bcd32");
                mapAddrTypeToDataType.insert(tr("自动分配"), szListDataType);
            } else {
                QStringList szListAddrType;
                QStringList szListDataType;
                IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pTagObj->m_szDevType);
                if (pDevPluginObj) {
                    QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
                    XMLObject xmlObj;
                    if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                        XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                        if(pRegAreasObj) {
                            QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                            foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                                QString szAddrType = pXmlObj->getProperty("Name");
                                szListAddrType << szAddrType;

                                if(pXmlObj->getProperty("DataType").trimmed() != "") mapAddrTypeToDataType.insert(szAddrType, pXmlObj->getProperty("DataType").split("|"));
                                else mapAddrTypeToDataType.insert(szAddrType, QStringList());

                                if(pXmlObj->getProperty("SubArea").trimmed() != "") mapAddrTypeToSubAddrType.insert(szAddrType, pXmlObj->getProperty("SubArea").split("|"));
                                else mapAddrTypeToSubAddrType.insert(szAddrType, QStringList());
                            }
                        }
                    }
                }
                mapDevToAddrType.insert(pTagObj->m_szDevType, szListAddrType);
            }

            dlg.setAddrTypeAndDataType(mapDevToAddrType, mapAddrTypeToSubAddrType, mapAddrTypeToDataType);
            QJsonObject jsonTagObj = pTagObj->toJsonObject();
            dlg.setTagObj(jsonTagObj);
            dlg.updateUI();
            if(dlg.exec() == QDialog::Accepted) {
                jsonTagObj = dlg.getTagObj();
                pTagObj->fromJsonObject(jsonTagObj);

                if(pTagObj->m_szDevType == "MEMORY") { // 内存变量
                    if(pTagObj->m_szAddrType == tr("自动分配")) pTagObj->m_szAddrType = "AutoAlloc";
                } else {
                    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pTagObj->m_szDevType);
                    if (pDevPluginObj) {
                        QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
                        XMLObject xmlObj;
                        if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                            XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                            if(pRegAreasObj) {
                                QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                                foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                                    if(pXmlObj->getProperty("Name") == pTagObj->m_szAddrType || pXmlObj->getProperty("Alias") == pTagObj->m_szAddrType) {
                                        if(pXmlObj->getProperty("Alias") != "") {
                                            pTagObj->m_szAddrType = pXmlObj->getProperty("Alias");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                updateTable();
            }
            return;
        }
    }
}

///
/// \brief QTableWidgetEx::contextMenuEvent
/// @details 右键菜单
///
void QTableWidgetEx::contextMenuEvent(QContextMenuEvent * event) {
    Q_UNUSED(event)
    QMenu *pMenu = new QMenu(this);

    QItemSelectionModel *pItemSelectionModel = this->selectionModel();
    QModelIndexList listModelIndex = pItemSelectionModel->selectedIndexes();
    QMap<int, int> tagIDMap;
    foreach (QModelIndex index, listModelIndex) {
        QStringList rowDat = m_pTagTableModel->m_tagRows.at(index.row());
        int iTagID = rowDat.at(0).toInt();
        tagIDMap.insert(iTagID, 0);
    }

    // 添加变量
    QAction *pAddTagAct = new QAction(tr("添加变量"), this);
    pAddTagAct->setStatusTip(tr("添加一个变量至当前变量表"));
    m_mapIdToAction.insert(TagAct_Add, pAddTagAct);
    connect(pAddTagAct, SIGNAL(triggered()), this, SLOT(onAddTag()));

    // 复制
    QAction *pCopyTagAct = new QAction(tr("复制"), this);
    pCopyTagAct->setStatusTip(tr("复制当前选中变量"));
    m_mapIdToAction.insert(TagAct_Copy, pCopyTagAct);
    connect(pCopyTagAct, SIGNAL(triggered()), this, SLOT(onCopyTag()));

    // 粘贴
    QAction *pPasteTagAct = new QAction(tr("粘贴"), this);
    pPasteTagAct->setStatusTip(tr("粘贴变量至当前变量表"));
    m_mapIdToAction.insert(TagAct_Paste, pPasteTagAct);
    connect(pPasteTagAct, SIGNAL(triggered()), this, SLOT(onPasteTag()));

    // 删除
    QAction *pDeleteTagAct = new QAction(tr("删除"), this);
    pDeleteTagAct->setStatusTip(tr("删除当前选中变量"));
    m_mapIdToAction.insert(TagAct_Delete, pDeleteTagAct);
    connect(pDeleteTagAct, SIGNAL(triggered()), this, SLOT(onDeleteTag()));

    // 属性
    QAction *pEditTagAct = new QAction(tr("属性"), this);
    pEditTagAct->setStatusTip(tr("复制当前选中变量"));
    m_mapIdToAction.insert(TagAct_Edit, pEditTagAct);
    connect(pEditTagAct, SIGNAL(triggered()), this, SLOT(onEditTag()));

    QModelIndex index = indexAt(event->pos());
    if(index.isValid()) { // 表格行
        if(tagIDMap.size() < 2) {
            pMenu->addAction(pAddTagAct);
        }
        pMenu->addAction(pCopyTagAct);
        pMenu->addAction(pPasteTagAct);
        pMenu->addAction(pDeleteTagAct);
        if(tagIDMap.size() < 2) pMenu->addAction(pEditTagAct);
    }
    else { // 表格行下空白部分
        pMenu->addAction(pAddTagAct);
        pMenu->addAction(pPasteTagAct);
    }
    pPasteTagAct->setEnabled(m_bCopyOrCutDone);

    if(!pMenu->isEmpty()) {
        pMenu->move(cursor().pos());
        pMenu->exec();
        pMenu->clear();
    }
    delete pMenu;
}

///
/// \brief QTableWidgetEx::mousePressEvent
/// \details 鼠标按下事件
/// \param event
///
void QTableWidgetEx::mousePressEvent(QMouseEvent *event) {
    if (event->button()==Qt::LeftButton || event->button()==Qt::RightButton) {
        QModelIndex index = indexAt(event->pos());
        if(!index.isValid()) { // 单击空白部分
            this->clearSelection();
            emit clickedBlankArea();
        }
    }
    QTableView::mousePressEvent(event);
}


///
/// \brief QTableWidgetEx::onAddTag
/// \details 添加变量
///
void QTableWidgetEx::onAddTag() {
    TagEditDialog dlg(this);
    dlg.setWindowTitle(tr("新建变量"));
    QMap<QString, QStringList> mapDevToAddrType;
    QMap<QString, QStringList> mapAddrTypeToSubAddrType;
    QMap<QString, QStringList> mapAddrTypeToDataType;

    QStringList szListAddrType;
    szListAddrType << tr("自动分配");
    mapDevToAddrType.insert("MEMORY", szListAddrType);
    mapAddrTypeToSubAddrType.insert(tr("自动分配"), QStringList());
    QStringList szListDataType;
    szListDataType << tr("bool")
                   << tr("int8")
                   << tr("uint8")
                   << tr("int16")
                   << tr("uint16")
                   << tr("int32")
                   << tr("uint32")
                   << tr("float32")
                   << tr("double")
                   << tr("bcd16")
                   << tr("bcd32");
    mapAddrTypeToDataType.insert(tr("自动分配"), szListDataType);

    //--------------------------------------------------------------------------

    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        QStringList szListAddrType;
        QStringList szListDataType;
        IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pObj->szName_);
        if (pDevPluginObj) {
            QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
            XMLObject xmlObj;
            if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                if(pRegAreasObj) {
                    QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                    foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                        QString szAddrType = pXmlObj->getProperty("Name");
                        szListAddrType << szAddrType;

                        if(pXmlObj->getProperty("DataType").trimmed() != "") mapAddrTypeToDataType.insert(szAddrType, pXmlObj->getProperty("DataType").split("|"));
                        else mapAddrTypeToDataType.insert(szAddrType, QStringList());

                        if(pXmlObj->getProperty("SubArea").trimmed() != "") mapAddrTypeToSubAddrType.insert(szAddrType, pXmlObj->getProperty("SubArea").split("|"));
                        else mapAddrTypeToSubAddrType.insert(szAddrType, QStringList());
                    }
                }
            }
        }
        mapDevToAddrType.insert(pObj->szName_, szListAddrType);
    }

    dlg.setAddrTypeAndDataType(mapDevToAddrType, mapAddrTypeToSubAddrType, mapAddrTypeToDataType);
    QJsonObject jsonTagObj;
    dlg.setTagObj(jsonTagObj);
    dlg.updateUI();
    if(dlg.exec() == QDialog::Accepted) {
        Tag *pTagObj = new Tag();
        pTagObj->fromJsonObject(dlg.getTagObj());
        pTagObj->m_iID = QSoftCore::getCore()->getProjectCore()->tagMgr_.allocID();

        if(pTagObj->m_szDevType == "MEMORY") { // 内存变量
            if(pTagObj->m_szAddrType == tr("自动分配")) pTagObj->m_szAddrType = "AutoAlloc";
        } else {
            IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pTagObj->m_szDevType);
            if (pDevPluginObj) {
                QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
                XMLObject xmlObj;
                if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                    XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                    if(pRegAreasObj) {
                        QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                        foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                            if(pXmlObj->getProperty("Name") == pTagObj->m_szAddrType || pXmlObj->getProperty("Alias") == pTagObj->m_szAddrType) {
                                if(pXmlObj->getProperty("Alias") != "") {
                                    pTagObj->m_szAddrType = pXmlObj->getProperty("Alias");
                                }
                            }
                        }
                    }
                }
            }
        }
        QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags.append(pTagObj);
        updateTable();
    }
}


///
/// \brief QTableWidgetEx::onCopyTag
/// \details 复制变量
///
void QTableWidgetEx::onCopyTag() {
    QItemSelectionModel *pItemSelectionModel = this->selectionModel();
    QModelIndexList listModelIndex = pItemSelectionModel->selectedIndexes();
    QMap<int, int> tagIDMap;
    foreach (QModelIndex index, listModelIndex) {
        QStringList rowDat = m_pTagTableModel->m_tagRows.at(index.row());
        int iTagID = rowDat.at(0).toInt();
        if(rowDat.at(1).startsWith(QString("$"))) continue; // 系统变量不可以复制
        tagIDMap.insert(iTagID, 0);
    }

    QMapIterator<int, int> tagIDMapIterator(tagIDMap);
    while (tagIDMapIterator.hasNext()) {
        tagIDMapIterator.next();
        int iTagID = tagIDMapIterator.key();
        foreach (Tag *pTagObj, QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags) {
            if(pTagObj->m_iID == iTagID) {
                emit copyOrCutTagToClipboard();
                setActionEnable(TagAct_Paste, true);
                QClipboard *clipboard = QApplication::clipboard();
                clipboard->setText(pTagObj->toXmlNodeString());
                m_bCopyOrCutDone = true;
                return;
            }
        }
    }
}

///
/// \brief QTableWidgetEx::onEditTag
/// \details 粘贴变量
///
void QTableWidgetEx::onPasteTag()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString szTagObj = clipboard->text();

    Tag *pTagObj = new Tag;
    if(pTagObj->fromXmlNodeString(szTagObj)) {
        pTagObj->m_iID = QSoftCore::getCore()->getProjectCore()->tagMgr_.allocID();
        QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags.append(pTagObj);
        updateTable();
    } else {
        delete pTagObj;
        pTagObj = Q_NULLPTR;
    }
}


///
/// \brief QTableWidgetEx::onEditTag
/// \details 删除变量
///
void QTableWidgetEx::onDeleteTag() {
    bool bUpdate = false;

    QItemSelectionModel *pItemSelectionModel = this->selectionModel();
    QModelIndexList listModelIndex = pItemSelectionModel->selectedIndexes();
    QMap<int, int> tagIDMap;
    foreach (QModelIndex index, listModelIndex) {
        QStringList rowDat = m_pTagTableModel->m_tagRows.at(index.row());
        int iTagID = rowDat.at(0).toInt();
        if(rowDat.at(1).startsWith(QString("$"))) continue; // 系统变量不可以删除
        tagIDMap.insert(iTagID, 0);
    }

    int iIDToDel;
    QMapIterator<int, int> tagIDMapIterator(tagIDMap);
    tagIDMapIterator.toBack();
    while (tagIDMapIterator.hasPrevious()) {
        tagIDMapIterator.previous();
        iIDToDel = tagIDMapIterator.key();
        Tag *pFindTagObj = Q_NULLPTR;
        foreach (Tag *pTagObj, QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags) {
            if(pTagObj->m_iID == iIDToDel) {
                pFindTagObj = pTagObj;
                break;
            }
        }
        if(pFindTagObj != Q_NULLPTR) {
            QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags.removeOne(pFindTagObj);
            bUpdate = true;
        }
    }

    if(bUpdate) updateTable();
}


///
/// \brief QTableWidgetEx::onEditTag
/// \details 编辑变量属性
///
void QTableWidgetEx::onEditTag() {
    int iRow = this->currentIndex().row();
    QStringList rowDat = m_pTagTableModel->m_tagRows.at(iRow);
    int iTagID = rowDat.at(0).toInt();
    if(rowDat.at(1).startsWith(QString("$"))) return; // 系统变量不可以编辑
    foreach (Tag *pTagObj, QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags) {
        if(pTagObj->m_iID == iTagID) {
            TagEditDialog dlg(this);
            dlg.setWindowTitle(tr("编辑变量"));
            QMap<QString, QStringList> mapDevToAddrType;
            QMap<QString, QStringList> mapAddrTypeToSubAddrType;
            QMap<QString, QStringList> mapAddrTypeToDataType;

            QStringList szListAddrType;
            szListAddrType << tr("自动分配");
            mapDevToAddrType.insert("MEMORY", szListAddrType);
            mapAddrTypeToSubAddrType.insert(tr("自动分配"), QStringList());
            QStringList szListDataType;
            szListDataType << tr("bool")
                           << tr("int8")
                           << tr("uint8")
                           << tr("int16")
                           << tr("uint16")
                           << tr("int32")
                           << tr("uint32")
                           << tr("float32")
                           << tr("double")
                           << tr("bcd16")
                           << tr("bcd32");
            mapAddrTypeToDataType.insert(tr("自动分配"), szListDataType);

            //--------------------------------------------------------------------------

            DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
            for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
                DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
                QStringList szListAddrType;
                QStringList szListDataType;
                IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pObj->szName_);
                if (pDevPluginObj) {
                    QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
                    XMLObject xmlObj;
                    if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                        XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                        if(pRegAreasObj) {
                            QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                            foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                                QString szAddrType = pXmlObj->getProperty("Name");
                                szListAddrType << szAddrType;

                                if(pXmlObj->getProperty("DataType").trimmed() != "") mapAddrTypeToDataType.insert(szAddrType, pXmlObj->getProperty("DataType").split("|"));
                                else mapAddrTypeToDataType.insert(szAddrType, QStringList());

                                if(pXmlObj->getProperty("SubArea").trimmed() != "") mapAddrTypeToSubAddrType.insert(szAddrType, pXmlObj->getProperty("SubArea").split("|"));
                                else mapAddrTypeToSubAddrType.insert(szAddrType, QStringList());
                            }
                        }
                    }
                }
                mapDevToAddrType.insert(pObj->szName_, szListAddrType);
            }

            dlg.setAddrTypeAndDataType(mapDevToAddrType, mapAddrTypeToSubAddrType, mapAddrTypeToDataType);
            QJsonObject jsonTagObj = pTagObj->toJsonObject();
            dlg.setTagObj(jsonTagObj);
            dlg.updateUI();
            if(dlg.exec() == QDialog::Accepted) {
                pTagObj->fromJsonObject(dlg.getTagObj());
                if(pTagObj->m_szDevType == "MEMORY") { // 内存变量
                    if(pTagObj->m_szAddrType == tr("自动分配")) pTagObj->m_szAddrType = "AutoAlloc";
                } else {
                    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pTagObj->m_szDevType);
                    if (pDevPluginObj) {
                        QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
                        XMLObject xmlObj;
                        if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                            XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                            if(pRegAreasObj) {
                                QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                                foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                                    if(pXmlObj->getProperty("Name") == pTagObj->m_szAddrType || pXmlObj->getProperty("Alias") == pTagObj->m_szAddrType) {
                                        if(pXmlObj->getProperty("Alias") != "") {
                                            pTagObj->m_szAddrType = pXmlObj->getProperty("Alias");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                updateTable();
            }
            return;
        }
    }
}


///
/// \brief QTableWidgetEx::setActionEnable
/// \details 启用或禁用功能菜单
/// \param id 功能菜单ID
/// \param enable true-启用, false-禁用
///
void QTableWidgetEx::setActionEnable(TagTableActonType id, bool enable) {
    QAction* pActObj = m_mapIdToAction[id];
    if(pActObj != Q_NULLPTR) {
        pActObj->setEnabled(enable);
    }
}

///
/// \brief QTableWidgetEx::onExportToCsv
/// \details 导出变量至CSV
///
void QTableWidgetEx::onExportToCsv()
{
    QString szDirPath = QCoreApplication::applicationDirPath();
    QString szSaveCsvPath = QFileDialog::getExistingDirectory(this,
                                                              tr("选择导出csv路径"),
                                                              szDirPath,
                                                              QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (szSaveCsvPath == "") return;

    QtCSV::StringData varData;
    foreach (Tag *pTagObj, QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags) {
        QStringList varRow;
        varRow << QString::number(pTagObj->m_iID)
               << pTagObj->m_szName
               << pTagObj->m_szUnit
               << pTagObj->m_szAddrType
               << pTagObj->m_szAddrOffset
               << pTagObj->m_szAddrType2
               << pTagObj->m_szAddrOffset2
               << pTagObj->m_szDataType
               << QString::number(pTagObj->m_iWriteable)
               << pTagObj->m_szRemark
               << pTagObj->m_szOwnGroup
               << pTagObj->m_szDevType;
        varData.addRow(varRow);
    }

    QString filepath = szSaveCsvPath + "/变量.csv";
    QStringList header;
    header << tr("ID")
           << tr("名称")
           << tr("单位")
           << tr("地址类型")
           << tr("地址偏移")
           << tr("地址类型2")
           << tr("地址偏移2")
           << tr("数据类型")
           << tr("读写类型")
           << tr("描述")
           << tr("所属组")
           << tr("设备类型");

    QtCSV::Writer::write(filepath,
                         varData,
                         QString(","),
                         QString("\""),
                         QtCSV::Writer::REWRITE,
                         header,
                         QStringList(),
                         QTextCodec::codecForName("GB18030"));
}

///
/// \brief QTableWidgetEx::onImportFromCsv
/// \details 从CSV导入变量
///
void QTableWidgetEx::onImportFromCsv()
{
    QString szDirPath = QCoreApplication::applicationDirPath();
    QString szSaveCsvFile = QFileDialog::getOpenFileName(this,
                                                         tr("选择csv文件"),
                                                         szDirPath,
                                                         tr("csv file (*.csv)"));
    if(szSaveCsvFile == "") return;

    QList<QStringList> data = QtCSV::Reader::readToList(szSaveCsvFile,
                                                        QString(","),
                                                        QString("\""),
                                                        QTextCodec::codecForName("GB18030"));
    for(int i=0; i<data.size(); i++) {
        QStringList row = data.at(i);
        if(row.at(0) == "ID") continue;
        Tag *pObj = new Tag();
        pObj->m_iID = QSoftCore::getCore()->getProjectCore()->tagMgr_.allocID();
        pObj->m_szName = row.at(1);
        pObj->m_szUnit = row.at(2);
        pObj->m_szAddrType = row.at(3);
        pObj->m_szAddrOffset = row.at(4);
        pObj->m_szAddrType2 = row.at(5);
        pObj->m_szAddrOffset2 = row.at(6);
        pObj->m_szDataType = row.at(7);
        pObj->m_iWriteable = row.at(8).toInt();
        pObj->m_szRemark = row.at(9);
        pObj->m_szOwnGroup = row.at(10);
        pObj->m_szDevType = row.at(11);
        QSoftCore::getCore()->getProjectCore()->tagMgr_.m_vecTags.append(pObj);
    }

    this->updateTable();
}

//------------------------------------------------------------------------------


TagManagerChild::TagManagerChild(QWidget *parent) : QWidget(parent)
{
    m_pTopVLayoutObj = new QVBoxLayout(this);
    m_pTopVLayoutObj->setSpacing(1);
    m_pTopVLayoutObj->setContentsMargins(1, 1, 1, 1);

    m_pTagMgrTableViewObj = new QTableWidgetEx(this);
    m_pTagMgrTableViewObj->setObjectName(QString::fromUtf8("TagManagerTableView"));

    m_pTopVLayoutObj->addWidget(m_pTagMgrTableViewObj);
}

TagManagerChild::~TagManagerChild()
{
    DEL_OBJ(m_pTagMgrTableViewObj);
    DEL_OBJ(m_pTopVLayoutObj);
}


void TagManagerChild::buildUserInterface(QMainWindow* pMainWin)
{
    if(pMainWin == Q_NULLPTR) return;

    if(m_pMainWinObj == Q_NULLPTR) {

        //-----------------------------<变量编辑菜单>----------------------------------
        // 添加变量
        m_pActAddTagObj = new QAction(QIcon(":/images/data_add.png"), tr("添加变量"));
        connect(m_pActAddTagObj, SIGNAL(triggered()), m_pTagMgrTableViewObj, SLOT(onAddTag()));

        // 拷贝变量
        m_pActCopyTagObj = new QAction(QIcon(":/images/data_rowcopy.png"), tr("拷贝变量"));
        connect(m_pActCopyTagObj, SIGNAL(triggered()), m_pTagMgrTableViewObj, SLOT(onCopyTag()));

        // 粘贴变量
        m_pActPasteTagObj = new QAction(QIcon(":/images/data_supadd.png"), tr("粘贴变量"));
        connect(m_pActPasteTagObj, SIGNAL(triggered()), m_pTagMgrTableViewObj, SLOT(onPasteTag()));

        // 修改变量
        m_pActModifyTagObj = new QAction(QIcon(":/images/icon_modify.png"), tr("修改变量"));
        connect(m_pActModifyTagObj, SIGNAL(triggered()), m_pTagMgrTableViewObj, SLOT(onEditTag()));

        // 删除变量
        m_pActDeleteTagObj = new QAction(QIcon(":/images/icon_delete.png"), tr("删除变量"));
        connect(m_pActDeleteTagObj, SIGNAL(triggered()), m_pTagMgrTableViewObj, SLOT(onDeleteTag()));

        // 导出变量
        m_pActExportTagObj = new QAction(QIcon(":/images/data_export.png"), tr("导出变量"));
        connect(m_pActExportTagObj, SIGNAL(triggered()), m_pTagMgrTableViewObj, SLOT(onExportToCsv()));

        // 导入变量
        m_pActImportTagObj = new QAction(QIcon(":/images/data_import.png"), tr("导入变量"));
        connect(m_pActImportTagObj, SIGNAL(triggered()), m_pTagMgrTableViewObj, SLOT(onImportFromCsv()));

        //-----------------------------<变量编辑菜单>----------------------------------
        m_pMenuTagEditObj = pMainWin->menuBar()->addMenu(tr("变量编辑"));
        m_pMenuTagEditObj->addAction(m_pActAddTagObj); // 添加变量
        m_pMenuTagEditObj->addAction(m_pActCopyTagObj); // 拷贝变量
        m_pMenuTagEditObj->addAction(m_pActPasteTagObj); // 粘贴变量
        m_pMenuTagEditObj->addAction(m_pActModifyTagObj); // 修改变量
        m_pMenuTagEditObj->addAction(m_pActDeleteTagObj); // 删除变量
        m_pMenuTagEditObj->addAction(m_pActExportTagObj); // 导出变量
        m_pMenuTagEditObj->addAction(m_pActImportTagObj); // 导入变量

        //-----------------------------<变量编辑>----------------------------------
        m_pToolBarTagEditObj = new QToolBar(pMainWin);
        m_pToolBarTagEditObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_pToolBarTagEditObj->addAction(m_pActAddTagObj);  // 添加变量
        m_pToolBarTagEditObj->addAction(m_pActCopyTagObj); // 拷贝变量
        m_pToolBarTagEditObj->addAction(m_pActPasteTagObj); // 粘贴变量
        m_pToolBarTagEditObj->addAction(m_pActModifyTagObj); // 修改变量
        m_pToolBarTagEditObj->addAction(m_pActDeleteTagObj); // 删除变量
        m_pToolBarTagEditObj->addAction(m_pActExportTagObj); // 导出变量
        m_pToolBarTagEditObj->addAction(m_pActImportTagObj); // 导入变量

        pMainWin->addToolBar(Qt::TopToolBarArea, m_pToolBarTagEditObj);

        m_pMainWinObj = pMainWin;
    }

    //////////////////////////////////////////////////////////////////////////

    m_pTagMgrTableViewObj->updateTable();
}

void TagManagerChild::removeUserInterface(QMainWindow* pMainWin)
{
    if(pMainWin == Q_NULLPTR) return;

    if(m_pMainWinObj) {
        m_pMenuTagEditObj->clear();
        DEL_OBJ(m_pActAddTagObj);
        DEL_OBJ(m_pActCopyTagObj);
        DEL_OBJ(m_pActPasteTagObj);
        DEL_OBJ(m_pActModifyTagObj);
        DEL_OBJ(m_pActDeleteTagObj);
        DEL_OBJ(m_pActExportTagObj);
        DEL_OBJ(m_pActImportTagObj);

        pMainWin->menuBar()->removeAction(m_pMenuTagEditObj->menuAction());
        DEL_OBJ(m_pMenuTagEditObj);

        pMainWin->removeToolBar(m_pToolBarTagEditObj);

        m_pMainWinObj = Q_NULLPTR;
    }
}

QString TagManagerChild::wndTitle() const
{
    return this->windowTitle();
}





