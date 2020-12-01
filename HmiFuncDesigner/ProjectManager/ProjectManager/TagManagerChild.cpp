#include "TagManagerChild.h"
#include "NewComDeviceDialog.h"
#include "stringdata.h"
#include "writer.h"
#include "reader.h"
#include "variantdata.h"
#include "Helper.h"
#include "ProjectData.h"
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
#include "ProjectData.h"
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
#include "Tag.h"
#include "TagEditDialog.h"
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
    this->setColumnWidth(1, 80); // 变量名称
    this->setColumnWidth(2, 80); // 变量地址
    this->setColumnWidth(3, 80); // 数据类型
    this->setColumnWidth(4, 80); // 读写
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
    foreach (Tag *pTagObj, ProjectData::getInstance()->tagMgr_.m_vecTags) {
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
    rowDat << QString("$%1").arg(pObj->m_szName); // 变量名称
    rowDat << pObj->m_szDataType; // 数据类型

    if(pObj->m_szDevType == "MEMORY") { // 内存变量
        rowDat << "AutoAlloc"; // 地址类型
    } else if(pObj->m_szDevType == "SYSTEM") { // 系统变量
        rowDat << "AutoAlloc"; // 地址类型
    } else {
        IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pObj->m_szDevType);
        if (pDevPluginObj) {
            QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
            XMLObject xmlObj;
            if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
                XMLObject*pRegAreasObj = xmlObj.getCurrentChild("RegAreas");
                if(pRegAreasObj) {
                    QVector<XMLObject* > pRegAreaObjs = pRegAreasObj->getCurrentChildren("RegArea");
                    foreach(XMLObject* pXmlObj, pRegAreaObjs) {
                        if(pXmlObj->getProperty("Name") == pObj->m_szName || pXmlObj->getProperty("Alias") == pObj->m_szName) {
                            QString szAddrTypeAlias = pObj->m_szAddrType;
                            szAddrTypeAlias += pObj->m_szAddrOffset;
                            if(pObj->m_szAddrType2 != "") {
                                szAddrTypeAlias += ".";
                                szAddrTypeAlias += pObj->m_szAddrType2;
                                szAddrTypeAlias += pObj->m_szAddrOffset2;
                            }
                            rowDat << szAddrTypeAlias; // 地址类型
                        }
                    }
                }
            }
        }
    }

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
    for(int i=0; i<ProjectData::getInstance()->tagMgr_.m_vecTags.count(); i++) {
        Tag *pTagObj = ProjectData::getInstance()->tagMgr_.m_vecTags[i];
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
                szListAddrType << "AutoAlloc";
                mapDevToAddrType.insert("MEMORY", szListAddrType);
                mapAddrTypeToSubAddrType.insert("AutoAlloc", QStringList());
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
                mapAddrTypeToDataType.insert("AutoAlloc", szListDataType);
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
                                if(pXmlObj->getProperty("Alias") != "") szAddrType = pXmlObj->getProperty("Alias");
                                szListAddrType << szAddrType;
                                mapAddrTypeToDataType.insert(szAddrType, pXmlObj->getProperty("DataType").split("|"));
                                mapAddrTypeToSubAddrType.insert(szAddrType, pXmlObj->getProperty("SubArea").split("|"));
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
    szListAddrType << "AutoAlloc";
    mapDevToAddrType.insert("MEMORY", szListAddrType);
    mapAddrTypeToSubAddrType.insert("AutoAlloc", QStringList());
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
    mapAddrTypeToDataType.insert("AutoAlloc", szListDataType);

    //--------------------------------------------------------------------------

    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
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
                        if(pXmlObj->getProperty("Alias") != "") szAddrType = pXmlObj->getProperty("Alias");
                        szListAddrType << szAddrType;
                        mapAddrTypeToDataType.insert(szAddrType, pXmlObj->getProperty("DataType").split("|"));
                        mapAddrTypeToSubAddrType.insert(szAddrType, pXmlObj->getProperty("SubArea").split("|"));
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
        pTagObj->m_iID = ProjectData::getInstance()->tagMgr_.allocID();
        pTagObj->m_szDevType = "SYSTEM";
        ProjectData::getInstance()->tagMgr_.m_vecTags.append(pTagObj);
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
        tagIDMap.insert(iTagID, 0);
    }

    QMapIterator<int, int> tagIDMapIterator(tagIDMap);
    while (tagIDMapIterator.hasNext()) {
        tagIDMapIterator.next();
        int iTagID = tagIDMapIterator.key();
        foreach (Tag *pTagObj, ProjectData::getInstance()->tagMgr_.m_vecTags) {
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
        pTagObj->m_iID = ProjectData::getInstance()->tagMgr_.allocID();
        ProjectData::getInstance()->tagMgr_.m_vecTags.append(pTagObj);
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
        tagIDMap.insert(iTagID, 0);
    }

    int iIDToDel;
    QMapIterator<int, int> tagIDMapIterator(tagIDMap);
    tagIDMapIterator.toBack();
    while (tagIDMapIterator.hasPrevious()) {
        tagIDMapIterator.previous();
        iIDToDel = tagIDMapIterator.key();
        Tag *pFindTagObj = Q_NULLPTR;
        foreach (Tag *pTagObj, ProjectData::getInstance()->tagMgr_.m_vecTags) {
            if(pTagObj->m_iID == iIDToDel) {
                pFindTagObj = pTagObj;
                break;
            }
        }
        if(pFindTagObj != Q_NULLPTR) {
            ProjectData::getInstance()->tagMgr_.m_vecTags.removeOne(pFindTagObj);
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
    foreach (Tag *pTagObj, ProjectData::getInstance()->tagMgr_.m_vecTags) {
        if(pTagObj->m_iID == iTagID) {
            TagEditDialog dlg(this);
            dlg.setWindowTitle(tr("编辑变量"));
            QMap<QString, QStringList> mapDevToAddrType;
            QMap<QString, QStringList> mapAddrTypeToSubAddrType;
            QMap<QString, QStringList> mapAddrTypeToDataType;

            QStringList szListAddrType;
            szListAddrType << "AutoAlloc";
            mapDevToAddrType.insert("MEMORY", szListAddrType);
            mapAddrTypeToSubAddrType.insert("AutoAlloc", QStringList());
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
            mapAddrTypeToDataType.insert("AutoAlloc", szListDataType);

            //--------------------------------------------------------------------------

            DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
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
                                if(pXmlObj->getProperty("Alias") != "") szAddrType = pXmlObj->getProperty("Alias");
                                szListAddrType << szAddrType;
                                mapAddrTypeToDataType.insert(szAddrType, pXmlObj->getProperty("DataType").split("|"));
                                mapAddrTypeToSubAddrType.insert(szAddrType, pXmlObj->getProperty("SubArea").split("|"));
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



//------------------------------------------------------------------------------


TagManagerChild::TagManagerChild(QWidget *parent) : QWidget(parent)
{
    initUi();
    setContextMenuPolicy(Qt::DefaultContextMenu);

}

TagManagerChild::~TagManagerChild()
{
    DEL_OBJ(m_pTagMgrTableViewObj);
    DEL_OBJ(m_pTopVLayoutObj);
}

void TagManagerChild::initUi()
{
    m_pTopVLayoutObj = new QVBoxLayout(this);
    m_pTopVLayoutObj->setSpacing(1);
    m_pTopVLayoutObj->setContentsMargins(1, 1, 1, 1);

    m_pTagMgrTableViewObj = new QTableWidgetEx(this);
    m_pTagMgrTableViewObj->setObjectName(QString::fromUtf8("TagManagerTableView"));

    m_pTopVLayoutObj->addWidget(m_pTagMgrTableViewObj);
}

/*
* 导出变量表
*/
void TagManagerChild::exportToCsv(const QString &path)
{
    if(wndTitle().startsWith(tr("设备变量"))) {
        // 设备变量表变量导出至CSV文件
        this->tagIOExportToCsv(path, QString("%1-%2").arg(tr("设备变量")).arg(m_szItemName));
    } else if(wndTitle().startsWith(tr("中间变量"))) {
        // 中间变量表变量导出至CSV文件
        tagTmpExportToCsv(path, m_szItemName);
    }
}


/*
* 导入变量表
*/
void TagManagerChild::importFromCsv(const QString &path)
{
    if(path.contains(tr("设备变量"))) {
        // 从CSV文件导入设备变量至设备变量表
        this->tagIOImportFromCsv(path);
    } else if(path.contains(tr("中间变量"))) {
        // 从CSV文件导入中间变量至中间变量表
        tagTmpImportFromCsv(path);
    }
}


/*
* 右键菜单
*/
void TagManagerChild::contextMenuEvent(QContextMenuEvent * /*event*/)
{  
    QMenu *pMenu = new QMenu(this);

    QAction *pAddAct = new QAction(QIcon(":/images/data_add.png"), tr("增加"), this);
    pAddAct->setStatusTip(tr("新增变量"));
    connect(pAddAct, SIGNAL(triggered()), this, SLOT(onSlotAddTag()));
    pMenu->addAction(pAddAct);

    QAction *pAppendAct = new QAction(QIcon(":/images/data_supadd.png"), tr("追加"), this);
    pAppendAct->setStatusTip(tr("追加"));
    connect(pAppendAct, SIGNAL(triggered()), this, SLOT(onSlotAppendTag()));
    pMenu->addAction(pAppendAct);

    QAction *pRowCopyAct = new QAction(QIcon(":/images/data_rowcopy.png"), tr("行拷"), this);
    pRowCopyAct->setStatusTip(tr("行拷"));
    connect(pRowCopyAct, SIGNAL(triggered()), this, SLOT(onSlotRowCopyTag()));
    pMenu->addAction(pRowCopyAct);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    pModifyAct->setStatusTip(tr("修改"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(onSlotModifyTag()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/data_delete.png"), tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(onSlotDeleteTag()));
    pMenu->addAction(pDeleteAct);

    if(!pMenu->isEmpty()) {
        pMenu->move(cursor().pos());
        pMenu->exec();
        pMenu->clear();
    }
    delete pMenu;
}

/**
 * @brief TagManagerChild::onSlotAddTag
 * @details 增加变量
 */
void TagManagerChild::onSlotAddTag()
{
    if(wndTitle().startsWith(tr("设备变量"))) {
        // 创建设备变量
        createTagIO();
    } else if(wndTitle().startsWith(tr("中间变量"))) {
        // 创建中间变量
        createTagTmp();
    }
}

/**
 * @brief TagManagerChild::onSlotAppendTag
 * @details 追加变量
 */
void TagManagerChild::onSlotAppendTag()
{
    if(wndTitle().startsWith(tr("设备变量"))) {
        // 追加设备变量
        appendTagIO();
    } else if(wndTitle().startsWith(tr("中间变量"))) {
        // 追加中间变量
        appendTagTmp();
    }
}


/**
 * @brief TagManagerChild::onSlotRowCopyTag
 * @details 行拷贝变量
 */
void TagManagerChild::onSlotRowCopyTag()
{
    if(wndTitle().startsWith(tr("设备变量"))) {
        // 拷贝选中设备变量
        copyCurTagIO();
    } else if(wndTitle().startsWith(tr("中间变量"))) {
        // 拷贝选中中间变量
        copyCurTagTmp();
    }
}


/**
 * @brief TagManagerChild::onSlotModifyTag
 * @details 修改变量
 */
void TagManagerChild::onSlotModifyTag()
{
    if(wndTitle().startsWith(tr("设备变量"))) {
        // 修改选中设备变量
        modifyCurTagIO();
    } else if(wndTitle().startsWith(tr("中间变量"))) {
        // 修改选中中间变量
        modifyCurTagTmp();
    }
}

/**
 * @brief TagManagerChild::onSlotDeleteTag
 * @details 删除变量
 */
void TagManagerChild::onSlotDeleteTag()
{
    if(wndTitle().startsWith(tr("设备变量"))) {
        // 删除选中设备变量
        deleteCurTagIO();
    } else if(wndTitle().startsWith(tr("中间变量"))) {
        // 删除选中中间变量
        deleteCurTagTmp();
    }
}


/*
 * 关闭事件
 */
void TagManagerChild::closeEvent(QCloseEvent *event)
{
    event->accept();
}


/**
 * @brief TagManagerChild::tableTagTmpAddRow
 * @details 中间变量表新增一行
 * @return 新增的行号
 */
int TagManagerChild::tableTagTmpAddRow()
{
    return  0;
    //#define XX(item) \
    //    item->setFlags(item->flags() | Qt::ItemIsEditable);

    //    int iRowCount = m_pTableTagTmpObj->rowCount();
    //    m_pTableTagTmpObj->insertRow(iRowCount);

    //    QTableWidgetItem *pItemID = new QTableWidgetItem("");
    //    m_pTableTagTmpObj->setItem(iRowCount, 0, pItemID);

    //    QTableWidgetItem *pItemName = new QTableWidgetItem("");
    //    XX(pItemName);
    //    m_pTableTagTmpObj->setItem(iRowCount, 1, pItemName);

    //    QTableWidgetItem *pItemDescription = new QTableWidgetItem("");
    //    XX(pItemDescription);
    //    m_pTableTagTmpObj->setItem(iRowCount, 2, pItemDescription);

    //    QTableWidgetItem *pItemDataType = new QTableWidgetItem("");
    //    XX(pItemDataType);
    //    m_pTableTagTmpObj->setItem(iRowCount, 3, pItemDataType);

    //    QTableWidgetItem *pItemInitVal = new QTableWidgetItem("");
    //    XX(pItemInitVal);
    //    m_pTableTagTmpObj->setItem(iRowCount, 4, pItemInitVal);

    //    QTableWidgetItem *pItemMinVal = new QTableWidgetItem("");
    //    XX(pItemMinVal);
    //    m_pTableTagTmpObj->setItem(iRowCount, 5, pItemMinVal);

    //    QTableWidgetItem *pItemMaxVal = new QTableWidgetItem("");
    //    XX(pItemMaxVal);
    //    m_pTableTagTmpObj->setItem(iRowCount, 6, pItemMaxVal);

    //    QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem("");
    //    m_pTableTagTmpObj->setItem(iRowCount, 7, pItemProjectConverter);

    //#undef XX
    //    return iRowCount;
}


/**
 * @brief TagManagerChild::getTagTmpIdNumValue
 * @details 获取指定行中间变量id数值部分
 * @param iRow 指定行
 * @return 中间变量id数值部分
 */
int TagManagerChild::getTagTmpIdNumValue(int iRow)
{
    int id = 0;
    //    int iRowCnt = m_pTableTagTmpObj->rowCount();
    //    if(iRowCnt > 0) {
    //        QString szIDText = "";
    //        QTableWidgetItem *pItemID = m_pTableTagTmpObj->item(iRow, 0);
    //        if(pItemID != Q_NULLPTR) {
    //            szIDText = pItemID->text();
    //        }
    //        QString szStartText = "tmp.";
    //        QString szTmp = "0";
    //        if(szIDText.startsWith(szStartText)) {
    //            szTmp = szIDText.remove(0, szStartText.length());
    //            id = szTmp.toInt();
    //        }
    //    }
    return id;
}

/**
 * @brief TagManagerChild::updateTableTagTmp
 * @details 刷新中间变量表
 */
void TagManagerChild::updateTableTagTmp()
{
    //    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    //    m_pTableTagTmpObj->clearContents();
    //    m_pTableTagTmpObj->setRowCount(0);
    //    foreach (TagTmpDBItem * itemTagTmp, tagTmp.listTagTmpDBItem_) {
    //        int iRow = tableTagTmpAddRow();
    //        setTagTmpObjByRow(iRow, itemTagTmp);
    //    }
}


/**
 * @brief TagManagerChild::tagTmpExportToCsv
 * @details 中间变量表变量导出至CSV文件
 * @param path
 */
void TagManagerChild::tagTmpExportToCsv(const QString &path, const QString &/*group*/)
{
    QtCSV::StringData varData;
    int iRowCount = 0;//m_pTableTagTmpObj->rowCount();
    for(int i=0; i<iRowCount; i++) {
        //        TagTmpDBItem * pTagTmp = getTagTmpObjByRow(i);

        //        QStringList varRow;
        //        varRow << pTagTmp->m_szTagID << pTagTmp->m_szName << pTagTmp->m_szDataType
        //               << pTagTmp->m_szDescription << pTagTmp->m_szInitVal << pTagTmp->m_szMinVal
        //               << pTagTmp->m_szMaxVal << pTagTmp->m_szProjectConverter ;
        //        varData.addRow(varRow);

        //        if(pTagTmp != Q_NULLPTR) {
        //            delete pTagTmp;
        //            pTagTmp = Q_NULLPTR;
        //        }
    }

    QString filepath = path + "/中间变量.csv";
    QStringList header;
    header << tr("ID") << tr("名称") << tr("数据类型") << tr("变量描述")
           << tr("初始值") << tr("最小值") << tr("最大值") << tr("工程转换");
    QtCSV::Writer::write(filepath, varData, QString(","), QString("\""),
                         QtCSV::Writer::REWRITE, header,  QStringList(),
                         QTextCodec::codecForName("GB18030"));
}

/**
 * @brief TagManagerChild::tagTmpImportFromCsv
 * @details 从CSV文件导入中间变量至中间变量表
 * @param path
 */
void TagManagerChild::tagTmpImportFromCsv(const QString &path)
{
    //    QString filepath = path;

    //    QList<QStringList> data = QtCSV::Reader::readToList(filepath,
    //                                                        QString(","),
    //                                                        QString("\""),
    //                                                        QTextCodec::codecForName("GB18030"));
    //    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    //    for(int i=0; i<data.size(); i++) {
    //        QStringList row = data.at(i);
    //        if(row.at(0) == "ID") continue;

    //        TagTmpDBItem * pTagTmp = new TagTmpDBItem();
    //        int iRowCnt = m_pTableTagTmpObj->rowCount();
    //        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
    //        pTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
    //        //pTagTmp->m_szTagID = row.at(0);
    //        pTagTmp->m_szName = row.at(1);
    //        pTagTmp->m_szDescription = row.at(2);
    //        pTagTmp->m_szDataType = row.at(3);
    //        pTagTmp->m_szInitVal = row.at(4);
    //        pTagTmp->m_szMinVal = row.at(5);
    //        pTagTmp->m_szMaxVal = row.at(6);
    //        pTagTmp->m_szProjectConverter = row.at(7);

    //        int iRow = tableTagTmpAddRow();
    //        setTagTmpObjByRow(iRow, pTagTmp);

    //        tagTmp.listTagTmpDBItem_.append(pTagTmp);
    //    }
}

/**
 * @brief TagManagerChild::createTagTmp
 * @details 创建中间变量
 */
void TagManagerChild::createTagTmp()
{
    //    TagTmpEditDialog *pDlg = new TagTmpEditDialog(this);
    //    if(pDlg->exec() == QDialog::Accepted) {
    //        int num = pDlg->createTagNum();
    //        int iRowCnt = m_pTableTagTmpObj->rowCount();
    //        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
    //        TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    //        for(int i=0; i<num; i++) {
    //            TagTmpDBItem * pTagTmp = new TagTmpDBItem();
    //            pTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
    //            pTagTmp->m_szName = pDlg->tagName();
    //            pTagTmp->m_szDescription = pDlg->tagDescription();
    //            pTagTmp->m_szDataType = pDlg->tagDataType();
    //            pTagTmp->m_szInitVal = pDlg->tagInitValue();
    //            pTagTmp->m_szMinVal = pDlg->tagMinValue();
    //            pTagTmp->m_szMaxVal = pDlg->tagMaxValue();
    //            pTagTmp->m_szProjectConverter = "";

    //            int iRow = tableTagTmpAddRow();
    //            setTagTmpObjByRow(iRow, pTagTmp);

    //            tagTmp.listTagTmpDBItem_.append(pTagTmp);
    //            id++;
    //        }
    //    }
    //    delete pDlg;
}


/**
 * @brief TagManagerChild::appendTagTmp
 * @details 追加中间变量
 */
void TagManagerChild::appendTagTmp()
{
    //    int iRowCnt = m_pTableTagTmpObj->rowCount();
    //    if(iRowCnt < 1) return;

    //    int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
    //    TagTmpDBItem * pNewTagTmp = new TagTmpDBItem();
    //    pNewTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
    //    pNewTagTmp->m_szName = "";
    //    pNewTagTmp->m_szDescription = "";
    //    pNewTagTmp->m_szDataType = "";
    //    QTableWidgetItem *pItemDataType = m_pTableTagTmpObj->item(iRowCnt - 1, 3);
    //    if(pItemDataType != Q_NULLPTR) {
    //        pNewTagTmp->m_szDataType = pItemDataType->text();
    //    }
    //    pNewTagTmp->m_szInitVal = "0";
    //    pNewTagTmp->m_szMinVal = "";
    //    pNewTagTmp->m_szMaxVal = "";
    //    pNewTagTmp->m_szProjectConverter = "";
    //    int iRow = tableTagTmpAddRow();
    //    setTagTmpObjByRow(iRow, pNewTagTmp);
    //    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    //    tagTmp.listTagTmpDBItem_.append(pNewTagTmp);
}


/**
 * @brief TagManagerChild::copyCurTagTmp
 * @details 拷贝选中中间变量
 */
void TagManagerChild::copyCurTagTmp()
{
    int iSelectedRow = m_iTableTagTmpSelectedRow;
    if(iSelectedRow >= 0) {
        //        TagTmpDBItem * pNewTagTmp = getTagTmpObjByRow(iSelectedRow);
        //        int iRowCnt = m_pTableTagTmpObj->rowCount();
        //        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
        //        pNewTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
        //        int iRow = tableTagTmpAddRow();
        //        setTagTmpObjByRow(iRow, pNewTagTmp);
        //        TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
        //        tagTmp.listTagTmpDBItem_.append(pNewTagTmp);
    }
}


/**
 * @brief TagManagerChild::modifyCurTagTmp
 * @details 修改选中中间变量
 */
void TagManagerChild::modifyCurTagTmp()
{
    int iSelectedRow = m_iTableTagTmpSelectedRow;
    if(iSelectedRow >= 0) {
        //        TagTmpDBItem *pTagTmp = getTagTmpObjByRow(iSelectedRow);

        //        TagTmpEditDialog *pDlg = new TagTmpEditDialog(this);
        //        pDlg->setWindowTitle(tr("编辑中间变量"));
        //        pDlg->hideCreateNumUI();
        //        pDlg->setTagName(pTagTmp->m_szName);
        //        pDlg->setTagDescription(pTagTmp->m_szDescription);
        //        pDlg->setTagDataType(pTagTmp->m_szDataType);
        //        pDlg->setTagInitValue(pTagTmp->m_szInitVal);
        //        pDlg->setTagMinValue(pTagTmp->m_szMinVal);
        //        pDlg->setTagMaxValue(pTagTmp->m_szMaxVal);

        //        if(pDlg->exec() == QDialog::Accepted) {
        //            pTagTmp->m_szName = pDlg->tagName();
        //            pTagTmp->m_szDescription = pDlg->tagDescription();
        //            pTagTmp->m_szDataType = pDlg->tagDataType();
        //            pTagTmp->m_szInitVal = pDlg->tagInitValue();
        //            pTagTmp->m_szMinVal = pDlg->tagMinValue();
        //            pTagTmp->m_szMaxVal = pDlg->tagMaxValue();
        //            setTagTmpObjByRow(iSelectedRow, pTagTmp);
        //        }
        //        delete pDlg;
    }
}


/**
 * @brief TagManagerChild::deleteCurTagTmp
 * @details 删除选中中间变量
 */
void TagManagerChild::deleteCurTagTmp()
{
    //    m_listTagTmpDeleteRows.clear();
    //    QList<QTableWidgetItem *> pSelectItems = m_pTableTagTmpObj->selectedItems();
    //    QMap<int, int> rowMap;
    //    foreach (QTableWidgetItem *pItem, pSelectItems) {
    //        rowMap.insert(pItem->row(), 0);
    //    }
    //    int rowToDel;
    //    QMapIterator<int, int> rowMapIterator(rowMap);
    //    rowMapIterator.toBack();
    //    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    //    while (rowMapIterator.hasPrevious()) {
    //        rowMapIterator.previous();
    //        rowToDel = rowMapIterator.key();
    //        QTableWidgetItem *pItemID = m_pTableTagTmpObj->item(rowToDel, 0);
    //        if(pItemID != Q_NULLPTR) {
    //            m_listTagTmpDeleteRows << pItemID->text();
    //            TagTmpDBItem *pTagTmpObj = tagTmp.getTagTmpDBItemByID(pItemID->text());
    //            tagTmp.listTagTmpDBItem_.removeOne(pTagTmpObj);
    //        }
    //        m_pTableTagTmpObj->removeRow(rowToDel);
    //    }
}


void getRegTypeByDeviceName(const QString &deviceName,
                            QStringList &listReg,
                            QStringList &listType)
{
    QString pluginNmae = deviceName;
    if(pluginNmae.indexOf("_") >= 0)
        pluginNmae = pluginNmae.left(pluginNmae.indexOf("_"));

    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cdUp();
    pluginsDir.cd("deviceplugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
#ifdef Q_OS_WIN
        QString szFileEndWith = QString(".dll");
#endif

#ifdef Q_OS_LINUX
        QString szFileEndWith = QString(".so");
#endif
        if(fileName.indexOf(pluginNmae) != -1 && fileName.endsWith(szFileEndWith)) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDevicePlugin *iDevPlugin = qobject_cast<IDevicePlugin *>(plugin);
                if (iDevPlugin) {
                    // 获取设备支持的所有寄存器区
                    //listReg = iDevPlugin->GetDeviceSupportRegisterArea();
                    // 获取设备支持的所有数据类型
                    //listType = iDevPlugin->GetDeviceSupportDataType();
                } else {
                    QMessageBox::critical(Q_NULLPTR,
                                          QObject::tr("系统错误"),
                                          QObject::tr("插件加载失败！\n") + fileName);
                }
            }
        }
    }
}


/**
 * @brief TagManagerChild::tableTagIOAddRow
 * @details 设备变量表新增一行
 * @return 新增行号
 */
int TagManagerChild::tableTagIOAddRow()
{
    return 0;
    //    int iRowCount = m_pTableTagIOObj->rowCount();
    //    m_pTableTagIOObj->insertRow(iRowCount);

    //#define XX(item) \
    //    item->setFlags(item->flags() | Qt::ItemIsEditable);

    //    QTableWidgetItem *pItemID = new QTableWidgetItem("");
    //    m_pTableTagIOObj->setItem(iRowCount, 0, pItemID);
    //    QTableWidgetItem *pItemName = new QTableWidgetItem("var");
    //    XX(pItemName);
    //    m_pTableTagIOObj->setItem(iRowCount, 1, pItemName);
    //    QTableWidgetItem *pItemDescription = new QTableWidgetItem(tr("描述"));
    //    XX(pItemName);
    //    m_pTableTagIOObj->setItem(iRowCount, 2, pItemDescription);

    //    QTableWidgetItem *pItemDeviceName = new QTableWidgetItem("");
    //    XX(pItemDeviceName);
    //    m_pTableTagIOObj->setItem(iRowCount, 3, pItemDeviceName);

    //    QTableWidgetItem *pItemDeviceAddr = new QTableWidgetItem("1");
    //    XX(pItemDeviceAddr);
    //    m_pTableTagIOObj->setItem(iRowCount, 4, pItemDeviceAddr);

    //    QTableWidgetItem *pItemRegisterArea = new QTableWidgetItem("");
    //    XX(pItemRegisterArea);
    //    m_pTableTagIOObj->setItem(iRowCount, 5, pItemRegisterArea);

    //    QTableWidgetItem *pItemRegisterAddr = new QTableWidgetItem("0");
    //    XX(pItemRegisterAddr);
    //    m_pTableTagIOObj->setItem(iRowCount, 6, pItemRegisterAddr);

    //    QTableWidgetItem *pItemAddrOffset = new QTableWidgetItem("0");
    //    XX(pItemAddrOffset);
    //    m_pTableTagIOObj->setItem(iRowCount, 7, pItemAddrOffset);

    //    QTableWidgetItem *pItemReadWriteType = new QTableWidgetItem("");
    //    XX(pItemReadWriteType);
    //    m_pTableTagIOObj->setItem(iRowCount, 8, pItemReadWriteType);

    //    QTableWidgetItem *pItemDataType = new QTableWidgetItem("");
    //    XX(pItemDataType);
    //    m_pTableTagIOObj->setItem(iRowCount, 9, pItemDataType);

    //    QTableWidgetItem *pItemInitVal = new QTableWidgetItem("0");
    //    XX(pItemInitVal);
    //    m_pTableTagIOObj->setItem(iRowCount, 10, pItemInitVal);

    //    QTableWidgetItem *pItemMinVal = new QTableWidgetItem("0");
    //    XX(pItemMinVal);
    //    m_pTableTagIOObj->setItem(iRowCount, 11, pItemMinVal);
    //    QTableWidgetItem *pItemMaxVal = new QTableWidgetItem("0");
    //    XX(pItemMaxVal);
    //    m_pTableTagIOObj->setItem(iRowCount, 12, pItemMaxVal);
    //    QTableWidgetItem *pItemScale = new QTableWidgetItem("1");
    //    XX(pItemScale);
    //    m_pTableTagIOObj->setItem(iRowCount, 13, pItemScale);
    //    QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem("");
    //    m_pTableTagIOObj->setItem(iRowCount, 14, pItemProjectConverter);

    //#undef XX
    //    return iRowCount;
}


/**
 * @brief TagManagerChild::getTagIOIdNumValue
 * @details 获取指定行设备变量id数值部分
 * @param iRow 行号
 * @return 设备变量id数值部分
 */
int TagManagerChild::getTagIOIdNumValue(int iRow)
{
    //    int iRowCnt = m_pTableTagIOObj->rowCount();
    int id = 0;
    //    if(iRowCnt > 0) {
    //        int iPos = -1;
    //        QString szVarTmp = "";
    //        QString szTmp = "0";
    //        QTableWidgetItem *pItemID = m_pTableTagIOObj->item(iRow, 0);
    //        if(pItemID != Q_NULLPTR) {
    //            szVarTmp = pItemID->text();
    //        }
    //        if(szVarTmp.startsWith("io.")) {
    //            iPos = szVarTmp.lastIndexOf(".");
    //            if(iPos >= 0) {
    //                szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
    //                id = szTmp.toInt();
    //            }
    //        }
    //    }
    return id;
}

/**
 * @brief TagManagerChild::updateTableTagIO
 * @details 刷新设备变量表
 */
void TagManagerChild::updateTableTagIO()
{
    //    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    //    if(m_szCurIOGroupName == QString()) return;
    //    m_pTableTagIOObj->clearContents();
    //    m_pTableTagIOObj->setRowCount(0);
    //    foreach (TagIODBItem * itemTagIO, tagIO.listTagIODBItem_) {
    //        if(m_szCurIOGroupName != itemTagIO->m_szGroupName) continue;
    //        int iRow = tableTagIOAddRow();
    //        setTagIOObjByRow(iRow, itemTagIO);
    //    }
}


/**
 * @brief TagManagerChild::tagIOExportToCsv
 * @details 设备变量表变量导出至CSV文件
 * @param path
 */
void TagManagerChild::tagIOExportToCsv(const QString &path, const QString &group)
{
    QtCSV::StringData varData;
    int iRowCount = 0;//m_pTableTagIOObj->rowCount();
    for(int i=0; i<iRowCount; i++) {
        //        TagIODBItem * pTagIO = getTagIOObjByRow(i);

        //        QStringList varRow;
        //        varRow << pTagIO->m_szTagID << pTagIO->m_szGroupName << pTagIO->m_szName<< pTagIO->m_szDescription
        //               << pTagIO->m_szDeviceName << pTagIO->m_szDeviceAddr << pTagIO->m_szRegisterArea << pTagIO->m_szRegisterAddr
        //               << pTagIO->m_szAddrOffset << pTagIO->m_szReadWriteType << pTagIO->m_szDataType << pTagIO->m_szInitVal
        //               << pTagIO->m_szMinVal << pTagIO->m_szMaxVal << pTagIO->m_szScale << pTagIO->m_szProjectConverter;

        //        varData.addRow(varRow);
    }

    QString filepath = path + "/" + group + ".csv";
    QStringList header;
    header << tr("ID") << tr("变量组名称") << tr("名称") << tr("变量描述") << tr("设备名")
           << tr("设备地址") << tr("寄存器区") << tr("寄存器地址") << tr("偏移地址")
           << tr("读写类型") << tr("数据类型") << tr("初始值") << tr("最小值")
           << tr("最大值") << tr("变比") << tr("工程转换");
    QtCSV::Writer::write(filepath, varData, QString(","), QString("\""),
                         QtCSV::Writer::REWRITE, header,  QStringList(),
                         QTextCodec::codecForName("GB18030"));
}

/**
 * @brief TagManagerChild::tagIOImportFromCsv
 * @details 从CSV文件导入设备变量至设备变量表
 * @param path
 */
void TagManagerChild::tagIOImportFromCsv(const QString &path)
{
    QString filepath = path;

    QList<QStringList> data = QtCSV::Reader::readToList(filepath,
                                                        QString(","),
                                                        QString("\""),
                                                        QTextCodec::codecForName("GB18030"));
    //    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    //    for(int i=0; i<data.size(); i++) {
    //        QStringList row = data.at(i);
    //        if(row.at(0) == "ID") continue;

    //        TagIODBItem * pTagIO = new TagIODBItem();
    //        int iRowCnt = m_pTableTagIOObj->rowCount();
    //        int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
    //        (void)id;
    //        //pTagIO->m_szTagID = QString("tmp.%1").arg(QString::number(id));

    //        // TODO

    //        //pTagIO->m_szTagID = row.at(0);
    //        pTagIO->m_szGroupName = row.at(1);
    //        pTagIO->m_szName = row.at(2);
    //        pTagIO->m_szDescription = row.at(3);
    //        pTagIO->m_szDeviceName = row.at(4);
    //        pTagIO->m_szDeviceAddr = row.at(5);
    //        pTagIO->m_szRegisterArea = row.at(6);
    //        pTagIO->m_szRegisterAddr = row.at(7);
    //        pTagIO->m_szAddrOffset = row.at(9);
    //        pTagIO->m_szReadWriteType = row.at(10);
    //        pTagIO->m_szDataType = row.at(11);
    //        pTagIO->m_szInitVal = row.at(12);
    //        pTagIO->m_szMinVal = row.at(13);
    //        pTagIO->m_szMaxVal = row.at(14);
    //        pTagIO->m_szScale = row.at(15);
    //        pTagIO->m_szProjectConverter = row.at(16);

    //        int iRow = tableTagIOAddRow();
    //        setTagIOObjByRow(iRow, pTagIO);
    //        tagIO.listTagIODBItem_.append(pTagIO);
    //    }
}

/**
 * @brief TagManagerChild::createTagIO
 * @details 创建设备变量
 */
void TagManagerChild::createTagIO()
{
    //    if(m_szCurIOGroupName == QString()) return;

    //    // 判断是否已经建立通讯设备
    //    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    //    if(deviceInfo.listDeviceInfoObject_.count() < 1) {
    //        QMessageBox::warning(this,
    //                             tr("提示"),
    //                             QString(tr("不存在通讯设备，请先新建通讯设备！")));
    //        return;
    //    }

    //    TagIOEditDialog *pDlg = new TagIOEditDialog(m_szProjectName, this);
    //    if(pDlg->exec() == QDialog::Accepted) {
    //        int num = pDlg->createTagNum();
    //        int iOffset = pDlg->addrOffset();
    //        int iRowCnt = m_pTableTagIOObj->rowCount();
    //        int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
    //        TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    //        for(int i=0; i<num; i++) {
    //            TagIODBItem * pTagIO = new TagIODBItem();
    //            pTagIO->m_szTagID = QString("io.%1.%2").arg(m_szCurIOGroupName).arg(QString::number(id));
    //            pTagIO->m_szGroupName = m_szCurIOGroupName;
    //            pTagIO->m_szName = pDlg->tagName();
    //            pTagIO->m_szDescription = pDlg->tagDesc();
    //            pTagIO->m_szDeviceName = pDlg->deviceName();
    //            pTagIO->m_szDeviceAddr = pDlg->deviceAddr();
    //            pTagIO->m_szRegisterArea = pDlg->regSection();
    //            pTagIO->m_szRegisterAddr = QString::number(pDlg->regAddr().toInt() + iOffset * i);
    //            pTagIO->m_szAddrOffset = pDlg->regAddrOffset();
    //            pTagIO->m_szReadWriteType = pDlg->typeReadWrite();
    //            pTagIO->m_szDataType = pDlg->dataType();
    //            pTagIO->m_szInitVal  = pDlg->tagInitValue();
    //            pTagIO->m_szMinVal  = pDlg->tagMinValue();
    //            pTagIO->m_szMaxVal  = pDlg->tagMaxValue();
    //            pTagIO->m_szScale  = pDlg->scale();
    //            pTagIO->m_szProjectConverter = "";

    //            int iRow = tableTagIOAddRow();
    //            setTagIOObjByRow(iRow, pTagIO);

    //            tagIO.listTagIODBItem_.append(pTagIO);
    //            id++;
    //        }
    //    }
    //    delete pDlg;
}

/**
 * @brief TagManagerChild::appendTagIO
 * @details 追加设备变量
 */
void TagManagerChild::appendTagIO()
{
    //    int iRowCnt = m_pTableTagIOObj->rowCount();
    //    if(iRowCnt < 1)
    //        return;

    //    if(m_szCurIOGroupName == QString()) return;

    //    int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
    //    TagIODBItem * pNewTagIO = new TagIODBItem();
    //    pNewTagIO->m_szTagID = QString("io.%1.%2").arg(m_szCurIOGroupName).arg(QString::number(id));
    //    pNewTagIO->m_szGroupName = m_szCurIOGroupName;
    //    pNewTagIO->m_szName = "";
    //    pNewTagIO->m_szDescription = "";
    //    pNewTagIO->m_szDeviceName = "";
    //    QTableWidgetItem *pItemDeviceName = m_pTableTagIOObj->item(iRowCnt - 1, 3);
    //    if(pItemDeviceName != Q_NULLPTR) {
    //        pNewTagIO->m_szDeviceName = pItemDeviceName->text();
    //    }
    //    pNewTagIO->m_szDeviceAddr = "1";
    //    pNewTagIO->m_szRegisterArea = "";
    //    QTableWidgetItem *pItemRegisterArea = m_pTableTagIOObj->item(iRowCnt - 1, 5);
    //    if(pItemRegisterArea != Q_NULLPTR) {
    //        pNewTagIO->m_szRegisterArea = pItemRegisterArea->text();
    //    }
    //    pNewTagIO->m_szRegisterAddr = "0";
    //    pNewTagIO->m_szAddrOffset = "0";
    //    pNewTagIO->m_szReadWriteType = "";
    //    QTableWidgetItem *pItemReadWriteType = m_pTableTagIOObj->item(iRowCnt - 1, 8);
    //    if(pItemReadWriteType != Q_NULLPTR) {
    //        pNewTagIO->m_szReadWriteType = pItemReadWriteType->text();
    //    }
    //    pNewTagIO->m_szDataType  = "";
    //    QTableWidgetItem *pItemDataType = m_pTableTagIOObj->item(iRowCnt - 1, 9);
    //    if(pItemDataType != Q_NULLPTR) {
    //        pNewTagIO->m_szDataType = pItemDataType->text();
    //    }
    //    pNewTagIO->m_szInitVal  = "0";
    //    pNewTagIO->m_szMinVal  = "";
    //    pNewTagIO->m_szMaxVal  = "";
    //    pNewTagIO->m_szScale  = "1";
    //    pNewTagIO->m_szProjectConverter  = "";

    //    int iRow = tableTagIOAddRow();
    //    setTagIOObjByRow(iRow, pNewTagIO);

    //    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    //    tagIO.listTagIODBItem_.append(pNewTagIO);
}


/**
 * @brief TagManagerChild::copyCurTagIO
 * @details 拷贝选中设备变量
 */
void TagManagerChild::copyCurTagIO()
{
    if(m_szCurIOGroupName == QString()) return;

    //    int iSelectedRow = m_iTableTagIOSelectedRow;
    //    if(iSelectedRow >= 0) {
    //        TagIODBItem * pNewTagIO = getTagIOObjByRow(iSelectedRow);
    //        int iRowCnt = m_pTableTagIOObj->rowCount();
    //        int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
    //        pNewTagIO->m_szTagID = QString("io.%1.%2").arg(m_szCurIOGroupName).arg(QString::number(id));
    //        int iRow = tableTagIOAddRow();
    //        setTagIOObjByRow(iRow, pNewTagIO);
    //        TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    //        tagIO.listTagIODBItem_.append(pNewTagIO);
    //    }
}


/**
 * @brief TagManagerChild::modifyCurTagIO
 * @details 修改选中设备变量
 */
void TagManagerChild::modifyCurTagIO()
{
    //    int iSelectedRow = m_iTableTagIOSelectedRow;
    //    if(iSelectedRow >= 0) {
    //        TagIODBItem *pTagIO = getTagIOObjByRow(iSelectedRow);

    //        TagIOEditDialog *pDlg = new TagIOEditDialog(m_szProjectName, this);
    //        pDlg->setWindowTitle(tr("编辑设备变量"));
    //        pDlg->hideCreateNumUI();
    //        pDlg->setTagName(pTagIO->m_szName);
    //        pDlg->setTagDesc(pTagIO->m_szDescription);
    //        pDlg->setDeviceName(pTagIO->m_szDeviceName);
    //        pDlg->setDeviceAddr(pTagIO->m_szDeviceAddr);
    //        pDlg->setRegSection(pTagIO->m_szRegisterArea);
    //        pDlg->setRegAddr(pTagIO->m_szRegisterAddr);
    //        pDlg->setRegAddrOffset(pTagIO->m_szAddrOffset);
    //        pDlg->setTypeReadWrite(pTagIO->m_szReadWriteType);
    //        pDlg->setDataType(pTagIO->m_szDataType);
    //        pDlg->setTagInitValue(pTagIO->m_szInitVal);
    //        pDlg->setTagMinValue(pTagIO->m_szMinVal);
    //        pDlg->setTagMaxValue(pTagIO->m_szMaxVal);
    //        pDlg->setScale(pTagIO->m_szScale);
    //        if(pDlg->exec() == QDialog::Accepted) {
    //            pTagIO->m_szName = pDlg->tagName();
    //            pTagIO->m_szDescription = pDlg->tagDesc();
    //            pTagIO->m_szDeviceName = pDlg->deviceName();
    //            pTagIO->m_szDeviceAddr = pDlg->deviceAddr();
    //            pTagIO->m_szRegisterArea = pDlg->regSection();
    //            pTagIO->m_szRegisterAddr = pDlg->regAddr();
    //            pTagIO->m_szAddrOffset = pDlg->regAddrOffset();
    //            pTagIO->m_szReadWriteType = pDlg->typeReadWrite();
    //            pTagIO->m_szDataType = pDlg->dataType();
    //            pTagIO->m_szInitVal  = pDlg->tagInitValue();
    //            pTagIO->m_szMinVal  = pDlg->tagMinValue();
    //            pTagIO->m_szMaxVal  = pDlg->tagMaxValue();
    //            pTagIO->m_szScale  = pDlg->scale();
    //            setTagIOObjByRow(iSelectedRow, pTagIO);
    //        }
    //        delete pDlg;
    //    }
}


/**
 * @brief TagManagerChild::deleteCurTagIO
 * @details 删除选中设备变量
 */
void TagManagerChild::deleteCurTagIO()
{
    //    m_listTagIODeleteRows.clear();
    //    QList<QTableWidgetItem *> pSelectItems = m_pTableTagIOObj->selectedItems();
    //    QMap<int, int> rowMap;
    //    foreach (QTableWidgetItem *pItem, pSelectItems) {
    //        rowMap.insert(pItem->row(), 0);
    //    }
    //    int rowToDel;
    //    QMapIterator<int, int> rowMapIterator(rowMap);
    //    rowMapIterator.toBack();
    //    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    //    while (rowMapIterator.hasPrevious()) {
    //        rowMapIterator.previous();
    //        rowToDel = rowMapIterator.key();
    //        QTableWidgetItem *pItemID = m_pTableTagIOObj->item(rowToDel, 0);
    //        if(pItemID != Q_NULLPTR) {
    //            m_listTagIODeleteRows << pItemID->text();
    //            TagIODBItem *pTagIOObj = tagIO.getTagIODBItemByID(pItemID->text());
    //            tagIO.listTagIODBItem_.removeOne(pTagIOObj);
    //        }
    //        m_pTableTagIOObj->removeRow(rowToDel);
    //    }
}



/**
    * @brief TagManagerChild::onSlotExportTag
    * @details 导出变量
    */
void TagManagerChild::onSlotExportTag()
{
    QString szDirPath = QCoreApplication::applicationDirPath();
    QString szSaveCsvPath = QFileDialog::getExistingDirectory(
                this, tr("选择导出csv路径"), szDirPath,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (szSaveCsvPath == "") return;
    this->exportToCsv(szSaveCsvPath);
}


/**
    * @brief TagManagerChild::onSlotImportTag
    * @details 导入变量
    */
void TagManagerChild::onSlotImportTag()
{
    //    QString szDirPath = QCoreApplication::applicationDirPath();
    //    QString szSaveCsvFile = QFileDialog::getOpenFileName(this, tr("选择csv文件"),
    //                                                          szDirPath,
    //                                                          tr("csv file (*.csv)"));
    //    if(szSaveCsvFile == "") return;

    //    this->importFromCsv(szSaveCsvFile);

    //    QString szCsvName= szSaveCsvFile.mid(szSaveCsvFile.lastIndexOf("/") + 1, szSaveCsvFile.indexOf(".") - szSaveCsvFile.lastIndexOf("/") - 1);
    //    QString szGroupName = szCsvName.right(szCsvName.length() - szCsvName.lastIndexOf("-") - 1);

    //    if(szCsvName.startsWith(tr("中间变量"))) {

    //    } else if(szCsvName.startsWith(tr("设备变量"))) {
    //        bool bFound = false;

    //        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    //        foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
    //            if(szGroupName == pObj->m_szShowName) {
    //                bFound = true;
    //                break;
    //            }
    //        }

    //        if(!bFound) {
    //            TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    //            TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
    //            pObj->m_id = tagIOGroup.getGroupCount() + 1;
    //            pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
    //            pObj->m_szShowName = szGroupName;
    //            tagIOGroup.listTagIOGroupDBItem_.append(pObj);
    //            if(m_pMainWinObj) {
    //                MainWindow *pWndObj = dynamic_cast<MainWindow *>(m_pMainWinObj);
    //                if(pWndObj) {
    //                    pWndObj->UpdateDeviceVariableTableGroup();
    //                    pWndObj->onSlotTreeProjectViewClicked(tr("设备变量"));
    //                    QApplication::processEvents();
    //                    pWndObj->onSlotTreeProjectViewClicked(szGroupName);
    //                    QApplication::processEvents();
    //                }
    //            }
    //        }
    //    }
}


void TagManagerChild::buildUserInterface(QMainWindow* pMainWin)
{
    if(pMainWin == Q_NULLPTR) return;

    if(wndTitle().startsWith(tr("设备变量"))|| wndTitle().startsWith(tr("中间变量"))) {
        if(m_pMainWinObj == Q_NULLPTR) {

            //-----------------------------<变量编辑菜单>----------------------------------
            // 添加变量
            m_pActAddTagObj = new QAction(QIcon(":/images/data_add.png"), tr("添加变量"));
            connect(m_pActAddTagObj, SIGNAL(triggered()), SLOT(onSlotAddTag()));

            // 追加变量
            m_pActAppendTagObj = new QAction(QIcon(":/images/data_supadd.png"), tr("追加变量"));
            connect(m_pActAppendTagObj, SIGNAL(triggered()), SLOT(onSlotAppendTag()));

            // 拷贝变量
            m_pActRowCopyTagObj = new QAction(QIcon(":/images/data_rowcopy.png"), tr("拷贝变量"));
            connect(m_pActRowCopyTagObj, SIGNAL(triggered()), SLOT(onSlotRowCopyTag()));

            // 修改变量
            m_pActModifyTagObj = new QAction(QIcon(":/images/icon_modify.png"), tr("修改变量"));
            connect(m_pActModifyTagObj, SIGNAL(triggered()), SLOT(onSlotModifyTag()));

            // 删除变量
            m_pActDeleteTagObj = new QAction(QIcon(":/images/icon_delete.png"), tr("删除变量"));
            connect(m_pActDeleteTagObj, SIGNAL(triggered()), SLOT(onSlotDeleteTag()));

            // 导出变量
            m_pActExportTagObj = new QAction(QIcon(":/images/data_export.png"), tr("导出变量"));
            connect(m_pActExportTagObj, SIGNAL(triggered()), SLOT(onSlotExportTag()));

            // 导入变量
            m_pActImportTagObj = new QAction(QIcon(":/images/data_import.png"), tr("导入变量"));
            connect(m_pActImportTagObj, SIGNAL(triggered()), SLOT(onSlotImportTag()));


            //-----------------------------<变量编辑菜单>----------------------------------
            m_pMenuTagEditObj = pMainWin->menuBar()->addMenu(tr("变量编辑"));
            m_pMenuTagEditObj->addAction(m_pActAddTagObj); // 添加变量
            m_pMenuTagEditObj->addAction(m_pActAppendTagObj); // 追加变量
            m_pMenuTagEditObj->addAction(m_pActRowCopyTagObj); // 拷贝变量
            m_pMenuTagEditObj->addAction(m_pActModifyTagObj); // 修改变量
            m_pMenuTagEditObj->addAction(m_pActDeleteTagObj); // 删除变量
            m_pMenuTagEditObj->addAction(m_pActExportTagObj); // 导出变量
            m_pMenuTagEditObj->addAction(m_pActImportTagObj); // 导入变量

            //-----------------------------<变量编辑>----------------------------------
            m_pToolBarTagEditObj = new QToolBar(pMainWin);
            m_pToolBarTagEditObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            m_pToolBarTagEditObj->addAction(m_pActAddTagObj);  // 添加变量
            m_pToolBarTagEditObj->addAction(m_pActAppendTagObj); // 追加变量
            m_pToolBarTagEditObj->addAction(m_pActRowCopyTagObj); // 拷贝变量
            m_pToolBarTagEditObj->addAction(m_pActModifyTagObj); // 修改变量
            m_pToolBarTagEditObj->addAction(m_pActDeleteTagObj); // 删除变量
            m_pToolBarTagEditObj->addAction(m_pActExportTagObj); // 导出变量
            m_pToolBarTagEditObj->addAction(m_pActImportTagObj); // 导入变量

            pMainWin->addToolBar(Qt::TopToolBarArea, m_pToolBarTagEditObj);

            m_pMainWinObj = pMainWin;
        }
    } else {
        m_pMainWinObj = Q_NULLPTR;
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
        DEL_OBJ(m_pActAppendTagObj);
        DEL_OBJ(m_pActRowCopyTagObj);
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





