#include "TagManagerWin.h"
#include "ui_TagManagerWin.h"
#include "CommunicationDeviceWin.h"
#include "ui_CommunicationDeviceWin.h"
#include "NewComDeviceDialog.h"
#include "VariableEditDialog.h"
#include "TagTmpEditDialog.h"
#include "stringdata.h"
#include "writer.h"
#include "reader.h"
#include "variantdata.h"
#include "Helper.h"
#include "ProjectMgrUtils.h"
#include "TagManager.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QFile>
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
#include "Tag.h"
#include "TagSys.h"
#include <QDebug>


TagManagerWin::TagManagerWin(QWidget *parent,
                             const QString &itemName,
                             const QString &projName) :
    ChildBase(parent, itemName, projName),
    ui(new Ui::TagManagerWin),
    m_strCurTagType("")
{
    ui->setupUi(this);

    ui->verticalLayout->setSpacing(1);
    ui->verticalLayout->setContentsMargins(1, 1, 1, 1);

    setContextMenuPolicy(Qt::DefaultContextMenu);
    // 初始化系统变量表
    initialTableTagSys();
    // 初始化中间变量表
    initialTableTagTmp();
    // 初始化设备变量表
    initialTableTagIO();

}

TagManagerWin::~TagManagerWin()
{
    delete ui;
}

void TagManagerWin::init(const QString &itemName)
{   
    if(m_strItemName == tr("设备变量")) {
        ui->stackedWidget->setCurrentWidget(ui->pageTagIO);
    } else if(m_strItemName == tr("中间变量")) {
        ui->stackedWidget->setCurrentWidget(ui->pageTagTmp);
    } else if(m_strItemName == tr("系统变量")) {
        ui->stackedWidget->setCurrentWidget(ui->pageTagSys);
    }

    if(m_strCurTagType != itemName) {
        if(!m_strCurTagType.isEmpty()) {
            SetTitle(m_strCurTagType);
            save();
        }

        SetTitle(itemName);
        load(m_strItemName);
    }

    if(itemName.indexOf(tr("设备变量-")) > -1)
        SetTitle(itemName);

    //m_variableTableView->horizontalHeader()->setHighlightSections(false);  // 当表格只有一行的时候，则表头会出现塌陷问题
}

void TagManagerWin::SetTitle(const QString &t)
{
    if(t == tr("中间变量") || t == tr("系统变量")) {
        m_strItemName = t;
    } else {
        m_strItemName = tr("设备变量");
        QString str = t;
        m_IOVariableListWhat = str.replace(QString(tr("设备变量-")), "");
    }
    m_strCurTagType = t;
}


void TagManagerWin::load(const QString &it)
{
    if(it == "设备变量") {
        // 刷新设备变量表
        this->updateTableTagIO(it);
    } else if(it == "中间变量") {
        // 刷新中间变量表
        this->updateTableTagTmp();
    } else if(it == "系统变量") {
        // 刷新系统变量表
        this->updateTableTagSys();
    }
}

void TagManagerWin::save(const QString &it)
{
    if(it == "设备变量") {
        // 保存设备变量表
        this->saveTableTagIO();
    } else if(it == "中间变量") {
        // 保存中间变量表
        this->saveTableTagTmp();
    } else if(it == "系统变量") {
        // do nothing here!
    }
}

/*
* 导出变量表
*/
void TagManagerWin::exportToCsv(const QString &path)
{
    if(m_strItemName == tr("设备变量")) {
        // 设备变量表变量导出至CSV文件
        this->tagIOExportToCsv(path, QString("%1-%2").arg(m_strItemName).arg(m_IOVariableListWhat));
    } else if(m_strItemName == tr("中间变量")) {
        // 中间变量表变量导出至CSV文件
        tagTmpExportToCsv(path, m_strItemName);
    }
}


/*
* 导入变量表
*/
void TagManagerWin::importFromCsv(const QString &path)
{
    if(path.contains(tr("设备变量"))) {
        // // 从CSV文件导入设备变量至设备变量表
        this->tagIOImportFromCsv(path);
    } else if(path.contains(tr("中间变量"))) {
        // 从CSV文件导入中间变量至中间变量表
        tagTmpImportFromCsv(path);
    }
}


