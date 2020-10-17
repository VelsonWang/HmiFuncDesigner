#include "TagManagerChild.h"
#include "NewComDeviceDialog.h"
#include "TagIOEditDialog.h"
#include "TagTmpEditDialog.h"
#include "stringdata.h"
#include "writer.h"
#include "reader.h"
#include "variantdata.h"
#include "Helper.h"
#include "ProjectData.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QPainter>
#include <QModelIndex>
#include <QComboBox>
#include <QString>
#include <QCloseEvent>
#include <QStringList>
#include <QHeaderView>
#include <QTableView>
#include <QModelIndex>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include "ProjectData.h"
#include "DBTag.h"
#include "TagSys.h"
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
#include <QDebug>

TagManagerChild::TagManagerChild(QWidget *parent) : QWidget(parent)
{
    initUi();

    setContextMenuPolicy(Qt::DefaultContextMenu);
    // 初始化系统变量表
    initialTableTagSys();
    // 初始化中间变量表
    initialTableTagTmp();
    // 初始化设备变量表
    initialTableTagIO();

}

TagManagerChild::~TagManagerChild()
{
    DEL_OBJ(m_pTableTagIOObj);
    DEL_OBJ(m_pTableTagTmpObj);
    DEL_OBJ(m_pTableTagSysObj);
    DEL_OBJ(m_pStackedWidgetObj);
    DEL_OBJ(m_pTopVLayoutObj);
}

