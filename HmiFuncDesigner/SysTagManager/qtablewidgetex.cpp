#include "qtablewidgetex.h"
#include "Tag.h"
#include "TagEditDialog.h"
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QInputDialog>
#include <QScrollBar>
#include <QTime>
#include <QMessageBox>
#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

////////////////////////////////////////////////////////////////////

const int MaxTagIOTableColumns = TagTableModel::MaxColumns;


Qt::ItemFlags TagTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable |/*Qt::ItemIsEditable|*/Qt::ItemIsEnabled;
    }
    return theFlags;
}


QVariant TagTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
            index.row() < 0 || index.row() >= m_tagRows.count() ||
            index.column() < 0 || index.column() >= MaxTagIOTableColumns) {
        return QVariant();
    }
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


QVariant TagTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Horizontal) {
        switch (section) {
            case TagID:
                return tr("变量ID");
            case Name:
                return tr("变量名称");
            case DataType:
                return tr("数据类型");
            case ReadWrite:
                return tr("读写");
            case Unit:
                return tr("单位");
            case Remark:
                return tr("变量描述");
            default:
                Q_ASSERT(false);
        }
    }
    return section + 1;
}


int TagTableModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_tagRows.count();
}


int TagTableModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : MaxTagIOTableColumns;
}


bool TagTableModel::setData(const QModelIndex &index,
                            const QVariant &value,
                            int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
            index.row() < 0 || index.row() >= m_tagRows.count() ||
            index.column() < 0 || index.column() >= MaxTagIOTableColumns) {
        return false;
    }

    QStringList &rowDat = m_tagRows[index.row()];
    rowDat[index.column()] = value.toString();
    emit dataChanged(index, index);
    return true;
}


bool TagTableModel::insertRows(int row, int count, const QModelIndex&)
{
    if(count < 1) {
        return false;
    }
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_tagRows.insert(row, QStringList());
    }
    endInsertRows();
    return true;
}


bool TagTableModel::removeRows(int row, int count, const QModelIndex&)
{
    if(count < 1) {
        return false;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_tagRows.removeAt(row);
    }
    endRemoveRows();
    return true;
}


void TagTableModel::AppendRow(QStringList rowDat)
{
    int iRow = m_tagRows.size();
    beginInsertRows(QModelIndex(), iRow, iRow);
    QStringList it = rowDat;
    m_tagRows.append(it);
    endInsertRows();
}

void TagTableModel::AppendRows(QVector<QStringList> rowsDat)
{
    if(rowsDat.size() < 1) {
        return;
    }
    int iRow = m_tagRows.size();
    beginInsertRows(QModelIndex(), iRow, iRow + rowsDat.size() - 1);
    for(int i = 0; i < rowsDat.size(); i++) {
        QStringList row = rowsDat.at(i);
        m_tagRows.append(row);
    }
    endInsertRows();
}


void TagTableModel::InsertRow(int i, QStringList rowDat)
{
    beginInsertRows(QModelIndex(), i, i);
    QStringList it = rowDat;
    m_tagRows.insert(i, it);
    endInsertRows();
}


QStringList TagTableModel::GetRow(int i)
{
    QStringList it;
    if(i < m_tagRows.size()) {
        return m_tagRows.at(i);
    }
    return it;
}


void TagTableModel::UpdateRow(int i, QStringList rowDat)
{
    if(i < m_tagRows.size()) {
        m_tagRows.replace(i, rowDat);
        for(int c = 0; c < MaxTagIOTableColumns; c++) {
            emit dataChanged(this->index(i, c), this->index(i, c));
        }
    }
}

void TagTableModel::UpdateRows(QVector<QStringList> rowsDat)
{
    m_tagRows = rowsDat;
    emit dataChanged(this->index(0, 0), this->index(m_tagRows.size() - 1, MaxTagIOTableColumns - 1));
}


////////////////////////////////////////////////////////////////////



QTableWidgetEx::QTableWidgetEx(QWidget *parent) : QTableView(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
    m_pTagTableModel = new TagTableModel();
    // 初始化变量表控件
    initTagsTable();
    connect(this, &QAbstractItemView::doubleClicked, this, &QTableWidgetEx::onDoubleClicked);
}

QTableWidgetEx::~QTableWidgetEx()
{
    if(m_pTagTableModel != NULL) {
        delete m_pTagTableModel;
        m_pTagTableModel = NULL;
    }
}