/*
* 右键菜单
*/
void TagManagerWin::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    if(m_strItemName == tr("变量管理") || m_strItemName == tr("系统变量"))
        return;

    QMenu *pMenu = new QMenu(this);

    QAction *pAddAct = new QAction(QIcon(":/images/data_add.png"), tr("增加"), this);
    pAddAct->setStatusTip(tr("新增变量"));
    connect(pAddAct, SIGNAL(triggered()), this, SLOT(onTagAdd()));
    pMenu->addAction(pAddAct);

    QAction *pAppendAct = new QAction(QIcon(":/images/data_supadd.png"), tr("追加"), this);
    pAppendAct->setStatusTip(tr("追加"));
    connect(pAppendAct, SIGNAL(triggered()), this, SLOT(onTagAppend()));
    pMenu->addAction(pAppendAct);

    QAction *pRowCopyAct = new QAction(QIcon(":/images/data_rowcopy.png"), tr("行拷"), this);
    pRowCopyAct->setStatusTip(tr("行拷"));
    connect(pRowCopyAct, SIGNAL(triggered()), this, SLOT(onTagRowCopy()));
    pMenu->addAction(pRowCopyAct);

    QAction *pColCopyAct = new QAction(QIcon(":/images/data_colcopy.png"), tr("列拷"), this);
    pColCopyAct->setStatusTip(tr("列拷"));
    connect(pColCopyAct, SIGNAL(triggered()), this, SLOT(onTagColCopy()));
    pMenu->addAction(pColCopyAct);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    pModifyAct->setStatusTip(tr("修改"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(onTagModify()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/data_delete.png"), tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(onTagDelete()));
    pMenu->addAction(pDeleteAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}



/*
* 插槽：增加变量
*/
void TagManagerWin::onTagAdd()
{
    int i;
    int num;
#if 0
    if(m_strItemName == tr("设备变量")) {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->RemoveTab(1); // 隐藏数据属性页
        if(pDlg->exec() == QDialog::Accepted) {
            num = pDlg->GetBatchNum().toInt(&ok, 10);
            if(ok) {
                int iPageID = 0;
                QString szProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
                TagManager::clearData();
                TagManager::loadProjectTags(szProjectPath);
                foreach(DBVarGroup *var, TagManager::ioDBVarGroups_.varBlockGroupList_) {
                    if(m_IOVariableListWhat == var->m_name) {
                        iPageID = static_cast<int>(var->m_iPageID);
                        break;
                    }
                }

                for(i=0; i<num; i++) {
                    TagIOItem prevItem;
                    TagIOItem newItem;

                    int id = 1;
                    int iPos = -1;
                    QString szVarTmp = "";
                    QString szTmp = "0";
                    if(pTagIOTableModel->rowCount() > 0) {
                        prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
                        szVarTmp = prevItem.m_sTagID;
                        if(szVarTmp.startsWith("io.")) {
                            iPos = szVarTmp.lastIndexOf(".");
                            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
                            id = szTmp.toInt() + 1;
                        }
                    }

                    if(iPos == -1) {
                        newItem.m_sTagID =  QString("io.group%1.%2")
                                .arg(QString::number(iPageID))
                                .arg(QString::number(id));
                    } else {
                        newItem.m_sTagID =  QString("%1%2")
                                .arg(szVarTmp.left(iPos + 1))
                                .arg(QString::number(id));
                    }

                    newItem.m_sDataType = pDlg->GetDataType();
                    newItem.m_sName = pDlg->GetName() + QString("%1").arg(i+1);
                    newItem.m_sDescription = pDlg->GetDescription();
                    newItem.m_sUnit = pDlg->GetUnit();

                    int offset = pDlg->GetDataRegisterSpace().toInt() * i;
                    pDlg->SetBatchOffset(offset);
                    newItem.m_sIOConnect = pDlg->GetIOConnectString();

                    newItem.m_sAlarm = pDlg->GetAlarmString();
                    newItem.m_sArchiveFile = pDlg->GetSaveDiskString();
                    pTagIOTableModel->AppendRow(newItem);
                }
            }
        }
        delete pDlg;
    } else
#endif
        if(m_strItemName == tr("中间变量")) {
            // 创建中间变量
            createTagTmp();
        }
}


/*
* 插槽：追加变量
*/
void TagManagerWin::onTagAppend()
{
#if 0
    if(m_strItemName == tr("设备变量")) {
        if(pTagIOTableModel->rowCount() < 1)
            return;

        TagIOItem prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
        TagIOItem newItem;

        int id = 1;
        int iPos = 0;
        QString szVarTmp = "";
        QString szTmp = "0";

        prevItem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);
        szVarTmp = prevItem.m_sTagID;
        if(szVarTmp.startsWith("io.")) {
            iPos = szVarTmp.lastIndexOf(".");
            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
            id = szTmp.toInt() + 1;
        }

        newItem.m_sTagID =  QString("%1%2")
                .arg(szVarTmp.left(iPos + 1))
                .arg(QString::number(id));

        newItem.m_sDataType = tr("模拟量");
        // 获取前一行的Name
        QString lastVarName = prevItem.m_sName;
        QString str = "var";
        if(lastVarName.indexOf("var") > -1) {
            int len = lastVarName.size();
            QString subStr = lastVarName.right(len - 3);
            int idx = subStr.toInt();
            str = QString("var%1").arg(idx+1);
        }
        newItem.m_sName = str;
        newItem.m_sDescription = tr("描述");
        newItem.m_sUnit = "mA";
        pTagIOTableModel->AppendRow(newItem);
    } else
#endif
        if(m_strItemName == "中间变量") {
            // 追加中间变量
            appendTagTmp();
        }

}


/*
* 插槽：行拷贝变量
*/
void TagManagerWin::onTagRowCopy()
{
#if 0
    QModelIndex ModelIndex = m_variableTableView->selectionModel()->currentIndex();
    int row = ModelIndex.row();
    int column = ModelIndex.column();

    if(row < 0 || column < 0)
        return;

    if(m_strItemName == tr("设备变量")) {
        TagIOItem curitem = pTagIOTableModel->GetRow(row);
        TagIOItem lastitem = pTagIOTableModel->GetRow(pTagIOTableModel->rowCount()-1);

        int id = 1;
        int iPos = 0;
        QString szVarTmp = "";
        QString szTmp = "0";

        szVarTmp = lastitem.m_sTagID;
        if(szVarTmp.startsWith("io.")) {
            iPos = szVarTmp.lastIndexOf(".");
            szTmp = szVarTmp.right(szVarTmp.length() - iPos - 1);
            id = szTmp.toInt() + 1;
        }

        curitem.m_sTagID =  QString("%1%2").arg(szVarTmp.left(iPos + 1)).arg(QString::number(id));

        pTagIOTableModel->AppendRow(curitem);
    } else
#endif
        if(m_strItemName == tr("中间变量")) {
            // 拷贝选中中间变量
            copyCurTagTmp();
        }
}

/*
* 插槽：列拷贝变量
*/
void TagManagerWin::onTagColCopy()
{
    if(m_strItemName == tr("设备变量")) {
    } else if(m_strItemName == tr("中间变量")) {
    }
}

/*
* 插槽：修改变量
*/
void TagManagerWin::onTagModify()
{
#if 0
    QStringList sl;
    int rowIndex = m_variableTableView->currentIndex().row();

    if(m_strItemName == tr("设备变量")) {
        VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
        pDlg->setWindowTitle(tr("编辑设备变量"));
        TagIOItem item = pTagIOTableModel->GetRow(rowIndex);

        if(item.m_sDataType == tr("模拟量"))
            pDlg->SetVariableType(VariableEditDialog::AI);
        else if(item.m_sDataType == tr("数字量"))
            pDlg->SetVariableType(VariableEditDialog::DI);
        else
            pDlg->SetVariableType(VariableEditDialog::NONE);

        sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;
        pDlg->SetBasicSetting(sl);

        // Removes the tab at position index from this stack of widgets.
        // The page widget itself is not deleted.
        pDlg->RemoveTab(1); // 隐藏数据属性页
        pDlg->SetDataAttribuyeString(item.m_sIOConnect);
        pDlg->SetAlarmString(item.m_sAlarm);
        pDlg->SetSaveDiskString(item.m_sArchiveFile);

        if(pDlg->exec() == QDialog::Accepted) {
            QStringList sl = pDlg->GetBasicSetting();
            item.m_sDataType = sl.at(0);
            item.m_sName = sl.at(1);
            item.m_sDescription = sl.at(2);
            item.m_sUnit = sl.at(3);
            item.m_sIOConnect = pDlg->GetIOConnectString();
            item.m_sAlarm = pDlg->GetAlarmString();
            item.m_sArchiveFile = pDlg->GetSaveDiskString();
            pTagIOTableModel->UpdateRow(rowIndex, item);
        }
        delete pDlg;
    } else
#endif
        if(m_strItemName == tr("中间变量")) {
            // 修改选中中间变量
            modifyCurTagTmp();
        }
}

/*
* 插槽：删除变量
*/
void TagManagerWin::onTagDelete()
{
#if 0
    QItemSelectionModel *pItemSelectionModel = m_variableTableView->selectionModel();
    QModelIndexList modelIndexList = pItemSelectionModel->selectedIndexes();
    QMap<int, int> rowMap;
    foreach (QModelIndex index, modelIndexList) {
        rowMap.insert(index.row(), 0);
    }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious()) {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        if(m_strItemName == tr("设备变量")) {
            pTagIOTableModel->removeRow(rowToDel);
        } else if(m_strItemName == tr("中间变量")) {
            pTagTmpTableModel->removeRow(rowToDel);
        }
    }
#endif
    if(m_strItemName == tr("设备变量")) {

    } else if(m_strItemName == tr("中间变量")) {
        // 删除选中中间变量
        deleteCurTagTmp();
    }

}


/*
 * 关闭事件
 */
void TagManagerWin::closeEvent(QCloseEvent *event)
{
    save();
    event->accept();
}

/*
 * 打开文件
 */
void TagManagerWin::open()
{

}

/*
 * 保存文件
 */
void TagManagerWin::save()
{
    if(!m_strItemName.isEmpty()) {
        save(m_strItemName);
    }
}

/*
* 显示大图标
*/
void TagManagerWin::showLargeIcon()
{

}

/*
* 显示小图标
*/
void TagManagerWin::showSmallIcon()
{

}



/**
 * @brief TagManagerWin::initialTableTagSys
 * @details 初始化系统变量表
 */
void TagManagerWin::initialTableTagSys()
{
    QStringList headerLabels;
    headerLabels << tr("ID") << tr("名称") << tr("变量描述") << tr("单位") << tr("工程转换") << tr("备注");
    ui->tableTagSys->setColumnCount(headerLabels.count());
    ui->tableTagSys->setHorizontalHeaderLabels(headerLabels);
    ui->tableTagSys->horizontalHeader()->setSectionsClickable(false);
    ui->tableTagSys->horizontalHeader()->setStretchLastSection(false);
    //ui->tableTagSys->horizontalHeader()->setHighlightSections(true);
    ui->tableTagSys->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableTagSys->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTagSys->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTagSys->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    /*ui->tableTagSys->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background:rgb(72,161,229); "
                "color: rgb(255, 255, 255); "
                "}");*/
    ui->tableTagSys->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableTagSys->horizontalHeader()->show();
    ui->tableTagSys->verticalHeader()->show();
    ui->tableTagSys->setColumnWidth(0, 80);
    ui->tableTagSys->setColumnWidth(1, 160);
    ui->tableTagSys->setColumnWidth(2, 200);
    ui->tableTagSys->setColumnWidth(3, 80);
    ui->tableTagSys->setColumnWidth(4, 200);
    ui->tableTagSys->setColumnWidth(5, 320);

    ui->tableTagSys->setAlternatingRowColors(true);
    ui->tableTagSys->setFocusPolicy(Qt::NoFocus);

    QString styleSheet = "QTableWidget{background-color: rgb(242,242,242);"
                         "alternate-background-color: white;"
                         "color: rgb(0, 0, 0);"
                         "selection-background-color:lightblue;}";

    //ui->tableTagSys->setStyleSheet(styleSheet);
    ui->tableTagSys->clearSelection();
}

/**
 * @brief TagManagerWin::updateTableTagSys
 * @details 刷新系统变量表
 */
void TagManagerWin::updateTableTagSys()
{
    TagSys &tagSys = ProjectData::getInstance()->tagSys_;
    tagSys.load(ProjectData::getInstance()->dbData_);

    ui->tableTagSys->clearContents();
    ui->tableTagSys->setRowCount(0);

    foreach (TagSysDBItem * itemTagSys, tagSys.listTagSysDBItem_) {
        int iRowCount = ui->tableTagSys->rowCount();
        ui->tableTagSys->insertRow(iRowCount);

        QTableWidgetItem *pItemID = new QTableWidgetItem(itemTagSys->m_szTagID);
        ui->tableTagSys->setItem(iRowCount, 0, pItemID);
        QTableWidgetItem *pItemName = new QTableWidgetItem(itemTagSys->m_szName);
        ui->tableTagSys->setItem(iRowCount, 1, pItemName);
        QTableWidgetItem *pItemDescription = new QTableWidgetItem(itemTagSys->m_szDescription);
        ui->tableTagSys->setItem(iRowCount, 2, pItemDescription);
        QTableWidgetItem *pItemUnit = new QTableWidgetItem(itemTagSys->m_szUnit);
        ui->tableTagSys->setItem(iRowCount, 3, pItemUnit);
        QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem(itemTagSys->m_szProjectConverter);
        ui->tableTagSys->setItem(iRowCount, 4, pItemProjectConverter);
        QTableWidgetItem *pItemComments = new QTableWidgetItem(itemTagSys->m_szComments);
        ui->tableTagSys->setItem(iRowCount, 5, pItemComments);
    }

    qDeleteAll(tagSys.listTagSysDBItem_);
    tagSys.listTagSysDBItem_.clear();
}



/**
 * @brief TagManagerWin::initialTableTagTmp
 * @details 初始化中间变量表
 */
void TagManagerWin::initialTableTagTmp()
{
    QStringList headerLabels;
    headerLabels << tr("ID") << tr("名称") << tr("变量描述") << tr("数据类型")
                 << tr("初始值") << tr("最小值") << tr("最大值") << tr("工程转换");

    ui->tableTagTmp->setColumnCount(headerLabels.count());
    ui->tableTagTmp->setHorizontalHeaderLabels(headerLabels);
    ui->tableTagTmp->horizontalHeader()->setSectionsClickable(false);
    ui->tableTagTmp->horizontalHeader()->setStretchLastSection(false);
    //ui->tableTagTmp->horizontalHeader()->setHighlightSections(true);
    ui->tableTagTmp->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableTagTmp->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableTagTmp->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTagTmp->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    /*ui->tableTagTmp->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background:rgb(72,161,229); "
                "color: rgb(255, 255, 255); "
                "}");*/
    ui->tableTagTmp->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableTagTmp->horizontalHeader()->show();
    ui->tableTagTmp->verticalHeader()->show();
    ui->tableTagTmp->setColumnWidth(0, 80);
    ui->tableTagTmp->setColumnWidth(1, 120);
    ui->tableTagTmp->setColumnWidth(2, 200);
    ui->tableTagTmp->setColumnWidth(3, 180);
    ui->tableTagTmp->setColumnWidth(4, 100);
    ui->tableTagTmp->setColumnWidth(5, 100);
    ui->tableTagTmp->setColumnWidth(6, 100);
    ui->tableTagTmp->setColumnWidth(7, 200);

    //ui->tableTagTmp->setAlternatingRowColors(true);
    //ui->tableTagTmp->setFocusPolicy(Qt::NoFocus);

    QString styleSheet = "QTableWidget{background-color: rgb(242,242,242);"
                         "alternate-background-color: white;"
                         "color: rgb(0, 0, 0);"
                         "selection-background-color:lightblue;}";

    //ui->tableTagTmp->setStyleSheet(styleSheet);
    ui->tableTagTmp->clearSelection();
}


/**
 * @brief TagManagerWin::tableTagTmpAddRow
 * @details 中间变量表新增一行
 * @return 新增的行号
 */
int TagManagerWin::tableTagTmpAddRow()
{
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
#define XX(item) \
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    int iRowCount = ui->tableTagTmp->rowCount();
    ui->tableTagTmp->insertRow(iRowCount);

    QTableWidgetItem *pItemID = new QTableWidgetItem("");
    ui->tableTagTmp->setItem(iRowCount, 0, pItemID);
    QTableWidgetItem *pItemName = new QTableWidgetItem("");
    XX(pItemName)
    ui->tableTagTmp->setItem(iRowCount, 1, pItemName);
    QTableWidgetItem *pItemDescription = new QTableWidgetItem("");
    XX(pItemDescription)
    ui->tableTagTmp->setItem(iRowCount, 2, pItemDescription);
    QComboBox* cboPtr = new QComboBox();
    cboPtr->clear();
    cboPtr->addItems(listDataType);
    cboPtr->setCurrentText("");
    ui->tableTagTmp->setCellWidget(iRowCount, 3, cboPtr);
    QTableWidgetItem *pItemInitVal = new QTableWidgetItem("");
    XX(pItemInitVal)
    ui->tableTagTmp->setItem(iRowCount, 4, pItemInitVal);
    QTableWidgetItem *pItemMinVal = new QTableWidgetItem("");
    XX(pItemMinVal)
    ui->tableTagTmp->setItem(iRowCount, 5, pItemMinVal);
    QTableWidgetItem *pItemMaxVal = new QTableWidgetItem("");
    XX(pItemMaxVal)
    ui->tableTagTmp->setItem(iRowCount, 6, pItemMaxVal);
    QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem("");
    ui->tableTagTmp->setItem(iRowCount, 7, pItemProjectConverter);

#undef XX
    return iRowCount;
}


/**
 * @brief TagManagerWin::getTagTmpIdNumValue
 * @details 获取指定行中间变量id数值部分
 * @param iRow 指定行
 * @return 中间变量id数值部分
 */
int TagManagerWin::getTagTmpIdNumValue(int iRow)
{
    int id = 0;
    int iRowCnt = ui->tableTagTmp->rowCount();
    if(iRowCnt > 0) {
        QString szIDText = "";
        QTableWidgetItem *pItemID = ui->tableTagTmp->item(iRow, 0);
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
 * @brief TagManagerWin::updateTableTagTmp
 * @details 刷新中间变量表
 */
void TagManagerWin::updateTableTagTmp()
{
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    tagTmp.load(ProjectData::getInstance()->dbData_);

    ui->tableTagTmp->clearContents();
    ui->tableTagTmp->setRowCount(0);

    foreach (TagTmpDBItem * itemTagTmp, tagTmp.listTagTmpDBItem_) {
        int iRow = tableTagTmpAddRow();
        setTagTmpObjByRow(iRow, itemTagTmp);
    }

    qDeleteAll(tagTmp.listTagTmpDBItem_);
    tagTmp.listTagTmpDBItem_.clear();
}

/**
 * @brief TagManagerWin::saveTableTagTmp
 * @details 保存中间变量表
 */
void TagManagerWin::saveTableTagTmp()
{
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    int iRowCount = ui->tableTagTmp->rowCount();
    for(int i=0; i<iRowCount; i++) {
        TagTmpDBItem * pTagTmp = getTagTmpObjByRow(i);
        tagTmp.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pTagTmp);
        if(pTagTmp != Q_NULLPTR) {
            delete pTagTmp;
            pTagTmp = Q_NULLPTR;
        }
    }

    foreach(QString szTagId, m_listTagTmpDeleteRows) {
        tagTmp.del(ProjectData::getInstance()->dbData_, szTagId);
    }
}

/**
 * @brief TagManagerWin::tagTmpExportToCsv
 * @details 中间变量表变量导出至CSV文件
 * @param path
 */
void TagManagerWin::tagTmpExportToCsv(const QString &path, const QString &/*group*/)
{
    QtCSV::StringData varData;
    int iRowCount = ui->tableTagTmp->rowCount();
    for(int i=0; i<iRowCount; i++) {
        TagTmpDBItem * pTagTmp = getTagTmpObjByRow(iRowCount);

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
 * @brief TagManagerWin::tagTmpImportFromCsv
 * @details 从CSV文件导入中间变量至中间变量表
 * @param path
 */
void TagManagerWin::tagTmpImportFromCsv(const QString &path)
{
    QString filepath = path;

    QList<QStringList> data = QtCSV::Reader::readToList(filepath,
                                                        QString(","),
                                                        QString("\""),
                                                        QTextCodec::codecForName("GB18030"));
    for(int i=0; i<data.size(); i++)
    {
        QStringList row = data.at(i);
        if(row.at(0) == "ID")
            continue;

        TagTmpDBItem * pTagTmp = new TagTmpDBItem();

        pTagTmp->m_szTagID = row.at(0);
        pTagTmp->m_szName = row.at(1);
        pTagTmp->m_szDescription = row.at(2);
        pTagTmp->m_szDataType = row.at(3);
        pTagTmp->m_szInitVal = row.at(4);
        pTagTmp->m_szMinVal = row.at(5);
        pTagTmp->m_szMaxVal = row.at(6);
        pTagTmp->m_szProjectConverter = row.at(7);

        int iRow = tableTagTmpAddRow();
        setTagTmpObjByRow(iRow, pTagTmp);
    }
}

/**
 * @brief TagManagerWin::createTagTmp
 * @details 创建中间变量
 */
void TagManagerWin::createTagTmp()
{
    TagTmpEditDialog *pDlg = new TagTmpEditDialog(this);
    if(pDlg->exec() == QDialog::Accepted) {
        int num = pDlg->createTagNum();
        int iRowCnt = ui->tableTagTmp->rowCount();
        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
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
            id++;
        }
    }
    delete pDlg;
}


/**
 * @brief TagManagerWin::appendTagTmp
 * @details 追加中间变量
 */
void TagManagerWin::appendTagTmp()
{
    int iRowCnt = ui->tableTagTmp->rowCount();
    if(iRowCnt < 1)
        return;

    int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
    TagTmpDBItem * pNewTagTmp = new TagTmpDBItem();
    pNewTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
    pNewTagTmp->m_szName = "";
    pNewTagTmp->m_szDescription = "";
    pNewTagTmp->m_szDataType = "";
    QComboBox *pCbo = dynamic_cast<QComboBox *>(ui->tableTagTmp->cellWidget(iRowCnt - 1, 3));
    if(pCbo != Q_NULLPTR) {
        pNewTagTmp->m_szDataType = pCbo->currentText();
    }
    pNewTagTmp->m_szInitVal = "0";
    pNewTagTmp->m_szMinVal = "";
    pNewTagTmp->m_szMaxVal = "";
    pNewTagTmp->m_szProjectConverter = "";
    int iRow = tableTagTmpAddRow();
    setTagTmpObjByRow(iRow, pNewTagTmp);
}


/**
 * @brief TagManagerWin::getTagTmpObjByRow
 * @details 获取指定行中间变量对象
 * @param iRow 指定行
 * @return 中间变量对象
 */
TagTmpDBItem *TagManagerWin::getTagTmpObjByRow(int iRow)
{
    int iRowCnt = ui->tableTagTmp->rowCount();
    if(iRow < 0 || iRow >= iRowCnt)
        return Q_NULLPTR;

    TagTmpDBItem * pNewTagTmp = new TagTmpDBItem();
    QTableWidgetItem *pItemID = ui->tableTagTmp->item(iRow, 0);
    if(pItemID != Q_NULLPTR) {
        pNewTagTmp->m_szTagID = pItemID->text();
    }
    QTableWidgetItem *pItemName = ui->tableTagTmp->item(iRow, 1);
    if(pItemName != Q_NULLPTR) {
        pNewTagTmp->m_szName = pItemName->text();
    }
    QTableWidgetItem *pItemDescription = ui->tableTagTmp->item(iRow, 2);
    if(pItemDescription != Q_NULLPTR) {
        pNewTagTmp->m_szDescription = pItemDescription->text();
    }
    QComboBox *pCbo = dynamic_cast<QComboBox *>(ui->tableTagTmp->cellWidget(iRow, 3));
    if(pCbo != Q_NULLPTR) {
        pNewTagTmp->m_szDataType = pCbo->currentText();
    }
    QTableWidgetItem *pItemInitVal = ui->tableTagTmp->item(iRow, 4);
    if(pItemInitVal != Q_NULLPTR) {
        pNewTagTmp->m_szInitVal = pItemInitVal->text();
    }
    QTableWidgetItem *pItemMinVal = ui->tableTagTmp->item(iRow, 5);
    if(pItemMinVal != Q_NULLPTR) {
        pNewTagTmp->m_szMinVal = pItemMinVal->text();
    }
    QTableWidgetItem *pItemMaxVal = ui->tableTagTmp->item(iRow, 6);
    if(pItemMaxVal != Q_NULLPTR) {
        pNewTagTmp->m_szMaxVal = pItemMaxVal->text();
    }
    QTableWidgetItem *pItemProjectConverter = ui->tableTagTmp->item(iRow, 7);
    if(pItemProjectConverter != Q_NULLPTR) {
        pNewTagTmp->m_szProjectConverter = pItemProjectConverter->text();
    }

    return pNewTagTmp;
}


/**
 * @brief TagManagerWin::setTagTmpObjByRow
 * @details 设置指定行中间变量对象
 * @param iRow 指定行
 * @param pObj 中间变量对象
 */
void TagManagerWin::setTagTmpObjByRow(int iRow, TagTmpDBItem *pObj)
{
    int iRowCnt = ui->tableTagTmp->rowCount();
    if(pObj == Q_NULLPTR || iRow < 0 || iRow >= iRowCnt)
        return;

    QTableWidgetItem *pItemID = ui->tableTagTmp->item(iRow, 0);
    if(pItemID != Q_NULLPTR) {
        pItemID->setText(pObj->m_szTagID);
    }
    QTableWidgetItem *pItemName = ui->tableTagTmp->item(iRow, 1);
    if(pItemName != Q_NULLPTR) {
        pItemName->setText(pObj->m_szName);
    }
    QTableWidgetItem *pItemDescription = ui->tableTagTmp->item(iRow, 2);
    if(pItemDescription != Q_NULLPTR) {
        pItemDescription->setText(pObj->m_szDescription);
    }
    QComboBox *pCbo = dynamic_cast<QComboBox *>(ui->tableTagTmp->cellWidget(iRow, 3));
    if(pCbo != Q_NULLPTR) {
        pCbo->setCurrentText(pObj->m_szDataType);
    }
    QTableWidgetItem *pItemInitVal = ui->tableTagTmp->item(iRow, 4);
    if(pItemInitVal != Q_NULLPTR) {
        pItemInitVal->setText(pObj->m_szInitVal);
    }
    QTableWidgetItem *pItemMinVal = ui->tableTagTmp->item(iRow, 5);
    if(pItemMinVal != Q_NULLPTR) {
        pItemMinVal->setText(pObj->m_szMinVal);
    }
    QTableWidgetItem *pItemMaxVal = ui->tableTagTmp->item(iRow, 6);
    if(pItemMaxVal != Q_NULLPTR) {
        pItemMaxVal->setText(pObj->m_szMaxVal);
    }
    QTableWidgetItem *pItemProjectConverter = ui->tableTagTmp->item(iRow, 7);
    if(pItemProjectConverter != Q_NULLPTR) {
        pItemProjectConverter->setText(pObj->m_szProjectConverter);
    }
}

/**
 * @brief TagManagerWin::copyCurTagTmp
 * @details 拷贝选中中间变量
 */
void TagManagerWin::copyCurTagTmp()
{
    int iSelectedRow = m_iTableTagTmpSelectedRow;
    if(iSelectedRow >= 0) {
        TagTmpDBItem * pNewTagTmp = getTagTmpObjByRow(iSelectedRow);
        int iRowCnt = ui->tableTagTmp->rowCount();
        int id = getTagTmpIdNumValue(iRowCnt - 1) + 1;
        pNewTagTmp->m_szTagID = QString("tmp.%1").arg(QString::number(id));
        int iRow = tableTagTmpAddRow();
        setTagTmpObjByRow(iRow, pNewTagTmp);
    }
}


/**
 * @brief TagManagerWin::modifyCurTagTmp
 * @details 修改选中中间变量
 */
void TagManagerWin::modifyCurTagTmp()
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

        if(pTagTmp != Q_NULLPTR) {
            delete pTagTmp;
            pTagTmp = Q_NULLPTR;
        }
    }
}


/**
 * @brief TagManagerWin::deleteCurTagTmp
 * @details 删除选中中间变量
 */
void TagManagerWin::deleteCurTagTmp()
{
    m_listTagTmpDeleteRows.clear();
    QList<QTableWidgetItem *> pSelectItems = ui->tableTagTmp->selectedItems();
    QMap<int, int> rowMap;
    foreach (QTableWidgetItem *pItem, pSelectItems) {
        rowMap.insert(pItem->row(), 0);
    }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious()) {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        QTableWidgetItem *pItemID = ui->tableTagTmp->item(rowToDel, 0);
        if(pItemID != Q_NULLPTR) {
            m_listTagTmpDeleteRows << pItemID->text();
        }
        ui->tableTagTmp->removeRow(rowToDel);
    }
}