void TagManagerChild::initUi()
{
    m_pTopVLayoutObj = new QVBoxLayout(this);
    m_pTopVLayoutObj->setSpacing(1);
    m_pTopVLayoutObj->setContentsMargins(1, 1, 1, 1);

    m_pStackedWidgetObj = new QStackedWidget(this);

    m_pTableTagIOObj = new QTableWidget(m_pStackedWidgetObj);
    m_pStackedWidgetObj->addWidget(m_pTableTagIOObj);

    m_pTableTagTmpObj = new QTableWidget(m_pStackedWidgetObj);
    m_pStackedWidgetObj->addWidget(m_pTableTagTmpObj);

    m_pTableTagSysObj = new QTableWidget(m_pStackedWidgetObj);
    m_pStackedWidgetObj->addWidget(m_pTableTagSysObj);

    m_pTopVLayoutObj->addWidget(m_pStackedWidgetObj);
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
    if(m_szItemName == tr("变量管理") || m_szItemName == tr("系统变量")) return;

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
 * @brief TagManagerChild::initialTableTagSys
 * @details 初始化系统变量表
 */
void TagManagerChild::initialTableTagSys()
{
    QStringList headerLabels;
    headerLabels << tr("ID") << tr("名称") << tr("变量描述") << tr("单位") << tr("工程转换") << tr("备注");
    m_pTableTagSysObj->setColumnCount(headerLabels.count());
    m_pTableTagSysObj->setHorizontalHeaderLabels(headerLabels);
    m_pTableTagSysObj->horizontalHeader()->setSectionsClickable(false);
    m_pTableTagSysObj->horizontalHeader()->setStretchLastSection(false);
    //m_pTableTagSysObj->horizontalHeader()->setHighlightSections(true); // 当表格只有一行的时候，则表头会出现塌陷问题
    m_pTableTagSysObj->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableTagSysObj->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableTagSysObj->setSelectionMode(QAbstractItemView::SingleSelection);
    /*m_pTableTagSysObj->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background:rgb(72,161,229); "
                "color: rgb(255, 255, 255); "
                "}");*/
    m_pTableTagSysObj->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableTagSysObj->horizontalHeader()->show();
    m_pTableTagSysObj->verticalHeader()->show();
    m_pTableTagSysObj->setColumnWidth(0, 80);
    m_pTableTagSysObj->setColumnWidth(1, 160);
    m_pTableTagSysObj->setColumnWidth(2, 200);
    m_pTableTagSysObj->setColumnWidth(3, 80);
    m_pTableTagSysObj->setColumnWidth(4, 200);
    m_pTableTagSysObj->setColumnWidth(5, 320);

    m_pTableTagSysObj->setAlternatingRowColors(true);
    m_pTableTagSysObj->setFocusPolicy(Qt::NoFocus);
    m_pTableTagSysObj->setWordWrap(false);

    QString styleSheet = "QTableWidget{background-color: rgb(242,242,242);"
                         "alternate-background-color: white;"
                         "color: rgb(0, 0, 0);"
                         "selection-background-color:lightblue;}";

    //m_pTableTagSysObj->setStyleSheet(styleSheet);
    m_pTableTagSysObj->clearSelection();
}

/**
 * @brief TagManagerChild::updateTableTagSys
 * @details 刷新系统变量表
 */
void TagManagerChild::updateTableTagSys()
{
    TagSys &tagSys = ProjectData::getInstance()->tagSys_;

    m_pTableTagSysObj->clearContents();
    m_pTableTagSysObj->setRowCount(0);

    foreach (TagSysDBItem * itemTagSys, tagSys.listTagSysDBItem_) {
        int iRowCount = m_pTableTagSysObj->rowCount();
        m_pTableTagSysObj->insertRow(iRowCount);

        QTableWidgetItem *pItemID = new QTableWidgetItem(itemTagSys->m_szTagID);
        m_pTableTagSysObj->setItem(iRowCount, 0, pItemID);
        QTableWidgetItem *pItemName = new QTableWidgetItem(itemTagSys->m_szName);
        m_pTableTagSysObj->setItem(iRowCount, 1, pItemName);
        QTableWidgetItem *pItemDescription = new QTableWidgetItem(itemTagSys->m_szDescription);
        m_pTableTagSysObj->setItem(iRowCount, 2, pItemDescription);
        QTableWidgetItem *pItemUnit = new QTableWidgetItem(itemTagSys->m_szUnit);
        m_pTableTagSysObj->setItem(iRowCount, 3, pItemUnit);
        QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem(itemTagSys->m_szProjectConverter);
        m_pTableTagSysObj->setItem(iRowCount, 4, pItemProjectConverter);
        QTableWidgetItem *pItemComments = new QTableWidgetItem(itemTagSys->m_szComments);
        m_pTableTagSysObj->setItem(iRowCount, 5, pItemComments);
    }
}



/**
 * @brief TagManagerChild::initialTableTagTmp
 * @details 初始化中间变量表
 */
void TagManagerChild::initialTableTagTmp()
{
    QStringList headerLabels;
    headerLabels << tr("ID") << tr("名称") << tr("变量描述") << tr("数据类型")
                 << tr("初始值") << tr("最小值") << tr("最大值") << tr("工程转换");

    m_pTableTagTmpObj->setColumnCount(headerLabels.count());
    m_pTableTagTmpObj->setHorizontalHeaderLabels(headerLabels);
    m_pTableTagTmpObj->horizontalHeader()->setSectionsClickable(false);
    m_pTableTagTmpObj->horizontalHeader()->setStretchLastSection(false);
    //m_pTableTagTmpObj->horizontalHeader()->setHighlightSections(true);
    m_pTableTagTmpObj->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /*m_pTableTagTmpObj->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background:rgb(72,161,229); "
                "color: rgb(255, 255, 255); "
                "}");*/
    m_pTableTagTmpObj->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableTagTmpObj->horizontalHeader()->show();
    m_pTableTagTmpObj->verticalHeader()->show();
    m_pTableTagTmpObj->setColumnWidth(0, 80);
    m_pTableTagTmpObj->setColumnWidth(1, 120);
    m_pTableTagTmpObj->setColumnWidth(2, 200);
    m_pTableTagTmpObj->setColumnWidth(3, 180);
    m_pTableTagTmpObj->setColumnWidth(4, 100);
    m_pTableTagTmpObj->setColumnWidth(5, 100);
    m_pTableTagTmpObj->setColumnWidth(6, 100);
    m_pTableTagTmpObj->setColumnWidth(7, 200);

    m_pTableTagTmpObj->setWordWrap(false);
    //m_pTableTagTmpObj->setAlternatingRowColors(true);
    //m_pTableTagTmpObj->setFocusPolicy(Qt::NoFocus);

    QString styleSheet = "QTableWidget{background-color: rgb(242,242,242);"
                         "alternate-background-color: white;"
                         "color: rgb(0, 0, 0);"
                         "selection-background-color:lightblue;}";

    //m_pTableTagTmpObj->setStyleSheet(styleSheet);
    m_pTableTagTmpObj->clearSelection();

    QStringList listDataType;
    listDataType << tr("Bit1开关量")
                 << tr("Char8位有符号数")
                 << tr("Byte8位无符号数")
                 << tr("Short16位有符号数")
                 << tr("Word16位无符号数")
                 << tr("ASCII2个字符")
                 << tr("Long32位有符号数")
                 << tr("Dword32位无符号数")
                 << tr("Float单精度浮点数")
                 << tr("String字符串")
                 << tr("Double双精度浮点数")
                 << tr("BCD")
                 << tr("LongLong64位有符号数")
                 << tr("DwordDword64位无符号数");
    TableViewComboBoxDelegate *m_pDelegate = new TableViewComboBoxDelegate(listDataType, false, m_pTableTagTmpObj);
    m_pTableTagTmpObj->setItemDelegateForColumn(3, m_pDelegate);
}


/**
 * @brief TagManagerChild::tableTagTmpAddRow
 * @details 中间变量表新增一行
 * @return 新增的行号
 */
int TagManagerChild::tableTagTmpAddRow()
{
#define XX(item) \
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    int iRowCount = m_pTableTagTmpObj->rowCount();
    m_pTableTagTmpObj->insertRow(iRowCount);

    QTableWidgetItem *pItemID = new QTableWidgetItem("");
    m_pTableTagTmpObj->setItem(iRowCount, 0, pItemID);

    QTableWidgetItem *pItemName = new QTableWidgetItem("");
    XX(pItemName);
    m_pTableTagTmpObj->setItem(iRowCount, 1, pItemName);

    QTableWidgetItem *pItemDescription = new QTableWidgetItem("");
    XX(pItemDescription);
    m_pTableTagTmpObj->setItem(iRowCount, 2, pItemDescription);

    QTableWidgetItem *pItemDataType = new QTableWidgetItem("");
    XX(pItemDataType);
    m_pTableTagTmpObj->setItem(iRowCount, 3, pItemDataType);

    QTableWidgetItem *pItemInitVal = new QTableWidgetItem("");
    XX(pItemInitVal);
    m_pTableTagTmpObj->setItem(iRowCount, 4, pItemInitVal);

    QTableWidgetItem *pItemMinVal = new QTableWidgetItem("");
    XX(pItemMinVal);
    m_pTableTagTmpObj->setItem(iRowCount, 5, pItemMinVal);

    QTableWidgetItem *pItemMaxVal = new QTableWidgetItem("");
    XX(pItemMaxVal);
    m_pTableTagTmpObj->setItem(iRowCount, 6, pItemMaxVal);

    QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem("");
    m_pTableTagTmpObj->setItem(iRowCount, 7, pItemProjectConverter);

#undef XX
    return iRowCount;
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
    int iRowCnt = m_pTableTagTmpObj->rowCount();
    if(iRowCnt > 0) {
        QString szIDText = "";
        QTableWidgetItem *pItemID = m_pTableTagTmpObj->item(iRow, 0);
        if(pItemID != Q_NULLPTR) {
            szIDText = pItemID->text();
        }
        QString szStartText = "tmp.";
        QString szTmp = "0";
        if(szIDText.startsWith(szStartText)) {
            szTmp = szIDText.remove(0, szStartText.length());
            id = szTmp.toInt();
        }
    }
    return id;
}

/**
 * @brief TagManagerChild::updateTableTagTmp
 * @details 刷新中间变量表
 */
void TagManagerChild::updateTableTagTmp()
{
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    m_pTableTagTmpObj->clearContents();
    m_pTableTagTmpObj->setRowCount(0);
    foreach (TagTmpDBItem * itemTagTmp, tagTmp.listTagTmpDBItem_) {
        int iRow = tableTagTmpAddRow();
        setTagTmpObjByRow(iRow, itemTagTmp);
    }
}


/**
 * @brief TagManagerChild::tagTmpExportToCsv
 * @details 中间变量表变量导出至CSV文件
 * @param path
 */
void TagManagerChild::tagTmpExportToCsv(const QString &path, const QString &/*group*/)
{
    QtCSV::StringData varData;
    int iRowCount = m_pTableTagTmpObj->rowCount();
    for(int i=0; i<iRowCount; i++) {
        TagTmpDBItem * pTagTmp = getTagTmpObjByRow(i);

        QStringList varRow;
        varRow << pTagTmp->m_szTagID << pTagTmp->m_szName << pTagTmp->m_szDataType
               << pTagTmp->m_szDescription << pTagTmp->m_szInitVal << pTagTmp->m_szMinVal
               << pTagTmp->m_szMaxVal << pTagTmp->m_szProjectConverter ;
        varData.addRow(varRow);

        if(pTagTmp != Q_NULLPTR) {
            delete pTagTmp;
            pTagTmp = Q_NULLPTR;
        }
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
    QString filepath = path;

    QList<QStringList> data = QtCSV::Reader::readToList(filepath,
                                                        QString(","),
                                                        QString("\""),
                                                        QTextCodec::codecForName("GB18030"));
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    for(int i=0; i<data.size(); i++) {
        QStringList row = data.at(i);
        if(row.at(0) == "ID") continue;

        TagTmpDBItem * pTagTmp = new TagTmpDBItem();
        int iRowCnt = m_pTableTagTmpObj->rowCount();
        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
        pTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
        //pTagTmp->m_szTagID = row.at(0);
        pTagTmp->m_szName = row.at(1);
        pTagTmp->m_szDescription = row.at(2);
        pTagTmp->m_szDataType = row.at(3);
        pTagTmp->m_szInitVal = row.at(4);
        pTagTmp->m_szMinVal = row.at(5);
        pTagTmp->m_szMaxVal = row.at(6);
        pTagTmp->m_szProjectConverter = row.at(7);

        int iRow = tableTagTmpAddRow();
        setTagTmpObjByRow(iRow, pTagTmp);

        tagTmp.listTagTmpDBItem_.append(pTagTmp);
    }
}

/**
 * @brief TagManagerChild::createTagTmp
 * @details 创建中间变量
 */
void TagManagerChild::createTagTmp()
{
    TagTmpEditDialog *pDlg = new TagTmpEditDialog(this);
    if(pDlg->exec() == QDialog::Accepted) {
        int num = pDlg->createTagNum();
        int iRowCnt = m_pTableTagTmpObj->rowCount();
        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
        TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
        for(int i=0; i<num; i++) {
            TagTmpDBItem * pTagTmp = new TagTmpDBItem();
            pTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
            pTagTmp->m_szName = pDlg->tagName();
            pTagTmp->m_szDescription = pDlg->tagDescription();
            pTagTmp->m_szDataType = pDlg->tagDataType();
            pTagTmp->m_szInitVal = pDlg->tagInitValue();
            pTagTmp->m_szMinVal = pDlg->tagMinValue();
            pTagTmp->m_szMaxVal = pDlg->tagMaxValue();
            pTagTmp->m_szProjectConverter = "";

            int iRow = tableTagTmpAddRow();
            setTagTmpObjByRow(iRow, pTagTmp);

            tagTmp.listTagTmpDBItem_.append(pTagTmp);
            id++;
        }
    }
    delete pDlg;
}


/**
 * @brief TagManagerChild::appendTagTmp
 * @details 追加中间变量
 */
void TagManagerChild::appendTagTmp()
{
    int iRowCnt = m_pTableTagTmpObj->rowCount();
    if(iRowCnt < 1) return;

    int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
    TagTmpDBItem * pNewTagTmp = new TagTmpDBItem();
    pNewTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
    pNewTagTmp->m_szName = "";
    pNewTagTmp->m_szDescription = "";
    pNewTagTmp->m_szDataType = "";
    QTableWidgetItem *pItemDataType = m_pTableTagTmpObj->item(iRowCnt - 1, 3);
    if(pItemDataType != Q_NULLPTR) {
        pNewTagTmp->m_szDataType = pItemDataType->text();
    }
    pNewTagTmp->m_szInitVal = "0";
    pNewTagTmp->m_szMinVal = "";
    pNewTagTmp->m_szMaxVal = "";
    pNewTagTmp->m_szProjectConverter = "";
    int iRow = tableTagTmpAddRow();
    setTagTmpObjByRow(iRow, pNewTagTmp);
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    tagTmp.listTagTmpDBItem_.append(pNewTagTmp);
}


/**
 * @brief TagManagerChild::getTagTmpObjByRow
 * @details 获取指定行中间变量对象
 * @param iRow 指定行
 * @return 中间变量对象
 */
TagTmpDBItem *TagManagerChild::getTagTmpObjByRow(int iRow)
{
    int iRowCnt = m_pTableTagTmpObj->rowCount();
    if(iRow < 0 || iRow >= iRowCnt) return Q_NULLPTR;

    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    TagTmpDBItem * pNewTagTmp = Q_NULLPTR;
    QTableWidgetItem *pItemID = m_pTableTagTmpObj->item(iRow, 0);
    if(pItemID != Q_NULLPTR) {
        pNewTagTmp = tagTmp.getTagTmpDBItemByID(ProjectData::getInstance()->dbData_, pItemID->text());
        if(pNewTagTmp == Q_NULLPTR) return Q_NULLPTR;
        pNewTagTmp->m_szTagID = pItemID->text();
    }

    QTableWidgetItem *pItemName = m_pTableTagTmpObj->item(iRow, 1);
    if(pItemName != Q_NULLPTR) {
        pNewTagTmp->m_szName = pItemName->text();
    }
    QTableWidgetItem *pItemDescription = m_pTableTagTmpObj->item(iRow, 2);
    if(pItemDescription != Q_NULLPTR) {
        pNewTagTmp->m_szDescription = pItemDescription->text();
    }
    QTableWidgetItem *pItemDataType = m_pTableTagTmpObj->item(iRow, 3);
    if(pItemDataType != Q_NULLPTR) {
        pNewTagTmp->m_szDataType = pItemDataType->text();
    }
    QTableWidgetItem *pItemInitVal = m_pTableTagTmpObj->item(iRow, 4);
    if(pItemInitVal != Q_NULLPTR) {
        pNewTagTmp->m_szInitVal = pItemInitVal->text();
    }
    QTableWidgetItem *pItemMinVal = m_pTableTagTmpObj->item(iRow, 5);
    if(pItemMinVal != Q_NULLPTR) {
        pNewTagTmp->m_szMinVal = pItemMinVal->text();
    }
    QTableWidgetItem *pItemMaxVal = m_pTableTagTmpObj->item(iRow, 6);
    if(pItemMaxVal != Q_NULLPTR) {
        pNewTagTmp->m_szMaxVal = pItemMaxVal->text();
    }
    QTableWidgetItem *pItemProjectConverter = m_pTableTagTmpObj->item(iRow, 7);
    if(pItemProjectConverter != Q_NULLPTR) {
        pNewTagTmp->m_szProjectConverter = pItemProjectConverter->text();
    }

    return pNewTagTmp;
}


/**
 * @brief TagManagerChild::setTagTmpObjByRow
 * @details 设置指定行中间变量对象
 * @param iRow 指定行
 * @param pObj 中间变量对象
 */
void TagManagerChild::setTagTmpObjByRow(int iRow, TagTmpDBItem *pObj)
{
    int iRowCnt = m_pTableTagTmpObj->rowCount();
    if(pObj == Q_NULLPTR || iRow < 0 || iRow >= iRowCnt) return;

    QTableWidgetItem *pItemID = m_pTableTagTmpObj->item(iRow, 0);
    if(pItemID != Q_NULLPTR) {
        pItemID->setText(pObj->m_szTagID);
    }
    QTableWidgetItem *pItemName = m_pTableTagTmpObj->item(iRow, 1);
    if(pItemName != Q_NULLPTR) {
        pItemName->setText(pObj->m_szName);
    }
    QTableWidgetItem *pItemDescription = m_pTableTagTmpObj->item(iRow, 2);
    if(pItemDescription != Q_NULLPTR) {
        pItemDescription->setText(pObj->m_szDescription);
    }
    QTableWidgetItem *pItemDataType = m_pTableTagTmpObj->item(iRow, 3);
    if(pItemDataType != Q_NULLPTR) {
        pItemDataType->setText(pObj->m_szDataType);
    }
    QTableWidgetItem *pItemInitVal = m_pTableTagTmpObj->item(iRow, 4);
    if(pItemInitVal != Q_NULLPTR) {
        pItemInitVal->setText(pObj->m_szInitVal);
    }
    QTableWidgetItem *pItemMinVal = m_pTableTagTmpObj->item(iRow, 5);
    if(pItemMinVal != Q_NULLPTR) {
        pItemMinVal->setText(pObj->m_szMinVal);
    }
    QTableWidgetItem *pItemMaxVal = m_pTableTagTmpObj->item(iRow, 6);
    if(pItemMaxVal != Q_NULLPTR) {
        pItemMaxVal->setText(pObj->m_szMaxVal);
    }
    QTableWidgetItem *pItemProjectConverter = m_pTableTagTmpObj->item(iRow, 7);
    if(pItemProjectConverter != Q_NULLPTR) {
        pItemProjectConverter->setText(pObj->m_szProjectConverter);
    }
}

/**
 * @brief TagManagerChild::copyCurTagTmp
 * @details 拷贝选中中间变量
 */
void TagManagerChild::copyCurTagTmp()
{
    int iSelectedRow = m_iTableTagTmpSelectedRow;
    if(iSelectedRow >= 0) {
        TagTmpDBItem * pNewTagTmp = getTagTmpObjByRow(iSelectedRow);
        int iRowCnt = m_pTableTagTmpObj->rowCount();
        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
        pNewTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
        int iRow = tableTagTmpAddRow();
        setTagTmpObjByRow(iRow, pNewTagTmp);
        TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
        tagTmp.listTagTmpDBItem_.append(pNewTagTmp);
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
        TagTmpDBItem *pTagTmp = getTagTmpObjByRow(iSelectedRow);

        TagTmpEditDialog *pDlg = new TagTmpEditDialog(this);
        pDlg->setWindowTitle(tr("编辑中间变量"));
        pDlg->hideCreateNumUI();
        pDlg->setTagName(pTagTmp->m_szName);
        pDlg->setTagDescription(pTagTmp->m_szDescription);
        pDlg->setTagDataType(pTagTmp->m_szDataType);
        pDlg->setTagInitValue(pTagTmp->m_szInitVal);
        pDlg->setTagMinValue(pTagTmp->m_szMinVal);
        pDlg->setTagMaxValue(pTagTmp->m_szMaxVal);

        if(pDlg->exec() == QDialog::Accepted) {
            pTagTmp->m_szName = pDlg->tagName();
            pTagTmp->m_szDescription = pDlg->tagDescription();
            pTagTmp->m_szDataType = pDlg->tagDataType();
            pTagTmp->m_szInitVal = pDlg->tagInitValue();
            pTagTmp->m_szMinVal = pDlg->tagMinValue();
            pTagTmp->m_szMaxVal = pDlg->tagMaxValue();
            setTagTmpObjByRow(iSelectedRow, pTagTmp);
        }
        delete pDlg;
    }
}


/**
 * @brief TagManagerChild::deleteCurTagTmp
 * @details 删除选中中间变量
 */
void TagManagerChild::deleteCurTagTmp()
{
    m_listTagTmpDeleteRows.clear();
    QList<QTableWidgetItem *> pSelectItems = m_pTableTagTmpObj->selectedItems();
    QMap<int, int> rowMap;
    foreach (QTableWidgetItem *pItem, pSelectItems) {
        rowMap.insert(pItem->row(), 0);
    }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    while (rowMapIterator.hasPrevious()) {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        QTableWidgetItem *pItemID = m_pTableTagTmpObj->item(rowToDel, 0);
        if(pItemID != Q_NULLPTR) {
            m_listTagTmpDeleteRows << pItemID->text();
            TagTmpDBItem *pTagTmpObj = tagTmp.getTagTmpDBItemByID(ProjectData::getInstance()->dbData_, pItemID->text());
            tagTmp.listTagTmpDBItem_.removeOne(pTagTmpObj);
        }
        m_pTableTagTmpObj->removeRow(rowToDel);
    }
}


/**
 * @brief TagManagerChild::initialTableTagIO
 * @details 初始化设备变量表
 */
void TagManagerChild::initialTableTagIO()
{
    QStringList headerLabels;
    headerLabels << tr("ID") << tr("名称") << tr("变量描述") << tr("设备名")
                 << tr("设备地址") << tr("寄存器区") << tr("寄存器地址") << tr("偏移地址")
                 << tr("读写类型") << tr("数据类型") << tr("初始值") << tr("最小值")
                 << tr("最大值") << tr("变比") << tr("工程转换");

    m_pTableTagIOObj->setColumnCount(headerLabels.count());
    m_pTableTagIOObj->setHorizontalHeaderLabels(headerLabels);
    m_pTableTagIOObj->horizontalHeader()->setSectionsClickable(false);
    m_pTableTagIOObj->horizontalHeader()->setStretchLastSection(true);
    //m_pTableTagIOObj->horizontalHeader()->setHighlightSections(true);
    m_pTableTagIOObj->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_pTableTagIOObj->setSelectionBehavior(QAbstractItemView::SelectRows);
    //m_pTableTagIOObj->setSelectionMode(QAbstractItemView::SingleSelection);
    /*m_pTableTagIOObj->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background:rgb(72,161,229); "
                "color: rgb(255, 255, 255); "
                "}");*/
    m_pTableTagIOObj->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableTagIOObj->horizontalHeader()->show();
    m_pTableTagIOObj->verticalHeader()->show();
    m_pTableTagIOObj->setColumnWidth(0, 100);
    m_pTableTagIOObj->setColumnWidth(1, 120);
    m_pTableTagIOObj->setColumnWidth(2, 200);
    m_pTableTagIOObj->setColumnWidth(3, 100);
    m_pTableTagIOObj->setColumnWidth(4, 70);
    m_pTableTagIOObj->setColumnWidth(5, 120);
    m_pTableTagIOObj->setColumnWidth(6, 80);
    m_pTableTagIOObj->setColumnWidth(7, 60);
    m_pTableTagIOObj->setColumnWidth(8, 70);
    m_pTableTagIOObj->setColumnWidth(9, 120);
    m_pTableTagIOObj->setColumnWidth(10, 80);
    m_pTableTagIOObj->setColumnWidth(11, 80);
    m_pTableTagIOObj->setColumnWidth(12, 80);
    m_pTableTagIOObj->setColumnWidth(13, 50);
    m_pTableTagIOObj->setColumnWidth(14, 180);

    m_pTableTagIOObj->setWordWrap(false);
    //m_pTableTagIOObj->setAlternatingRowColors(true);
    //m_pTableTagIOObj->setFocusPolicy(Qt::NoFocus);

    QString styleSheet = "QTableWidget{background-color: rgb(242,242,242);"
                         "alternate-background-color: white;"
                         "color: rgb(0, 0, 0);"
                         "selection-background-color:lightblue;}";

    //m_pTableTagIOObj->setStyleSheet(styleSheet);
    m_pTableTagIOObj->clearSelection();

    // 设备名称委托
    TableViewComboBoxDelegate *m_pDeviceNameDelegate = new TableViewComboBoxDelegate(QStringList(), false, m_pTableTagIOObj);
    m_pTableTagIOObj->setItemDelegateForColumn(3, m_pDeviceNameDelegate);

    // 寄存区类型委托
    TableViewComboBoxDelegate *m_pRegisterAreaDelegate = new TableViewComboBoxDelegate(QStringList(), false, m_pTableTagIOObj);
    m_pTableTagIOObj->setItemDelegateForColumn(5, m_pRegisterAreaDelegate);

    QStringList listDataType;
    listDataType << tr("只读") << tr("只写") << tr("读写");
    // 寄存区类型委托
    TableViewComboBoxDelegate *m_pReadWriteTypeDelegate = new TableViewComboBoxDelegate(listDataType, false, m_pTableTagIOObj);
    m_pTableTagIOObj->setItemDelegateForColumn(8, m_pReadWriteTypeDelegate);

    // 数据类型委托
    TableViewComboBoxDelegate *m_pDataTypeDelegate = new TableViewComboBoxDelegate(QStringList(), false, m_pTableTagIOObj);
    m_pTableTagIOObj->setItemDelegateForColumn(9, m_pDataTypeDelegate);
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
                    listReg = iDevPlugin->GetDeviceSupportRegisterArea();
                    // 获取设备支持的所有数据类型
                    listType = iDevPlugin->GetDeviceSupportDataType();
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
    int iRowCount = m_pTableTagIOObj->rowCount();
    m_pTableTagIOObj->insertRow(iRowCount);

#define XX(item) \
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    QTableWidgetItem *pItemID = new QTableWidgetItem("");
    m_pTableTagIOObj->setItem(iRowCount, 0, pItemID);
    QTableWidgetItem *pItemName = new QTableWidgetItem("var");
    XX(pItemName);
    m_pTableTagIOObj->setItem(iRowCount, 1, pItemName);
    QTableWidgetItem *pItemDescription = new QTableWidgetItem(tr("描述"));
    XX(pItemName);
    m_pTableTagIOObj->setItem(iRowCount, 2, pItemDescription);

    QTableWidgetItem *pItemDeviceName = new QTableWidgetItem("");
    XX(pItemDeviceName);
    m_pTableTagIOObj->setItem(iRowCount, 3, pItemDeviceName);

    QTableWidgetItem *pItemDeviceAddr = new QTableWidgetItem("1");
    XX(pItemDeviceAddr);
    m_pTableTagIOObj->setItem(iRowCount, 4, pItemDeviceAddr);

    QTableWidgetItem *pItemRegisterArea = new QTableWidgetItem("");
    XX(pItemRegisterArea);
    m_pTableTagIOObj->setItem(iRowCount, 5, pItemRegisterArea);

    QTableWidgetItem *pItemRegisterAddr = new QTableWidgetItem("0");
    XX(pItemRegisterAddr);
    m_pTableTagIOObj->setItem(iRowCount, 6, pItemRegisterAddr);

    QTableWidgetItem *pItemAddrOffset = new QTableWidgetItem("0");
    XX(pItemAddrOffset);
    m_pTableTagIOObj->setItem(iRowCount, 7, pItemAddrOffset);

    QTableWidgetItem *pItemReadWriteType = new QTableWidgetItem("");
    XX(pItemReadWriteType);
    m_pTableTagIOObj->setItem(iRowCount, 8, pItemReadWriteType);

    QTableWidgetItem *pItemDataType = new QTableWidgetItem("");
    XX(pItemDataType);
    m_pTableTagIOObj->setItem(iRowCount, 9, pItemDataType);

    QTableWidgetItem *pItemInitVal = new QTableWidgetItem("0");
    XX(pItemInitVal);
    m_pTableTagIOObj->setItem(iRowCount, 10, pItemInitVal);

    QTableWidgetItem *pItemMinVal = new QTableWidgetItem("0");
    XX(pItemMinVal);
    m_pTableTagIOObj->setItem(iRowCount, 11, pItemMinVal);
    QTableWidgetItem *pItemMaxVal = new QTableWidgetItem("0");
    XX(pItemMaxVal);
    m_pTableTagIOObj->setItem(iRowCount, 12, pItemMaxVal);
    QTableWidgetItem *pItemScale = new QTableWidgetItem("1");
    XX(pItemScale);
    m_pTableTagIOObj->setItem(iRowCount, 13, pItemScale);
    QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem("");
    m_pTableTagIOObj->setItem(iRowCount, 14, pItemProjectConverter);

#undef XX
    return iRowCount;
}


/**
 * @brief TagManagerChild::getTagIOIdNumValue
 * @details 获取指定行设备变量id数值部分
 * @param iRow 行号
 * @return 设备变量id数值部分
 */
int TagManagerChild::getTagIOIdNumValue(int iRow)
{
    int iRowCnt = m_pTableTagIOObj->rowCount();
    int id = 0;
    if(iRowCnt > 0) {
        int iPos = -1;
        QString szVarTmp = "";
        QString szTmp = "0";
        QTableWidgetItem *pItemID = m_pTableTagIOObj->item(iRow, 0);
        if(pItemID != Q_NULLPTR) {
            szVarTmp = pItemID->text();
        }
        if(szVarTmp.startsWith("io.")) {
            iPos = szVarTmp.lastIndexOf(".");
            if(iPos >= 0) {
                szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
                id = szTmp.toInt();
            }
        }
    }
    return id;
}

/**
 * @brief TagManagerChild::updateTableTagIO
 * @details 刷新设备变量表
 */
void TagManagerChild::updateTableTagIO()
{
    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    if(m_szCurIOGroupName == QString()) return;
    m_pTableTagIOObj->clearContents();
    m_pTableTagIOObj->setRowCount(0);
    foreach (TagIODBItem * itemTagIO, tagIO.listTagIODBItem_) {
        if(m_szCurIOGroupName != itemTagIO->m_szGroupName) continue;
        int iRow = tableTagIOAddRow();
        setTagIOObjByRow(iRow, itemTagIO);
    }
}

/**
 * @brief TagManagerChild::getTagIOObjByRow
 * @details 获取指定行设备变量对象
 * @param iRow 行号
 * @return 设备变量对象
 */
TagIODBItem *TagManagerChild::getTagIOObjByRow(int iRow)
{
    int iRowCnt = m_pTableTagIOObj->rowCount();
    if(iRow < 0 || iRow >= iRowCnt) return Q_NULLPTR;

    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    TagIODBItem *pObj = Q_NULLPTR;

    QTableWidgetItem *pItemID = m_pTableTagIOObj->item(iRow, 0);
    if(pItemID != Q_NULLPTR) {
        pObj->m_szTagID = pItemID->text();
        pObj = tagIO.getTagIODBItemByID(pItemID->text());
        if(pObj == Q_NULLPTR) return Q_NULLPTR;
    }
    QTableWidgetItem *pItemName = m_pTableTagIOObj->item(iRow, 1);
    if(pItemName != Q_NULLPTR) {
        pObj->m_szName = pItemName->text();
    }
    QTableWidgetItem *pItemDescription = m_pTableTagIOObj->item(iRow, 2);
    if(pItemDescription != Q_NULLPTR) {
        pObj->m_szDescription = pItemDescription->text();
    }
    QTableWidgetItem *pItemDeviceName = m_pTableTagIOObj->item(iRow, 3);
    if(pItemDeviceName != Q_NULLPTR) {
        pObj->m_szDeviceName = pItemDeviceName->text();
    }
    QTableWidgetItem *pItemDeviceAddr = m_pTableTagIOObj->item(iRow, 4);
    if(pItemDeviceAddr != Q_NULLPTR) {
        pObj->m_szDeviceAddr = pItemDeviceAddr->text();
    }
    QTableWidgetItem *pItemRegisterArea = m_pTableTagIOObj->item(iRow, 5);
    if(pItemRegisterArea != Q_NULLPTR) {
        pObj->m_szRegisterArea = pItemRegisterArea->text();
    }
    QTableWidgetItem *pItemRegisterAddr = m_pTableTagIOObj->item(iRow, 6);
    if(pItemRegisterAddr != Q_NULLPTR) {
        pObj->m_szRegisterAddr = pItemRegisterAddr->text();
    }
    QTableWidgetItem *pItemAddrOffset = m_pTableTagIOObj->item(iRow, 7);
    if(pItemAddrOffset != Q_NULLPTR) {
        pObj->m_szAddrOffset = pItemAddrOffset->text();
    }
    QTableWidgetItem *pItemReadWriteType = m_pTableTagIOObj->item(iRow, 8);
    if(pItemReadWriteType != Q_NULLPTR) {
        pObj->m_szReadWriteType = pItemReadWriteType->text();
    }
    QTableWidgetItem *pItemDataType = m_pTableTagIOObj->item(iRow, 9);
    if(pItemDataType != Q_NULLPTR) {
        pObj->m_szDataType = pItemDataType->text();
    }
    QTableWidgetItem *pItemInitVal = m_pTableTagIOObj->item(iRow, 10);
    if(pItemInitVal != Q_NULLPTR) {
        pObj->m_szInitVal = pItemInitVal->text();
    }
    QTableWidgetItem *pItemMinVal = m_pTableTagIOObj->item(iRow, 11);
    if(pItemMinVal != Q_NULLPTR) {
        pObj->m_szMinVal = pItemMinVal->text();
    }
    QTableWidgetItem *pItemMaxVal = m_pTableTagIOObj->item(iRow, 12);
    if(pItemMaxVal != Q_NULLPTR) {
        pObj->m_szMaxVal = pItemMaxVal->text();
    }
    QTableWidgetItem *pItemScale = m_pTableTagIOObj->item(iRow, 13);
    if(pItemScale != Q_NULLPTR) {
        pObj->m_szScale = pItemScale->text();
    }
    QTableWidgetItem *pItemProjectConverter = m_pTableTagIOObj->item(iRow, 14);
    if(pItemProjectConverter != Q_NULLPTR) {
        pObj->m_szProjectConverter = pItemProjectConverter->text();
    }

    pObj->m_szGroupName = m_szCurIOGroupName;

    return pObj;
}

/**
 * @brief TagManagerChild::setTagIOObjByRow
 * @details 设置指定行设备变量对象
 * @param iRow 行号
 * @param pObj 设备变量对象
 */
void TagManagerChild::setTagIOObjByRow(int iRow, TagIODBItem *pObj)
{
    int iRowCnt = m_pTableTagIOObj->rowCount();
    if(iRow < 0 || iRow >= iRowCnt) return;

    QTableWidgetItem *pItemID = m_pTableTagIOObj->item(iRow, 0);
    if(pItemID != Q_NULLPTR) {
        pItemID->setText(pObj->m_szTagID);
    }
    QTableWidgetItem *pItemName = m_pTableTagIOObj->item(iRow, 1);
    if(pItemName != Q_NULLPTR) {
        pItemName->setText(pObj->m_szName);
    }
    QTableWidgetItem *pItemDescription = m_pTableTagIOObj->item(iRow, 2);
    if(pItemDescription != Q_NULLPTR) {
        pItemDescription->setText(pObj->m_szDescription);
    }
    QTableWidgetItem *pItemDeviceName = m_pTableTagIOObj->item(iRow, 3);
    if(pItemDeviceName != Q_NULLPTR) {
        pItemDeviceName->setText(pObj->m_szDeviceName);
    }
    QTableWidgetItem *pItemDeviceAddr = m_pTableTagIOObj->item(iRow, 4);
    if(pItemDeviceAddr != Q_NULLPTR) {
        pItemDeviceAddr->setText(pObj->m_szDeviceAddr);
    }
    QTableWidgetItem *pItemRegisterArea = m_pTableTagIOObj->item(iRow, 5);
    if(pItemRegisterArea != Q_NULLPTR) {
        pItemRegisterArea->setText(pObj->m_szRegisterArea);
    }
    QTableWidgetItem *pItemRegisterAddr = m_pTableTagIOObj->item(iRow, 6);
    if(pItemRegisterAddr != Q_NULLPTR) {
        pItemRegisterAddr->setText(pObj->m_szRegisterAddr);
    }
    QTableWidgetItem *pItemAddrOffset = m_pTableTagIOObj->item(iRow, 7);
    if(pItemAddrOffset != Q_NULLPTR) {
        pItemAddrOffset->setText(pObj->m_szAddrOffset);
    }
    QTableWidgetItem *pItemReadWriteType = m_pTableTagIOObj->item(iRow, 8);
    if(pItemReadWriteType != Q_NULLPTR) {
        pItemReadWriteType->setText(pObj->m_szReadWriteType);
    }
    QTableWidgetItem *pItemDataType = m_pTableTagIOObj->item(iRow, 9);
    if(pItemDataType != Q_NULLPTR) {
        pItemDataType->setText(pObj->m_szDataType);
    }
    QTableWidgetItem *pItemInitVal = m_pTableTagIOObj->item(iRow, 10);
    if(pItemInitVal != Q_NULLPTR) {
        pItemInitVal->setText(pObj->m_szInitVal);
    }
    QTableWidgetItem *pItemMinVal = m_pTableTagIOObj->item(iRow, 11);
    if(pItemMinVal != Q_NULLPTR) {
        pItemMinVal->setText(pObj->m_szMinVal);
    }
    QTableWidgetItem *pItemMaxVal = m_pTableTagIOObj->item(iRow, 12);
    if(pItemMaxVal != Q_NULLPTR) {
        pItemMaxVal->setText(pObj->m_szMaxVal);
    }
    QTableWidgetItem *pItemScale = m_pTableTagIOObj->item(iRow, 13);
    if(pItemScale != Q_NULLPTR) {
        pItemScale->setText(pObj->m_szScale);
    }
    QTableWidgetItem *pItemProjectConverter = m_pTableTagIOObj->item(iRow, 14);
    if(pItemProjectConverter != Q_NULLPTR) {
        pItemProjectConverter->setText(pObj->m_szProjectConverter);
    }
}


/**
 * @brief TagManagerChild::tagIOExportToCsv
 * @details 设备变量表变量导出至CSV文件
 * @param path
 */
void TagManagerChild::tagIOExportToCsv(const QString &path, const QString &group)
{
    QtCSV::StringData varData;
    int iRowCount = m_pTableTagIOObj->rowCount();
    for(int i=0; i<iRowCount; i++) {
        TagIODBItem * pTagIO = getTagIOObjByRow(i);

        QStringList varRow;
        varRow << pTagIO->m_szTagID << pTagIO->m_szGroupName << pTagIO->m_szName<< pTagIO->m_szDescription
               << pTagIO->m_szDeviceName << pTagIO->m_szDeviceAddr << pTagIO->m_szRegisterArea << pTagIO->m_szRegisterAddr
               << pTagIO->m_szAddrOffset << pTagIO->m_szReadWriteType << pTagIO->m_szDataType << pTagIO->m_szInitVal
               << pTagIO->m_szMinVal << pTagIO->m_szMaxVal << pTagIO->m_szScale << pTagIO->m_szProjectConverter;

        varData.addRow(varRow);
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
    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    for(int i=0; i<data.size(); i++) {
        QStringList row = data.at(i);
        if(row.at(0) == "ID") continue;

        TagIODBItem * pTagIO = new TagIODBItem();
        int iRowCnt = m_pTableTagIOObj->rowCount();
        int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
        (void)id;
        //pTagIO->m_szTagID = QString("tmp.%1").arg(QString::number(id));

        // TODO

        //pTagIO->m_szTagID = row.at(0);
        pTagIO->m_szGroupName = row.at(1);
        pTagIO->m_szName = row.at(2);
        pTagIO->m_szDescription = row.at(3);
        pTagIO->m_szDeviceName = row.at(4);
        pTagIO->m_szDeviceAddr = row.at(5);
        pTagIO->m_szRegisterArea = row.at(6);
        pTagIO->m_szRegisterAddr = row.at(7);
        pTagIO->m_szAddrOffset = row.at(9);
        pTagIO->m_szReadWriteType = row.at(10);
        pTagIO->m_szDataType = row.at(11);
        pTagIO->m_szInitVal = row.at(12);
        pTagIO->m_szMinVal = row.at(13);
        pTagIO->m_szMaxVal = row.at(14);
        pTagIO->m_szScale = row.at(15);
        pTagIO->m_szProjectConverter = row.at(16);

        int iRow = tableTagIOAddRow();
        setTagIOObjByRow(iRow, pTagIO);
        tagIO.listTagIODBItem_.append(pTagIO);
    }
}

/**
 * @brief TagManagerChild::createTagIO
 * @details 创建设备变量
 */
void TagManagerChild::createTagIO()
{
    if(m_szCurIOGroupName == QString()) return;

    // 判断是否已经建立通讯设备
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    if(deviceInfo.listDeviceInfoObject_.count() < 1) {
        QMessageBox::warning(this,
                             tr("提示"),
                             QString(tr("不存在通讯设备，请先新建通讯设备！")));
        return;
    }

    TagIOEditDialog *pDlg = new TagIOEditDialog(m_szProjectName, this);
    if(pDlg->exec() == QDialog::Accepted) {
        int num = pDlg->createTagNum();
        int iOffset = pDlg->addrOffset();
        int iRowCnt = m_pTableTagIOObj->rowCount();
        int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
        TagIO &tagIO = ProjectData::getInstance()->tagIO_;
        for(int i=0; i<num; i++) {
            TagIODBItem * pTagIO = new TagIODBItem();
            pTagIO->m_szTagID = QString("io.%1.%2").arg(m_szCurIOGroupName).arg(QString::number(id));
            pTagIO->m_szGroupName = m_szCurIOGroupName;
            pTagIO->m_szName = pDlg->tagName();
            pTagIO->m_szDescription = pDlg->tagDesc();
            pTagIO->m_szDeviceName = pDlg->deviceName();
            pTagIO->m_szDeviceAddr = pDlg->deviceAddr();
            pTagIO->m_szRegisterArea = pDlg->regSection();
            pTagIO->m_szRegisterAddr = QString::number(pDlg->regAddr().toInt() + iOffset * i);
            pTagIO->m_szAddrOffset = pDlg->regAddrOffset();
            pTagIO->m_szReadWriteType = pDlg->typeReadWrite();
            pTagIO->m_szDataType = pDlg->dataType();
            pTagIO->m_szInitVal  = pDlg->tagInitValue();
            pTagIO->m_szMinVal  = pDlg->tagMinValue();
            pTagIO->m_szMaxVal  = pDlg->tagMaxValue();
            pTagIO->m_szScale  = pDlg->scale();
            pTagIO->m_szProjectConverter = "";

            int iRow = tableTagIOAddRow();
            setTagIOObjByRow(iRow, pTagIO);

            tagIO.listTagIODBItem_.append(pTagIO);
            id++;
        }
    }
    delete pDlg;
}

/**
 * @brief TagManagerChild::appendTagIO
 * @details 追加设备变量
 */
void TagManagerChild::appendTagIO()
{
    int iRowCnt = m_pTableTagIOObj->rowCount();
    if(iRowCnt < 1)
        return;

    if(m_szCurIOGroupName == QString()) return;

    int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
    TagIODBItem * pNewTagIO = new TagIODBItem();
    pNewTagIO->m_szTagID = QString("io.%1.%2").arg(m_szCurIOGroupName).arg(QString::number(id));
    pNewTagIO->m_szGroupName = m_szCurIOGroupName;
    pNewTagIO->m_szName = "";
    pNewTagIO->m_szDescription = "";
    pNewTagIO->m_szDeviceName = "";
    QTableWidgetItem *pItemDeviceName = m_pTableTagIOObj->item(iRowCnt - 1, 3);
    if(pItemDeviceName != Q_NULLPTR) {
        pNewTagIO->m_szDeviceName = pItemDeviceName->text();
    }
    pNewTagIO->m_szDeviceAddr = "1";
    pNewTagIO->m_szRegisterArea = "";
    QTableWidgetItem *pItemRegisterArea = m_pTableTagIOObj->item(iRowCnt - 1, 5);
    if(pItemRegisterArea != Q_NULLPTR) {
        pNewTagIO->m_szRegisterArea = pItemRegisterArea->text();
    }
    pNewTagIO->m_szRegisterAddr = "0";
    pNewTagIO->m_szAddrOffset = "0";
    pNewTagIO->m_szReadWriteType = "";
    QTableWidgetItem *pItemReadWriteType = m_pTableTagIOObj->item(iRowCnt - 1, 8);
    if(pItemReadWriteType != Q_NULLPTR) {
        pNewTagIO->m_szReadWriteType = pItemReadWriteType->text();
    }
    pNewTagIO->m_szDataType  = "";
    QTableWidgetItem *pItemDataType = m_pTableTagIOObj->item(iRowCnt - 1, 9);
    if(pItemDataType != Q_NULLPTR) {
        pNewTagIO->m_szDataType = pItemDataType->text();
    }
    pNewTagIO->m_szInitVal  = "0";
    pNewTagIO->m_szMinVal  = "";
    pNewTagIO->m_szMaxVal  = "";
    pNewTagIO->m_szScale  = "1";
    pNewTagIO->m_szProjectConverter  = "";

    int iRow = tableTagIOAddRow();
    setTagIOObjByRow(iRow, pNewTagIO);

    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    tagIO.listTagIODBItem_.append(pNewTagIO);
}


/**
 * @brief TagManagerChild::copyCurTagIO
 * @details 拷贝选中设备变量
 */
void TagManagerChild::copyCurTagIO()
{
    if(m_szCurIOGroupName == QString()) return;

    int iSelectedRow = m_iTableTagIOSelectedRow;
    if(iSelectedRow >= 0) {
        TagIODBItem * pNewTagIO = getTagIOObjByRow(iSelectedRow);
        int iRowCnt = m_pTableTagIOObj->rowCount();
        int id = getTagIOIdNumValue(iRowCnt - 1) + 1;
        pNewTagIO->m_szTagID = QString("io.%1.%2").arg(m_szCurIOGroupName).arg(QString::number(id));
        int iRow = tableTagIOAddRow();
        setTagIOObjByRow(iRow, pNewTagIO);
        TagIO &tagIO = ProjectData::getInstance()->tagIO_;
        tagIO.listTagIODBItem_.append(pNewTagIO);
    }
}


/**
 * @brief TagManagerChild::modifyCurTagIO
 * @details 修改选中设备变量
 */
void TagManagerChild::modifyCurTagIO()
{
    int iSelectedRow = m_iTableTagIOSelectedRow;
    if(iSelectedRow >= 0) {
        TagIODBItem *pTagIO = getTagIOObjByRow(iSelectedRow);

        TagIOEditDialog *pDlg = new TagIOEditDialog(m_szProjectName, this);
        pDlg->setWindowTitle(tr("编辑设备变量"));
        pDlg->hideCreateNumUI();
        pDlg->setTagName(pTagIO->m_szName);
        pDlg->setTagDesc(pTagIO->m_szDescription);
        pDlg->setDeviceName(pTagIO->m_szDeviceName);
        pDlg->setDeviceAddr(pTagIO->m_szDeviceAddr);
        pDlg->setRegSection(pTagIO->m_szRegisterArea);
        pDlg->setRegAddr(pTagIO->m_szRegisterAddr);
        pDlg->setRegAddrOffset(pTagIO->m_szAddrOffset);
        pDlg->setTypeReadWrite(pTagIO->m_szReadWriteType);
        pDlg->setDataType(pTagIO->m_szDataType);
        pDlg->setTagInitValue(pTagIO->m_szInitVal);
        pDlg->setTagMinValue(pTagIO->m_szMinVal);
        pDlg->setTagMaxValue(pTagIO->m_szMaxVal);
        pDlg->setScale(pTagIO->m_szScale);
        if(pDlg->exec() == QDialog::Accepted) {
            pTagIO->m_szName = pDlg->tagName();
            pTagIO->m_szDescription = pDlg->tagDesc();
            pTagIO->m_szDeviceName = pDlg->deviceName();
            pTagIO->m_szDeviceAddr = pDlg->deviceAddr();
            pTagIO->m_szRegisterArea = pDlg->regSection();
            pTagIO->m_szRegisterAddr = pDlg->regAddr();
            pTagIO->m_szAddrOffset = pDlg->regAddrOffset();
            pTagIO->m_szReadWriteType = pDlg->typeReadWrite();
            pTagIO->m_szDataType = pDlg->dataType();
            pTagIO->m_szInitVal  = pDlg->tagInitValue();
            pTagIO->m_szMinVal  = pDlg->tagMinValue();
            pTagIO->m_szMaxVal  = pDlg->tagMaxValue();
            pTagIO->m_szScale  = pDlg->scale();
            setTagIOObjByRow(iSelectedRow, pTagIO);
        }
        delete pDlg;
    }
}


/**
 * @brief TagManagerChild::deleteCurTagIO
 * @details 删除选中设备变量
 */
void TagManagerChild::deleteCurTagIO()
{
    m_listTagIODeleteRows.clear();
    QList<QTableWidgetItem *> pSelectItems = m_pTableTagIOObj->selectedItems();
    QMap<int, int> rowMap;
    foreach (QTableWidgetItem *pItem, pSelectItems) {
        rowMap.insert(pItem->row(), 0);
    }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    while (rowMapIterator.hasPrevious()) {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        QTableWidgetItem *pItemID = m_pTableTagIOObj->item(rowToDel, 0);
        if(pItemID != Q_NULLPTR) {
            m_listTagIODeleteRows << pItemID->text();
            TagIODBItem *pTagIOObj = tagIO.getTagIODBItemByID(pItemID->text());
            tagIO.listTagIODBItem_.removeOne(pTagIOObj);
        }
        m_pTableTagIOObj->removeRow(rowToDel);
    }
}


void TagManagerChild::on_tableTagIO_itemPressed(QTableWidgetItem *item)
{
    int iColumn = item->column();
    int iRow = item->row();
    m_iTableTagIOSelectedRow = iRow;
    if(iColumn > 0 && iColumn < 14) {
        if(iColumn == 3) { // 设备名
            QAbstractItemDelegate *pDelegateObj = m_pTableTagIOObj->itemDelegateForColumn(3);
            TableViewComboBoxDelegate * pComboBoxDelegateObj = dynamic_cast<TableViewComboBoxDelegate *>(pDelegateObj);
            if(pComboBoxDelegateObj != Q_NULLPTR) {
                QStringList szListDevNames;
                DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
                for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
                    DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
                    szListDevNames.append(pObj->szDeviceName_);
                }
                pComboBoxDelegateObj->setItems(szListDevNames);
            }
        }
        else if(iColumn == 5) { // 寄存器区
            QAbstractItemDelegate *pDelegateObj = m_pTableTagIOObj->itemDelegateForColumn(5);
            TableViewComboBoxDelegate * pComboBoxDelegateObj = dynamic_cast<TableViewComboBoxDelegate *>(pDelegateObj);
            TagIODBItem *pTagIOObj = getTagIOObjByRow(iRow);
            if(pComboBoxDelegateObj != Q_NULLPTR && pTagIOObj != Q_NULLPTR) {
                QStringList listReg;
                QStringList listType;
                getRegTypeByDeviceName(pTagIOObj->m_szDeviceName, listReg, listType);
                pComboBoxDelegateObj->setItems(listReg);
            }
        }
        else if(iColumn == 9) { // 数据类型
            QAbstractItemDelegate *pDelegateObj = m_pTableTagIOObj->itemDelegateForColumn(9);
            TableViewComboBoxDelegate * pComboBoxDelegateObj = dynamic_cast<TableViewComboBoxDelegate *>(pDelegateObj);
            TagIODBItem *pTagIOObj = getTagIOObjByRow(iRow);
            if(pComboBoxDelegateObj != Q_NULLPTR && pTagIOObj != Q_NULLPTR) {
                QStringList listReg;
                QStringList listType;
                getRegTypeByDeviceName(pTagIOObj->m_szDeviceName, listReg, listType);
                pComboBoxDelegateObj->setItems(listType);
            }
        }
        m_pTableTagIOObj->editItem(item);
    }
}

void TagManagerChild::on_tableTagTmp_itemPressed(QTableWidgetItem *item)
{
    int iColumn = item->column();
    int iRow = item->row();
    m_iTableTagTmpSelectedRow = iRow;
    if(iColumn > 0 && iColumn < 7) {
        m_pTableTagTmpObj->editItem(item);
    }
}

/**
 * @brief TagManagerChild::on_tableTagTmp_itemDoubleClicked
 * @details 中间变量表视图项双击处理
 * @param item
 */
void TagManagerChild::on_tableTagTmp_itemDoubleClicked(QTableWidgetItem *item)
{
    int iColumn = item->column();
    int iRow = item->row();

    if(iColumn > 0 && iColumn < 7) {
        // 修改选中中间变量
        modifyCurTagTmp();
    } else if(iColumn == 7) {
        TagTmpDBItem *pTagTmp = getTagTmpObjByRow(iRow);
        TagFuncEditDialog *pDlg = new TagFuncEditDialog(ProjectData::getInstance()->getProjectPath(m_szProjectName), this);
        pDlg->SetData(pTagTmp->m_szProjectConverter);
        if(pDlg->exec() == QDialog::Accepted) {
            pTagTmp->m_szProjectConverter = pDlg->GetData();
            setTagTmpObjByRow(iRow, pTagTmp);
        }
        delete pDlg;
    }
}

void TagManagerChild::on_tableTagIO_itemDoubleClicked(QTableWidgetItem *item)
{
    int iColumn = item->column();
    int iRow = item->row();
    m_iTableTagIOSelectedRow = iRow;
    if(iColumn > 0 && iColumn < 14) {
        // 修改选中设备变量
        modifyCurTagIO();
    } else if(iColumn == 14) {
        TagIODBItem *pTagIO = getTagIOObjByRow(iRow);
        TagFuncEditDialog *pDlg = new TagFuncEditDialog(ProjectData::getInstance()->getProjectPath(m_szProjectName), this);
        pDlg->SetData(pTagIO->m_szProjectConverter);
        if(pDlg->exec() == QDialog::Accepted) {
            pTagIO->m_szProjectConverter = pDlg->GetData();
            setTagIOObjByRow(iRow, pTagIO);
        }
        delete pDlg;
    }
}

void TagManagerChild::on_tableTagIO_cellClicked(int row, int column)
{
    Q_UNUSED(column)
    m_iTableTagIOSelectedRow = row;
}

void TagManagerChild::on_tableTagIO_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    m_iTableTagIOSelectedRow = row;
}

void TagManagerChild::on_tableTagTmp_cellClicked(int row, int column)
{
    Q_UNUSED(column)
    m_iTableTagTmpSelectedRow = row;
}

void TagManagerChild::on_tableTagTmp_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    m_iTableTagTmpSelectedRow = row;
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
    QString szDirPath = QCoreApplication::applicationDirPath();
    QString szSaveCsvFile = QFileDialog::getOpenFileName(this, tr("选择csv文件"),
                                                          szDirPath,
                                                          tr("csv file (*.csv)"));
    if(szSaveCsvFile == "") return;

    this->importFromCsv(szSaveCsvFile);

    QString szCsvName= szSaveCsvFile.mid(szSaveCsvFile.lastIndexOf("/") + 1, szSaveCsvFile.indexOf(".") - szSaveCsvFile.lastIndexOf("/") - 1);
    QString szGroupName = szCsvName.right(szCsvName.length() - szCsvName.lastIndexOf("-") - 1);

    if(szCsvName.startsWith(tr("中间变量"))) {

    } else if(szCsvName.startsWith(tr("设备变量"))) {
        bool bFound = false;

        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
            if(szGroupName == pObj->m_szShowName) {
                bFound = true;
                break;
            }
        }

        if(!bFound) {
            TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
            TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
            pObj->m_id = tagIOGroup.getGroupCount() + 1;
            pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
            pObj->m_szShowName = szGroupName;
            tagIOGroup.listTagIOGroupDBItem_.append(pObj);
            if(m_pMainWinObj) {
                MainWindow *pWndObj = dynamic_cast<MainWindow *>(m_pMainWinObj);
                if(pWndObj) {
                    pWndObj->UpdateDeviceVariableTableGroup();
                    pWndObj->onSlotTreeProjectViewClicked(tr("设备变量"));
                    QApplication::processEvents();
                    pWndObj->onSlotTreeProjectViewClicked(szGroupName);
                    QApplication::processEvents();
                }
            }
        }
    }
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

    if(wndTitle().startsWith(tr("设备变量"))) {
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        QString szGroup = tagIOGroup.getGroupNameByShowName(m_szItemName);
        m_szCurIOGroupName = QString();
        if(szGroup != QString()) {
            m_szCurIOGroupName = szGroup;
        }
        // 刷新设备变量表
        this->updateTableTagIO();
        m_pStackedWidgetObj->setCurrentWidget(m_pTableTagIOObj);
    } else if(wndTitle().startsWith(tr("中间变量"))) {
        // 刷新中间变量表
        this->updateTableTagTmp();
        m_pStackedWidgetObj->setCurrentWidget(m_pTableTagTmpObj);
    } else if(wndTitle().startsWith(tr("系统变量"))) {
        // 刷新系统变量表
        this->updateTableTagSys();
        m_pStackedWidgetObj->setCurrentWidget(m_pTableTagSysObj);
    }
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