///
/// \brief QTableWidgetEx::initTagsTable
/// \details 初始化变量表控件
///
void QTableWidgetEx::initTagsTable()
{
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
    this->setColumnWidth(2, 100); // 数据类型
    this->setColumnWidth(3, 100); // 读写
    this->setColumnWidth(4, 100); // 单位
    this->setColumnWidth(5, 160); // 变量描述
    this->setWordWrap(false);
    this->clearSelection();

    // 根据程序实例字体大小调整列宽
    QFontMetrics fm(qApp->font());
    for(int col = 0; col < TagTableModel::MaxColumns; col++) {
        QVariant headerData = m_pTagTableModel->headerData(col, Qt::Horizontal);
        int iWidth = fm.width(headerData.toString()) + 2 * fm.averageCharWidth();
        if(iWidth > this->columnWidth(col)) {
            this->setColumnWidth(col, iWidth);
        }
    }
}

///
/// \brief QTableWidgetEx::clearTable
/// \details 清空变量表
///
void QTableWidgetEx::clearTable()
{
    m_pTagTableModel->removeRows(0, m_pTagTableModel->rowCount());
}

///
/// \brief QTableWidgetEx::updateTable
/// \details 刷新变量表
///
void QTableWidgetEx::updateTable()
{
    clearTable();
    QVector<QStringList> rowsDat;
    foreach (Tag *pTagObj, m_tagMgr.m_vecTags) {
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
void QTableWidgetEx::setRowData(QStringList &rowDat, Tag *pObj)
{
    rowDat.clear();
    rowDat << QString::number(pObj->m_id); // 变量ID
    rowDat << QString("$%1").arg(pObj->m_name); // 变量名称
    rowDat << pObj->m_dataType; // 数据类型

    QString szWriteable = "只读";
    if (pObj->m_writeable == 0) {
        szWriteable = tr("只读");
    } else if (pObj->m_writeable == 1) {
        szWriteable = tr("可读可写");
    }
    rowDat << szWriteable; // 读写

    rowDat << pObj->m_unit; // 单位
    rowDat << pObj->m_remark; // 变量描述
}


///
/// \brief QTableWidgetEx::onDoubleClicked
/// \details 单元格被双击
/// \param row 单元格所在行
/// \param column 单元格所在列
///
void QTableWidgetEx::onDoubleClicked(const QModelIndex &index)
{
    if(index.row() < 0 || index.row() > m_pTagTableModel->m_tagRows.size()) {
        return;
    }

    QStringList rowDat = m_pTagTableModel->m_tagRows.at(index.row());
    int iTagID = rowDat.at(0).toInt();
    for(int i = 0; i < m_tagMgr.m_vecTags.count(); i++) {
        Tag *pTagObj = m_tagMgr.m_vecTags[i];
        if(pTagObj->m_id == iTagID) {
            TagEditDialog dlg(this);
            dlg.setWindowTitle(tr("编辑变量"));
            QMap<QString, QStringList> mapDevToAddrType;
            QMap<QString, QString> mapAddrTypeToAddrTypeAlias;
            QMap<QString, QStringList> mapAddrTypeToSubAddrType;
            QMap<QString, QStringList> mapAddrTypeToDataType;
            QStringList szListAddrType;
            szListAddrType << tr("自动分配");
            mapDevToAddrType.insert("SYSTEM", szListAddrType);
            mapAddrTypeToAddrTypeAlias.insert(tr("自动分配"), "");
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
            dlg.setAddrTypeAndDataType(mapDevToAddrType, mapAddrTypeToAddrTypeAlias, mapAddrTypeToSubAddrType, mapAddrTypeToDataType);
            QJsonObject jsonTagObj = pTagObj->toJsonObject();
            dlg.setTagObj(jsonTagObj);
            dlg.updateUI();
            if(dlg.exec() == QDialog::Accepted) {
                jsonTagObj = dlg.getTagObj();
                pTagObj->fromJsonObject(jsonTagObj);
                if(pTagObj->m_addrType == tr("自动分配")) {
                    pTagObj->m_addrType = "AutoAlloc";
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
void QTableWidgetEx::contextMenuEvent(QContextMenuEvent * event)
{
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
        if(tagIDMap.size() < 2) {
            pMenu->addAction(pEditTagAct);
        }
    } else { // 表格行下空白部分
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
void QTableWidgetEx::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
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
void QTableWidgetEx::onAddTag()
{
    TagEditDialog dlg(this);
    dlg.setWindowTitle(tr("新建变量"));
    QMap<QString, QStringList> mapDevToAddrType;
    QMap<QString, QString> mapAddrTypeToAddrTypeAlias;
    QMap<QString, QStringList> mapAddrTypeToSubAddrType;
    QMap<QString, QStringList> mapAddrTypeToDataType;
    QStringList szListAddrType;
    szListAddrType << tr("自动分配");
    mapDevToAddrType.insert("SYSTEM", szListAddrType);
    mapAddrTypeToSubAddrType.insert(tr("自动分配"), QStringList());
    mapAddrTypeToAddrTypeAlias.insert(tr("自动分配"), "");
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
    dlg.setAddrTypeAndDataType(mapDevToAddrType, mapAddrTypeToAddrTypeAlias, mapAddrTypeToSubAddrType, mapAddrTypeToDataType);
    QJsonObject jsonTagObj;
    dlg.setTagObj(jsonTagObj);
    dlg.updateUI();
    if(dlg.exec() == QDialog::Accepted) {
        Tag *pTagObj = new Tag();
        pTagObj->fromJsonObject(dlg.getTagObj());
        pTagObj->m_id = m_tagMgr.allocID();
        if(pTagObj->m_addrType == tr("自动分配")) {
            pTagObj->m_addrType = "AutoAlloc";
        }
        pTagObj->m_devType = "SYSTEM";
        m_tagMgr.m_vecTags.append(pTagObj);
        updateTable();
    }
}

///
/// \brief QTableWidgetEx::onCopyTag
/// \details 复制变量
///
void QTableWidgetEx::onCopyTag()
{
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
        foreach (Tag *pTagObj, m_tagMgr.m_vecTags) {
            if(pTagObj->m_id == iTagID) {
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
        pTagObj->m_id = m_tagMgr.allocID();
        m_tagMgr.m_vecTags.append(pTagObj);
        updateTable();
    } else {
        delete pTagObj;
        pTagObj = NULL;
    }
}


///
/// \brief QTableWidgetEx::onEditTag
/// \details 删除变量
///
void QTableWidgetEx::onDeleteTag()
{
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
        Tag *pFindTagObj = NULL;
        foreach (Tag *pTagObj, m_tagMgr.m_vecTags) {
            if(pTagObj->m_id == iIDToDel) {
                pFindTagObj = pTagObj;
                break;
            }
        }
        if(pFindTagObj != NULL) {
            m_tagMgr.m_vecTags.removeOne(pFindTagObj);
            bUpdate = true;
        }
    }

    if(bUpdate) {
        updateTable();
    }
}


///
/// \brief QTableWidgetEx::onEditTag
/// \details 编辑变量属性
///
void QTableWidgetEx::onEditTag()
{
    int iRow = this->currentIndex().row();
    QStringList rowDat = m_pTagTableModel->m_tagRows.at(iRow);
    int iTagID = rowDat.at(0).toInt();
    foreach (Tag *pTagObj, m_tagMgr.m_vecTags) {
        if(pTagObj->m_id == iTagID) {
            TagEditDialog dlg(this);
            QMap<QString, QStringList> mapDevToAddrType;
            QMap<QString, QString> mapAddrTypeToAddrTypeAlias;
            QMap<QString, QStringList> mapAddrTypeToSubAddrType;
            QMap<QString, QStringList> mapAddrTypeToDataType;
            QStringList szListAddrType;
            szListAddrType << tr("自动分配");
            mapDevToAddrType.insert("SYSTEM", szListAddrType);
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
            mapAddrTypeToAddrTypeAlias.insert(tr("自动分配"), "");
            dlg.setAddrTypeAndDataType(mapDevToAddrType, mapAddrTypeToAddrTypeAlias, mapAddrTypeToSubAddrType, mapAddrTypeToDataType);
            dlg.setWindowTitle(tr("编辑变量"));
            QJsonObject jsonTagObj = pTagObj->toJsonObject();
            dlg.setTagObj(jsonTagObj);
            dlg.updateUI();
            if(dlg.exec() == QDialog::Accepted) {
                pTagObj->fromJsonObject(dlg.getTagObj());
                if(pTagObj->m_addrType == tr("自动分配")) {
                    pTagObj->m_addrType = "AutoAlloc";
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
void QTableWidgetEx::setActionEnable(TagTableActonType id, bool enable)
{
    QAction* pActObj = m_mapIdToAction[id];
    if(pActObj != NULL) {
        pActObj->setEnabled(enable);
    }
}