/**
 * @brief TagManagerWin::initialTableTagIO
 * @details 初始化设备变量表
 */
void TagManagerWin::initialTableTagIO()
{
    QStringList headerLabels;
    headerLabels << tr("ID") << tr("名称") << tr("变量描述") << tr("设备名")
                 << tr("设备地址") << tr("寄存器区") << tr("寄存器地址") << tr("偏移地址")
                 << tr("读写类型") << tr("数据类型") << tr("初始值") << tr("最小值")
                 << tr("最大值") << tr("变比") << tr("工程转换");

    ui->tableTagIO->setColumnCount(headerLabels.count());
    ui->tableTagIO->setHorizontalHeaderLabels(headerLabels);
    ui->tableTagIO->horizontalHeader()->setSectionsClickable(false);
    ui->tableTagIO->horizontalHeader()->setStretchLastSection(true);
    //ui->tableTagIO->horizontalHeader()->setHighlightSections(true);
    ui->tableTagIO->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableTagIO->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTagIO->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTagIO->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    /*ui->tableTagIO->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background:rgb(72,161,229); "
                "color: rgb(255, 255, 255); "
                "}");*/
    ui->tableTagIO->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableTagIO->horizontalHeader()->show();
    ui->tableTagIO->verticalHeader()->show();
    ui->tableTagIO->setColumnWidth(0, 80);
    ui->tableTagIO->setColumnWidth(1, 120);
    ui->tableTagIO->setColumnWidth(2, 200);
    ui->tableTagIO->setColumnWidth(3, 120);
    ui->tableTagIO->setColumnWidth(4, 80);
    ui->tableTagIO->setColumnWidth(5, 80);
    ui->tableTagIO->setColumnWidth(6, 80);
    ui->tableTagIO->setColumnWidth(7, 80);
    ui->tableTagIO->setColumnWidth(8, 80);
    ui->tableTagIO->setColumnWidth(9, 140);
    ui->tableTagIO->setColumnWidth(10, 80);
    ui->tableTagIO->setColumnWidth(11, 80);
    ui->tableTagIO->setColumnWidth(12, 80);
    ui->tableTagIO->setColumnWidth(13, 80);
    ui->tableTagIO->setColumnWidth(14, 200);

    ui->tableTagIO->setAlternatingRowColors(true);
    ui->tableTagIO->setFocusPolicy(Qt::NoFocus);

    QString styleSheet = "QTableWidget{background-color: rgb(242,242,242);"
                         "alternate-background-color: white;"
                         "color: rgb(0, 0, 0);"
                         "selection-background-color:lightblue;}";

    //ui->tableTagIO->setStyleSheet(styleSheet);
    ui->tableTagIO->clearSelection();
}


/**
 * @brief TagManagerWin::updateTableTagIO
 * @details 刷新设备变量表
 * @param szGroupName 变量组名称 如："IO设备[缺省]"
 */
void TagManagerWin::updateTableTagIO(const QString &szGroupName)
{
    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    QString szGroup = tagIO.getGroupNameByShowName(ProjectData::getInstance()->dbData_,
                                                   szGroupName);
    if(szGroup == QString()) return;
    tagIO.load(ProjectData::getInstance()->dbData_);

    ui->tableTagIO->clearContents();
    ui->tableTagIO->setRowCount(0);

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

    foreach (TagIODBItem * itemTagIO, tagIO.listTagIODBItem_) {
        int iRowCount = ui->tableTagIO->rowCount();
        ui->tableTagIO->insertRow(iRowCount);

        QTableWidgetItem *pItemID = new QTableWidgetItem(itemTagIO->m_szTagID);
        ui->tableTagIO->setItem(iRowCount, 0, pItemID);
        QTableWidgetItem *pItemName = new QTableWidgetItem(itemTagIO->m_szName);
        ui->tableTagIO->setItem(iRowCount, 1, pItemName);
        QTableWidgetItem *pItemDescription = new QTableWidgetItem(itemTagIO->m_szDescription);
        ui->tableTagIO->setItem(iRowCount, 2, pItemDescription);

        QComboBox *cboDeviceNamePtr = new QComboBox();
        cboDeviceNamePtr->clear();
        cboDeviceNamePtr->addItems(QStringList()); // TODO
        cboDeviceNamePtr->setCurrentText(itemTagIO->m_szDeviceName);
        ui->tableTagIO->setCellWidget(iRowCount, 3, cboDeviceNamePtr);

        QTableWidgetItem *pItemDeviceAddr = new QTableWidgetItem(itemTagIO->m_szDeviceAddr);
        ui->tableTagIO->setItem(iRowCount, 4, pItemDeviceAddr);

        QComboBox *cboRegisterAreaPtr = new QComboBox();
        cboRegisterAreaPtr->clear();
        cboRegisterAreaPtr->addItems(QStringList()); // TODO
        cboRegisterAreaPtr->setCurrentText(itemTagIO->m_szRegisterArea);
        ui->tableTagIO->setCellWidget(iRowCount, 5, cboRegisterAreaPtr);

        QTableWidgetItem *pItemRegisterAddr = new QTableWidgetItem(itemTagIO->m_szRegisterAddr);
        ui->tableTagIO->setItem(iRowCount, 6, pItemRegisterAddr);

        QTableWidgetItem *pItemAddrOffset = new QTableWidgetItem(itemTagIO->m_szAddrOffset);
        ui->tableTagIO->setItem(iRowCount, 7, pItemAddrOffset);

        QComboBox *cboReadWriteTypePtr = new QComboBox();
        cboReadWriteTypePtr->clear();
        cboReadWriteTypePtr->addItems(QStringList()); // TODO
        cboReadWriteTypePtr->setCurrentText(itemTagIO->m_szReadWriteType);
        ui->tableTagIO->setCellWidget(iRowCount, 8, cboReadWriteTypePtr);

        QComboBox *cboDataTypePtr = new QComboBox();
        cboDataTypePtr->clear();
        cboDataTypePtr->addItems(QStringList()); // TODO
        cboDataTypePtr->setCurrentText(itemTagIO->m_szDataType);
        ui->tableTagIO->setCellWidget(iRowCount, 9, cboDataTypePtr);

        QTableWidgetItem *pItemInitVal = new QTableWidgetItem(itemTagIO->m_szInitVal);
        ui->tableTagIO->setItem(iRowCount, 10, pItemInitVal);

        QTableWidgetItem *pItemMinVal = new QTableWidgetItem(itemTagIO->m_szMinVal);
        ui->tableTagIO->setItem(iRowCount, 11, pItemMinVal);
        QTableWidgetItem *pItemMaxVal = new QTableWidgetItem(itemTagIO->m_szMaxVal);
        ui->tableTagIO->setItem(iRowCount, 12, pItemMaxVal);
        QTableWidgetItem *pItemScale = new QTableWidgetItem(itemTagIO->m_szScale);
        ui->tableTagIO->setItem(iRowCount, 13, pItemScale);
        QTableWidgetItem *pItemProjectConverter = new QTableWidgetItem(itemTagIO->m_szProjectConverter);
        ui->tableTagIO->setItem(iRowCount, 14, pItemProjectConverter);
    }

    qDeleteAll(tagIO.listTagIODBItem_);
    tagIO.listTagIODBItem_.clear();
}


/**
 * @brief TagManagerWin::saveTableTagIO
 * @details 保存设备变量表
 */
void TagManagerWin::saveTableTagIO()
{
    int iRowCount = ui->tableTagIO->rowCount();
    for(int i=0; i<iRowCount; i++) {
        TagIODBItem * pTagTmp = new TagIODBItem();
        QTableWidgetItem *pItemID = ui->tableTagIO->item(i, 0);
        if(pItemID != Q_NULLPTR) {
            pTagTmp->m_szTagID = pItemID->text();
        }
        QTableWidgetItem *pItemName = ui->tableTagIO->item(i, 1);
        if(pItemName != Q_NULLPTR) {
            pTagTmp->m_szName = pItemName->text();
        }
        QTableWidgetItem *pItemDescription = ui->tableTagIO->item(i, 2);
        if(pItemDescription != Q_NULLPTR) {
            pTagTmp->m_szDescription = pItemDescription->text();
        }
        QComboBox *pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(i, 3));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szDeviceName = pCbo->currentText();
        }
        QTableWidgetItem *pItemDeviceAddr = ui->tableTagIO->item(i, 4);
        if(pItemDeviceAddr != Q_NULLPTR) {
            pTagTmp->m_szDeviceAddr = pItemDeviceAddr->text();
        }
        pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(i, 5));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szRegisterArea = pCbo->currentText();
        }
        QTableWidgetItem *pItemRegisterAddr = ui->tableTagIO->item(i, 6);
        if(pItemRegisterAddr != Q_NULLPTR) {
            pTagTmp->m_szRegisterAddr = pItemRegisterAddr->text();
        }
        QTableWidgetItem *pItemAddrOffset = ui->tableTagIO->item(i, 7);
        if(pItemAddrOffset != Q_NULLPTR) {
            pTagTmp->m_szAddrOffset = pItemAddrOffset->text();
        }
        pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(i, 8));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szReadWriteType = pCbo->currentText();
        }
        pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(i, 9));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szDataType = pCbo->currentText();
        }
        QTableWidgetItem *pItemInitVal = ui->tableTagIO->item(i, 10);
        if(pItemInitVal != Q_NULLPTR) {
            pTagTmp->m_szInitVal = pItemInitVal->text();
        }
        QTableWidgetItem *pItemMinVal = ui->tableTagIO->item(i, 11);
        if(pItemMinVal != Q_NULLPTR) {
            pTagTmp->m_szMinVal = pItemMinVal->text();
        }
        QTableWidgetItem *pItemMaxVal = ui->tableTagIO->item(i, 12);
        if(pItemMaxVal != Q_NULLPTR) {
            pTagTmp->m_szMaxVal = pItemMaxVal->text();
        }
        QTableWidgetItem *pItemScale = ui->tableTagIO->item(i, 13);
        if(pItemScale != Q_NULLPTR) {
            pTagTmp->m_szScale = pItemScale->text();
        }
        QTableWidgetItem *pItemProjectConverter = ui->tableTagIO->item(i, 14);
        if(pItemProjectConverter != Q_NULLPTR) {
            pTagTmp->m_szProjectConverter = pItemProjectConverter->text();
        }

        if(pTagTmp != Q_NULLPTR) {
            delete pTagTmp;
            pTagTmp = Q_NULLPTR;
        }
    }
}


/**
 * @brief TagManagerWin::tagIOExportToCsv
 * @details 设备变量表变量导出至CSV文件
 * @param path
 */
void TagManagerWin::tagIOExportToCsv(const QString &path, const QString &group)
{
    QtCSV::StringData varData;
    int iRowCount = ui->tableTagIO->rowCount();
    for(int i=0; i<iRowCount; i++) {
        TagIODBItem * pTagTmp = new TagIODBItem();
        QTableWidgetItem *pItemID = ui->tableTagIO->item(iRowCount, 0);
        if(pItemID != Q_NULLPTR) {
            pTagTmp->m_szTagID = pItemID->text();
        }
        QTableWidgetItem *pItemName = ui->tableTagIO->item(iRowCount, 1);
        if(pItemName != Q_NULLPTR) {
            pTagTmp->m_szName = pItemName->text();
        }
        QTableWidgetItem *pItemDescription = ui->tableTagIO->item(iRowCount, 2);
        if(pItemDescription != Q_NULLPTR) {
            pTagTmp->m_szDescription = pItemDescription->text();
        }
        QComboBox *pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(iRowCount, 3));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szDeviceName = pCbo->currentText();
        }
        QTableWidgetItem *pItemDeviceAddr = ui->tableTagIO->item(iRowCount, 4);
        if(pItemDeviceAddr != Q_NULLPTR) {
            pTagTmp->m_szDeviceAddr = pItemDeviceAddr->text();
        }
        pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(iRowCount, 5));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szRegisterArea = pCbo->currentText();
        }
        QTableWidgetItem *pItemRegisterAddr = ui->tableTagIO->item(iRowCount, 6);
        if(pItemRegisterAddr != Q_NULLPTR) {
            pTagTmp->m_szRegisterAddr = pItemRegisterAddr->text();
        }
        QTableWidgetItem *pItemAddrOffset = ui->tableTagIO->item(iRowCount, 7);
        if(pItemAddrOffset != Q_NULLPTR) {
            pTagTmp->m_szAddrOffset = pItemAddrOffset->text();
        }
        pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(iRowCount, 8));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szReadWriteType = pCbo->currentText();
        }
        pCbo = dynamic_cast<QComboBox *>(ui->tableTagIO->cellWidget(iRowCount, 9));
        if(pCbo != Q_NULLPTR) {
            pTagTmp->m_szDataType = pCbo->currentText();
        }
        QTableWidgetItem *pItemInitVal = ui->tableTagIO->item(iRowCount, 10);
        if(pItemInitVal != Q_NULLPTR) {
            pTagTmp->m_szInitVal = pItemInitVal->text();
        }
        QTableWidgetItem *pItemMinVal = ui->tableTagIO->item(iRowCount, 11);
        if(pItemMinVal != Q_NULLPTR) {
            pTagTmp->m_szMinVal = pItemMinVal->text();
        }
        QTableWidgetItem *pItemMaxVal = ui->tableTagIO->item(iRowCount, 12);
        if(pItemMaxVal != Q_NULLPTR) {
            pTagTmp->m_szMaxVal = pItemMaxVal->text();
        }
        QTableWidgetItem *pItemScale = ui->tableTagIO->item(iRowCount, 13);
        if(pItemScale != Q_NULLPTR) {
            pTagTmp->m_szScale = pItemScale->text();
        }
        QTableWidgetItem *pItemProjectConverter = ui->tableTagIO->item(iRowCount, 14);
        if(pItemProjectConverter != Q_NULLPTR) {
            pTagTmp->m_szProjectConverter = pItemProjectConverter->text();
        }

        pTagTmp->m_szGroupName = m_IOVariableListWhat;

        QStringList varRow;
        varRow << pTagTmp->m_szTagID << pTagTmp->m_szGroupName << pTagTmp->m_szName<< pTagTmp->m_szDescription
               << pTagTmp->m_szDeviceName << pTagTmp->m_szDeviceAddr << pTagTmp->m_szRegisterArea << pTagTmp->m_szRegisterAddr
               << pTagTmp->m_szAddrOffset << pTagTmp->m_szReadWriteType << pTagTmp->m_szDataType << pTagTmp->m_szInitVal
               << pTagTmp->m_szMinVal << pTagTmp->m_szMaxVal << pTagTmp->m_szScale << pTagTmp->m_szProjectConverter;

        varData.addRow(varRow);

        if(pTagTmp != Q_NULLPTR) {
            delete pTagTmp;
            pTagTmp = Q_NULLPTR;
        }
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
 * @brief TagManagerWin::tagIOImportFromCsv
 * @details 从CSV文件导入设备变量至设备变量表
 * @param path
 */
void TagManagerWin::tagIOImportFromCsv(const QString &path)
{
    QString filepath = path;

    QList<QStringList> data = QtCSV::Reader::readToList(filepath,
                                                        QString(","),
                                                        QString("\""),
                                                        QTextCodec::codecForName("GB18030"));
    for(int i=0; i<data.size(); i++)
    {
        QStringList row = data.at(i);
        if(row.at(0) == "ID")
            continue;

        TagIODBItem * pTagIO = new TagIODBItem();

        pTagIO->m_szTagID = row.at(0);
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

        TagIO &tagIO = ProjectData::getInstance()->tagIO_;
        tagIO.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pTagIO);

        if(pTagIO != Q_NULLPTR) {
            delete pTagIO;
            pTagIO = Q_NULLPTR;
        }
    }

    // TOTO
    // 变量组不存在是创建变量组

    this->updateTableTagIO(m_IOVariableListWhat);
}


void TagManagerWin::on_tableTagIO_itemPressed(QTableWidgetItem *item)
{
    m_iTableTagIOSelectedRow = item->row();
    ui->tableTagIO->editItem(item);
}

void TagManagerWin::on_tableTagTmp_itemPressed(QTableWidgetItem *item)
{
    int iColumn = item->column();
    int iRow = item->row();
    m_iTableTagTmpSelectedRow = iRow;
    if(iColumn > 0 && iColumn < 7 && iColumn != 3) {
        ui->tableTagTmp->editItem(item);
    }
}

/**
 * @brief TagManagerWin::on_tableTagTmp_itemDoubleClicked
 * @details 中间变量表视图项双击处理
 * @param item
 */
void TagManagerWin::on_tableTagTmp_itemDoubleClicked(QTableWidgetItem *item)
{
    int iColumn = item->column();
    int iRow = item->row();

    if(iColumn > 0 && iColumn < 7) {
        // 修改选中中间变量
        modifyCurTagTmp();
    } else if(iColumn == 7) {
        TagTmpDBItem *pTagTmp = getTagTmpObjByRow(iRow);
        TagFuncEditDialog *pDlg = new TagFuncEditDialog(ProjectMgrUtils::getProjectPath(m_strProjectName), this);
        pDlg->SetData(pTagTmp->m_szProjectConverter);
        if(pDlg->exec() == QDialog::Accepted) {
            pTagTmp->m_szProjectConverter = pDlg->GetData();
            setTagTmpObjByRow(iRow, pTagTmp);
        }
        delete pDlg;
        if(pTagTmp != Q_NULLPTR) {
            delete pTagTmp;
            pTagTmp = Q_NULLPTR;
        }
    }
}

void TagManagerWin::on_tableTagIO_itemDoubleClicked(QTableWidgetItem *item)
{
#if 0
    QStringList sl;
    int rowIndex = index.row();
    int columnIndex = index.column();

    if(m_strItemName == tr("设备变量")) {
        TagIOItem item = pTagIOTableModel->GetRow(rowIndex);
        if(columnIndex == pTagIOTableModel->Column::IOConnect ||
                columnIndex == pTagIOTableModel->Column::Alarm ||
                columnIndex == pTagIOTableModel->Column::ArchiveFile) {
            VariableEditDialog *pDlg = new VariableEditDialog(m_strProjectName, this);
            pDlg->setWindowTitle(tr("编辑设备变量"));
            sl << item.m_sDataType << item.m_sName << item.m_sDescription << item.m_sUnit;

            // 单元格数据有可能已经改变
            if(item.m_sDataType == tr("模拟量"))
                pDlg->SetVariableType(VariableEditDialog::AI);
            else if(item.m_sDataType == tr("数字量"))
                pDlg->SetVariableType(VariableEditDialog::DI);
            else
                pDlg->SetVariableType(VariableEditDialog::NONE);
            pDlg->SetBasicSetting(sl);

            // Removes the tab at position index from this stack of widgets.
            // The page widget itself is not deleted.
            pDlg->RemoveTab(0); // 隐藏基本设置页
            pDlg->RemoveTab(0); // 隐藏数据属性页
            if(columnIndex == pTagIOTableModel->Column::IOConnect)
                pDlg->SetCurrentTabIndex(4-2); // IO连接页
            else if(columnIndex == pTagIOTableModel->Column::Alarm)
                pDlg->SetCurrentTabIndex(3-2); // 报警页
            else if(columnIndex == pTagIOTableModel->Column::ArchiveFile)
                pDlg->SetCurrentTabIndex(2-2); // 存盘页

            pDlg->SetIOConnectString(item.m_sIOConnect);
            pDlg->SetAlarmString(item.m_sAlarm);
            pDlg->SetSaveDiskString(item.m_sArchiveFile);

            if(pDlg->exec() == QDialog::Accepted) {
                QStringList sl = pDlg->GetBasicSetting();
                item.m_sDataType = sl.at(0);
                item.m_sName = sl.at(1);
                item.m_sDescription = sl.at(2);
                item.m_sUnit = sl.at(3);
                item.m_sIOConnect = pDlg->GetIOConnectString();
                item.m_sAlarm = pDlg->GetAlarmString();
                item.m_sArchiveFile = pDlg->GetSaveDiskString();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        } else if(columnIndex == pTagIOTableModel->Column::ProjectConverter) {
            TagFuncEditDialog *pDlg = new TagFuncEditDialog(ProjectMgrUtils::getProjectPath(m_strProjectName), this);
            pDlg->SetData(item.m_sProjectConverter);
            if(pDlg->exec() == QDialog::Accepted) {
                item.m_sProjectConverter = pDlg->GetData();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        } else if(columnIndex == pTagIOTableModel->Column::Comments) {
            CommentsDialog *pDlg = new CommentsDialog(this);
            pDlg->setCommentsText(item.m_sComments);
            if(pDlg->exec() == QDialog::Accepted) {
                item.m_sComments = pDlg->getCommentsText();
                pTagIOTableModel->UpdateRow(rowIndex, item);
            }
            delete pDlg;
        }
    } else
#endif
}


