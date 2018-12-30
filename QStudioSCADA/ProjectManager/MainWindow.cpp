#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "NewProjectDialog.h"
#include "CommunicationDeviceWin.h"
#include "ProjectDownloadDialog.h"
#include "ProjectUploadDialog.h"
#include "VariableManagerWin.h"
#include "RealTimeDatabaseWin.h"
#include "NewVariableGroupDialog.h"
#include "DrawPageWin.h"
#include "ScriptManageWin.h"
#include "configutils.h"
#include "Helper.h"
#include "AboutDialog.h"

#include <QDialog>
#include <QCloseEvent>
#include <QSettings>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>
#include <QStandardItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QIcon>
#include <QProcess>
#include <QStringList>
#include <QTime>
#include <QMdiSubWindow>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_strProjectPath(""),
    m_CurTreeViewItem(""),
    m_CurItem("")
{
    ui->setupUi(this);   
    enableToolBar(""); // 工具条使能
    setContextMenuPolicy(Qt::DefaultContextMenu); // 右键菜单生效
    readSettings(); // 初始窗口时读取窗口设置信息
    initWindow(); // 初始化窗口
    setUpProjectTreeView();
    loadRecentProjectList();
    on_actionBigIcon_triggered(); // 大图标显示
}

MainWindow::~MainWindow()
{
    delete pTreeViewProjectModel;
    pTreeViewProjectModel = NULL;
    delete ui;
}

// 工程管理器ui初始化
void MainWindow::setUpProjectTreeView()
{
    ui->treeViewProject->setHeaderHidden(true);

    pTreeViewProjectModel = new QStandardItemModel();
    pProjectItem = new QStandardItem(QIcon(":/images/pj_pro.png"), tr("未创建工程"));
    pProjectItem->setEditable(false);
    pSystemParameters = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("系统参数"));
    pSystemParameters->setEditable(false);
    pProjectItem->appendRow(pSystemParameters);

    //////////////////////////////////////////////////////

    pCommunicationDevice = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("通讯设备"));
    pCommunicationDevice->setEditable(false);
    pComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), tr("串口设备"));
    pComDevice->setEditable(false);
    pCommunicationDevice->appendRow(pComDevice);
    pNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), tr("网络设备"));
    pNetDevice->setEditable(false);
    pCommunicationDevice->appendRow(pNetDevice);
    /*
    pBusDevice = new QStandardItem(QIcon(":/images/pj_bus.png"), tr("总线设备"));
    pBusDevice->setEditable(false);
    pCommunicationDevice->appendRow(pBusDevice);
    pOPCDevice = new QStandardItem(QIcon(":/images/pj_opc.png"), tr("OPC设备"));
    pOPCDevice->setEditable(false);
    pCommunicationDevice->appendRow(pOPCDevice);
    */
    pProjectItem->appendRow(pCommunicationDevice);

    pDataBaseConfig = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("变量管理"));
    pDataBaseConfig->setEditable(false);
    pDevVariable = new QStandardItem(QIcon(":/images/pj_zone.png"), tr("设备变量"));
    pDevVariable->setEditable(false);

    pDataBaseConfig->appendRow(pDevVariable);
    pTmpVariable = new QStandardItem(QIcon(":/images/pj_zone.png"), tr("中间变量"));
    pTmpVariable->setEditable(false);
    pDataBaseConfig->appendRow(pTmpVariable);
    pSysVariable = new QStandardItem(QIcon(":/images/pj_zone.png"), tr("系统变量"));
    pSysVariable->setEditable(false);
    pDataBaseConfig->appendRow(pSysVariable);
    pProjectItem->appendRow(pDataBaseConfig);

    pDataBaseManager = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("数据库管理"));
    pDataBaseManager->setEditable(false);
    pRealTimeDatabase = new QStandardItem(QIcon(":/images/db_rtdbview.png"), tr("实时数据库"));
    pRealTimeDatabase->setEditable(false);
    pDataBaseManager->appendRow(pRealTimeDatabase);
    pHistoryDatabase = new QStandardItem(QIcon(":/images/db_hisdbview.png"), tr("历史数据库"));
    pHistoryDatabase->setEditable(false);
    pDataBaseManager->appendRow(pHistoryDatabase);
    pProjectItem->appendRow(pDataBaseManager);

    pDrawPage = new QStandardItem(QIcon(":/images/pj_draw.png"), tr("画面"));
    pDrawPage->setEditable(false);
    pProjectItem->appendRow(pDrawPage);

    pLogicProgram = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("逻辑编程"));
    pLogicProgram->setEditable(false);
    /*
    pLadderEditor = new QStandardItem(QIcon(":/images/pj_plc.png"), tr("梯形图"));
    pLadderEditor->setEditable(false);
    pLogicProgram->appendRow(pLadderEditor);
    */
    pScriptEditor = new QStandardItem(QIcon(":/images/pj_script.png"), tr("脚本编辑器"));
    pScriptEditor->setEditable(false);
    pLogicProgram->appendRow(pScriptEditor);
    pProjectItem->appendRow(pLogicProgram);

    /*
    pSystemTool = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("系统工具"));
    pSystemTool->setEditable(false);
    pDriveMonitor = new QStandardItem(QIcon(":/images/pj_drive.png"), tr("驱动监视"));
    pDriveMonitor->setEditable(false);
    pSystemTool->appendRow(pDriveMonitor);
    pDatabaseInterface = new QStandardItem(QIcon(":/images/pj_mysql.png"), tr("数据库接口"));
    pDatabaseInterface->setEditable(false);
    pSystemTool->appendRow(pDatabaseInterface);
    pBuilderXMLInterface = new QStandardItem(QIcon(":/images/pj_xml.png"), tr("建筑XML接口"));
    pBuilderXMLInterface->setEditable(false);
    pSystemTool->appendRow(pBuilderXMLInterface);
    pProjectItem->appendRow(pSystemTool);
    */

    pTreeViewProjectModel->appendRow(pProjectItem);
    ui->treeViewProject->setModel(pTreeViewProjectModel);
    ui->treeViewProject->expandAll();
}


ChildForm* MainWindow::activeMdiChild() //活动窗口
{
    // 如果有活动窗口，则将其内的中心部件转换为MdiChild类型
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
    {
        return qobject_cast<ChildForm *>(activeSubWindow->widget());
    }
    return NULL; // 没有活动窗口，直接返回0
}

void MainWindow::setActiveSubWindow(ChildForm *window)
{
    if(!window)
        return;
    window->showMaximized();
    m_CurItem = window->windowTitle();
    ui->mdiArea->setActiveSubWindow(0);//Activates the subwindow window. If window is 0, any current active window is deactivated.
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

ChildForm* MainWindow::getActiveSubWindow()
{
    return qobject_cast<ChildForm*>(ui->mdiArea->activeSubWindow()->widget());
}

ChildForm* MainWindow::findMdiChild(const QString &windowTitle)
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        ChildForm *pChildWin = qobject_cast<ChildForm *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return pChildWin;
    }
    return NULL;
}


QMdiSubWindow* MainWindow::findMdiSubWindow(const QString &windowTitle)
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        ChildBase * pChildWin = qobject_cast<ChildBase *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return window;
    }
    return NULL;
}

/*
* 新建工程时，创建缺省IO变量组
*/
void MainWindow::CreateDefaultIOTagGroup()
{
    if(pDevVariable->rowCount() == 0)
    {
        DBVarGroup *pGroup = new DBVarGroup();
        pGroup->m_type = "WorkNode";
        pGroup->m_name = QString(tr("IO设备[缺省]"));
        pGroup->m_iPageID = m_pIoDBVarGroups->m_VarBlockGroupList.count();
        m_pIoDBVarGroups->m_VarBlockGroupList.append(pGroup);
        m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
        UpdateDeviceVariableTableGroup();
    }
}

/*
* 增加组
*/
void MainWindow::VariableGroupAdd()
{
    NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
    pDlg->SetDialogName("新建数据组");
    pDlg->SetLabelName("数据组名：");
    if(pDlg->exec() == QDialog::Accepted)
    {
        DBVarGroup *pGroup = new DBVarGroup();
        pGroup->m_type = "WorkNode";
        pGroup->m_name = pDlg->GetGroupName();
        pGroup->m_iPageID = m_pIoDBVarGroups->m_VarBlockGroupList.count();
        m_pIoDBVarGroups->m_VarBlockGroupList.append(pGroup);
        m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
        UpdateDeviceVariableTableGroup();
    }
}

/*
* 重命名组
*/
void MainWindow::VariableGroupRename()
{
#if 0
    QModelIndex index = ui->treeViewProject->currentIndex();
    QString text = this->pTreeViewProjectModel->itemFromIndex(index)->text();
    QString titleOld = QString("%1%2%3").arg("设备变量").arg("-").arg(text);
    ChildBase* window = findMdiChild(titleOld);

    foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        if(text == var->m_name)
        {
            NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
            pDlg->SetDialogName("新建数据组");
            pDlg->SetLabelName("数据组名：");
            pDlg->SetGroupName(text);
            if(pDlg->exec() == QDialog::Accepted)
            {
                if(window != NULL)
                {
                    VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
                    QString titleNew = QString("%1%2%3").arg("设备变量").arg("-").arg(pDlg->GetGroupName());
                    pVariableManagerWin->SetTitle(titleNew);
                }
                else
                    qDebug()<<"window == NULL";
                QString srcfile, desfile;
                srcfile = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + var->m_name + ".odb";
                var->m_name = pDlg->GetGroupName();
                desfile = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + var->m_name + ".odb";
                QFile::rename(srcfile ,desfile);
                m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
                UpdateDeviceVariableTableGroup();
            }
        }
    }
#endif
}

/*
* 删除组
*/
void MainWindow::VariableDeleteGroup()
{
#if 0
    QModelIndex ModelIndex = ui->treeViewProject->selectionModel()->currentIndex();
    QStandardItem *qTiem = pTreeViewProjectModel->itemFromIndex(ModelIndex);

    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        if(qTiem->text() == var->m_name)
        {
            QMdiSubWindow* pWindow = findMdiSubWindow("设备变量-"+var->m_name);
            if(pWindow != NULL)
            {
                pWindow->close();
            }
            QString file = "";
            file = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + var->m_name + ".odb";
            QFile delFile(file);
            if(delFile.exists())
            {
                delFile.remove();
            }
            m_pIoDBVarGroups->m_VarBlockGroupList.removeOne(var);
            break;
        }
    }
    m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
    UpdateDeviceVariableTableGroup();
#endif
}

/*
* 复制组
*/
void MainWindow::VariableGroupCopy()
{
#if 0
    QModelIndex ModelIndex = ui->treeViewProject->selectionModel()->currentIndex();
    QStandardItem *qTiem = pTreeViewProjectModel->itemFromIndex(ModelIndex);

    // check the same name first
    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        if(var->m_name == QString("复制_%1").arg(qTiem->text()))
        {
            QMessageBox::information(this, "系统提示", "同名文件存在，请先修改名称！");
            return;
        }
    }

    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        if(qTiem->text() == var->m_name)
        {
            DBVarGroup *newVar = new DBVarGroup();
            newVar->m_type = var->m_type;
            newVar->m_name = QString("复制_%1").arg(var->m_name);
            QString srcfile, desfile;
            srcfile = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + var->m_name + ".odb";
            desfile = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + newVar->m_name + ".odb";
            QFile::copy(srcfile ,desfile);
            m_pIoDBVarGroups->m_VarBlockGroupList.append(newVar);
            m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
            UpdateDeviceVariableTableGroup();
            break;
        }
    }
#endif
}

/*
* 修改ID
*/
void MainWindow::VariableModifyGroupID()
{
#if 0
    QModelIndex index = ui->treeViewProject->currentIndex();
    QString text = this->pTreeViewProjectModel->itemFromIndex(index)->text();
    foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        if(text == var->m_name)
        {
            NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
            pDlg->SetDialogName("设置设备ID");
            pDlg->SetLabelName("设备ID：");
            if(pDlg->exec() == QDialog::Accepted)
            {
                //id = pDlg->GetGroupName();
                //m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
                UpdateDeviceVariableTableGroup();
            }
        }
    }
#endif
}

/*
* 右键菜单
*/
void MainWindow::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    bool found = false;

    QModelIndex index = ui->treeViewProject->currentIndex();
    m_CurTreeViewItem = this->pTreeViewProjectModel->itemFromIndex(index)->text();

    if(m_CurTreeViewItem == "设备变量")
        found = true;

    foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        if(m_CurTreeViewItem == var->m_name)
            found = true;
    }

    if(!found)
        return;

    QMenu *pMenu = new QMenu(this);

    QAction *pAddGroupAct = new QAction(tr("增加组"), this);
    pAddGroupAct->setStatusTip(tr("增加组"));
    connect(pAddGroupAct, SIGNAL(triggered()), this, SLOT(VariableGroupAdd()));
    pMenu->addAction(pAddGroupAct);

    QAction *pRenameGroupAct = new QAction(tr("重命名"), this);
    pRenameGroupAct->setStatusTip(tr("重命名"));
    connect(pRenameGroupAct, SIGNAL(triggered()), this, SLOT(VariableGroupRename()));
    pMenu->addAction(pRenameGroupAct);

    QAction *pDeleteGroupAct = new QAction(tr("删除组"), this);
    pDeleteGroupAct->setStatusTip(tr("删除组"));
    connect(pDeleteGroupAct, SIGNAL(triggered()), this, SLOT(VariableDeleteGroup()));
    pMenu->addAction(pDeleteGroupAct);

    QAction *pCopyGroupAct = new QAction(tr("复制组"), this);
    pCopyGroupAct->setStatusTip(tr("复制组"));
    connect(pCopyGroupAct, SIGNAL(triggered()), this, SLOT(VariableGroupCopy()));
    pMenu->addAction(pCopyGroupAct);

    QAction *pModifyGroupIDAct = new QAction(tr("修改ID"), this);
    pModifyGroupIDAct->setStatusTip(tr("修改ID"));
    connect(pModifyGroupIDAct, SIGNAL(triggered()), this, SLOT(VariableModifyGroupID()));
    pMenu->addAction(pModifyGroupIDAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}

void MainWindow::closeEvent(QCloseEvent *event) // 关闭事件
{
    //qDebug()<<"MainWindow::closeEvent()";
    QString strFile = ConfigUtils::AppDir() + "/lastpath.ini";
    if(pProjectItem->text() != tr("未创建工程"))
        ConfigUtils::SetCfgStr(strFile, "PathInfo", "Path", m_strProjectPath);
    ui->mdiArea->closeAllSubWindows(); // 先执行多文档区域的关闭操作
    writeSettings(); // 在关闭前写入窗口设置
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore(); // 如果还有窗口没有关闭，则忽略该事件
    } else {
        event->accept();
    }
}

void MainWindow::writeSettings() // 写入窗口设置
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::readSettings() // 读取窗口设置
{
    //qDebug()<<QCoreApplication::organizationName() << QCoreApplication::applicationName();
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() * 3 / 4, availableGeometry.height() * 3 / 4);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
}

void MainWindow::initWindow() // 初始化窗口
{
    setCentralWidget(ui->mdiArea);
//    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("QStudioSCADA跨平台自动化软件"));


    // 当多文档区域的内容超出可视区域后，出现滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->statusBar->showMessage(tr("欢迎使用SCADA组态系统"));




}



void MainWindow::on_actionNewPoject_triggered()
{
    if(pProjectItem->text() != "未创建工程") {
        QMessageBox::information(this, "系统提示", "工程文件已建立，请手动关闭当前工程文件后重新建立！");
        return;
    }

    NewProjectDialog *pNewProjectDlg = new NewProjectDialog(this);
    if(pNewProjectDlg->exec() == QDialog::Accepted) {
        UpdateProjectName(pNewProjectDlg->GetProjectName());
        updateRecentProjectList(pNewProjectDlg->GetProjectName());
        m_pIoDBVarGroups = new DBVarGroups(m_strProjectName);
        CreateDefaultIOTagGroup();
        m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
        UpdateDeviceVariableTableGroup();
    }
}

void MainWindow::doOpenProject(QString proj)
{
    QFile fileProj(proj);
    if(!fileProj.exists()) {
        QMessageBox::information(this, "系统提示", "工程：" + proj + "不存在！");
        return;
    }

    UpdateProjectName(proj);
    QString strFile = ConfigUtils::AppDir() + "/lastpath.ini";
    ConfigUtils::SetCfgStr(strFile, "PathInfo", "Path", m_strProjectPath);
    // 加载设备变量组信息
    m_pIoDBVarGroups = new DBVarGroups(m_strProjectName);
    m_pIoDBVarGroups->loadFromFile(DATA_SAVE_FORMAT);
    UpdateDeviceVariableTableGroup();
    updateRecentProjectList(proj);
}


void MainWindow::on_actionOpenProject_triggered()
{
    QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";    
    QString path = ConfigUtils::GetCfgStr(strFile, "PathInfo", "Path", "C:/");    
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择工程文件"),
                                                    path,
                                                    tr("project file (*.proj)"));
    if(fileName != NULL) {
        doOpenProject(fileName);
    }
}

void MainWindow::on_actionWorkSpace_triggered(bool checked)
{
    if(checked)
        ui->dockProjectManager->show();
    else
        ui->dockProjectManager->hide();
}

/*
* 工具条使能
*/
void MainWindow::enableToolBar(QString text)
{
    bool bTagIOOrTmp = (text == "中间变量")|(text.startsWith("设备变量"));
    ui->TagOperateToolBar->setEnabled(bTagIOOrTmp);

    bool bdevice = (text == "串口设备")|(text == "网络设备")|(text == "总线设备")|(text == "OPC设备");
    ui->DeviceOperateToolBar->setEnabled(bdevice);

}


void MainWindow::on_treeViewProject_clicked(const QModelIndex &index)
{
    if(m_strProjectName == NULL)
        return;

    QStandardItem *item = pTreeViewProjectModel->itemFromIndex(index);
    QString winTittle = item->text();

    // 工具条使能
    enableToolBar(winTittle);

    ChildForm *findForm = findMdiChild(m_strProjectName);
    if(findForm == NULL) {
        findForm = new ChildForm(this, m_strProjectName);
        findForm->setWindowTitle(m_strProjectName);
        ui->mdiArea->addSubWindow(findForm);
        connect(this, SIGNAL(treeItemClicked(const QString &)),
                findForm, SLOT(treeItemClicked(const QString &)));
    }

    ////////////////////////////////////

    if(item->text() == "系统参数") {
        findForm->switchPage(PAGE_SYSTEM_PARAMETER);
    } else if(item->text() == "通讯设备" || item->text() == "串口设备" || item->text() == "网络设备") {
        findForm->switchPage(PAGE_COMMUNICATE_DEVICE);
    }

    setActiveSubWindow(findForm);

    emit treeItemClicked(item->text());




#if 0
    if(item->text() == "中间变量" || item->text() == "系统变量")
    {
        winTittle = item->text();
    }
    else
    {
        // 设备变量
        foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
        {
            if(item->text() == var->m_name)
            {
                winTittle = QString("%1%2%3").arg("设备变量").arg("-").arg(item->text());
            }
        }
    }

    // 工具条使能
    enableToolBar(winTittle);

    ChildBase* window = findMdiChild(winTittle);
    if(window != NULL)
    {
        setActiveSubWindow(window);
        return;
    }

    ////////////////////////////////////

    bool VarFound = false;
    if(item->text() == "中间变量" || item->text() == "系统变量")
    {
        VarFound = true;
    }
    else
    {
        // 设备变量
        foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
        {
            if(item->text() == var->m_name)
                VarFound = true;
        }
    }

    ////////////////////////////////////


    else if(item->text() == "通讯设备" || item->text() == "串口设备" || item->text() == "网络设备")
    {
        CommunicationDeviceWin * pCommunicationDeviceWin = new CommunicationDeviceWin(this, item->text(), m_strProjectName);
        ui->mdiArea->addSubWindow(pCommunicationDeviceWin);
        setActiveSubWindow(pCommunicationDeviceWin);
    }
    else if(VarFound) // 设备变量
    {
        QString strItem;
        if(item->text() == "中间变量" || item->text() == "系统变量")
        {
            strItem = item->text();
        }
        else
        {
            strItem = QString("%1%2%3").arg("设备变量").arg("-").arg(item->text());
        }
        VariableManagerWin * pVariableManagerWin = new VariableManagerWin(this, strItem, m_strProjectName);
        ui->mdiArea->addSubWindow(pVariableManagerWin);
        setActiveSubWindow(pVariableManagerWin);
    }
    else if(item->text() == "画面")
    {
        DrawPageWin *pDrawPageWin = new DrawPageWin(this, "画面", m_strProjectName);
        ui->mdiArea->addSubWindow(pDrawPageWin);
        setActiveSubWindow(pDrawPageWin);
    }
    else if(item->text() == "实时数据库")
    {
        RealTimeDatabaseWin *pRealTimeDatabaseWin = new RealTimeDatabaseWin(this, "实时数据库", m_strProjectName);
        ui->mdiArea->addSubWindow(pRealTimeDatabaseWin);
        setActiveSubWindow(pRealTimeDatabaseWin);
    }
    else if(item->text() == "脚本编辑器")
    {
        ScriptManageWin *pScriptManageWin = new ScriptManageWin(this, "脚本编辑器", m_strProjectName);
        ui->mdiArea->addSubWindow(pScriptManageWin);
        setActiveSubWindow(pScriptManageWin);
    }
#endif
}


/*   QWidget* pComDeviceWin = createMdiChild();
    m_mapItemWindow.insert("串口设备", pComDeviceWin);
    QWidget* pNetDeviceWin = createMdiChild();
    m_mapItemWindow.insert("网络设备", pNetDeviceWin);
    QWidget* pBusDeviceWin = createMdiChild();
    m_mapItemWindow.insert("总线设备", pBusDeviceWin);
    QWidget* pOPCDeviceWin = createMdiChild();
    m_mapItemWindow.insert("OPC设备", pOPCDeviceWin);
    QWidget* pDataBaseManagerWin = createMdiChild();
    m_mapItemWindow.insert("数据库管理", pDataBaseManagerWin);
    QWidget* pRealTimeDatabaseWin = createMdiChild();
    m_mapItemWindow.insert("实时数据库", pRealTimeDatabaseWin);
    QWidget* pHistoryDatabaseWin = createMdiChild();
    m_mapItemWindow.insert("历史数据库", pHistoryDatabaseWin);
    QWidget* pDrawPageWin = createMdiChild();
    m_mapItemWindow.insert("画面", pDrawPageWin);
    QWidget* pLogicProgramWin = createMdiChild();
    m_mapItemWindow.insert("逻辑编程", pLogicProgramWin);
    QWidget* pLadderEditorWin = createMdiChild();
    m_mapItemWindow.insert("梯形图", pLadderEditorWin);
    QWidget* pScriptEditorWin = createMdiChild();
    m_mapItemWindow.insert("脚本编辑器", pScriptEditorWin);
    QWidget* pSystemToolWin = createMdiChild();
    m_mapItemWindow.insert("系统工具", pSystemToolWin);
    QWidget* pDriveMonitorWin = createMdiChild();
    m_mapItemWindow.insert("驱动监视", pDriveMonitorWin);
    QWidget* pDatabaseInterfaceWin = createMdiChild();
    m_mapItemWindow.insert("数据库接口", pDatabaseInterfaceWin);
    QWidget* pBuilderXMLInterfaceWin = createMdiChild();
    m_mapItemWindow.insert("建筑XML接口", pBuilderXMLInterfaceWin);
*/

void MainWindow::UpdateProjectName(QString name)
{
    if(name != NULL) {
        m_strProjectName = name;
        m_strProjectPath = name.left(name.lastIndexOf("/"));
        QString strName = name.mid(name.lastIndexOf("/") + 1, name.indexOf(".") - name.lastIndexOf("/") - 1);
        pProjectItem->setText(strName);
    } else {
        m_strProjectName = "";
        m_strProjectPath = "";
        qDeleteAll(m_pIoDBVarGroups->m_VarBlockGroupList);
        m_pIoDBVarGroups->m_VarBlockGroupList.clear();

        pTreeViewProjectModel->clear();
        ui->treeViewProject->reset();
        delete pTreeViewProjectModel;
        pTreeViewProjectModel = NULL;

        setUpProjectTreeView();
    }
}


/*
 * 更新设备变量组
 */
void MainWindow::UpdateDeviceVariableTableGroup()
{
    while(!pDevVariableTabList.empty())
        pDevVariableTabList.takeFirst();
    //while(pDevVariable->rowCount())
    pDevVariable->removeRows(0, pDevVariable->rowCount());
    foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        QStandardItem *pDevVarTab = new QStandardItem(QIcon(":/images/pj_zone.png"), var->m_name);
        pDevVarTab->setEditable(false);
        pDevVariableTabList.append(pDevVarTab);
        pDevVariable->appendRow(pDevVarTab);
    }
}


/*
 * 所有活动窗口执行保存
 */
void MainWindow::on_actionSaveProject_triggered()
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        ChildForm *findForm = qobject_cast<ChildForm *>(window->widget());
        if(findForm != NULL)
            findForm->save();
    }
}

/*
* 插槽：关闭
*/
void MainWindow::on_actionCloseProject_triggered()
{
    if(pProjectItem->text() == "未创建工程")
        return;
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        ChildForm * findForm = qobject_cast<ChildForm *>(window->widget());
        if(findForm != NULL)
            findForm->save();
        window->close();
    }
    UpdateProjectName(NULL);
}

/*
* 插槽：退出
*/
void MainWindow::on_actionExit_triggered()
{
    on_actionSaveProject_triggered();
    qApp->exit();
}

void MainWindow::on_treeViewProject_activated(const QModelIndex &/*index*/)
{

}

/*
 * 插槽：模拟
 */
void MainWindow::on_actionSimulate_triggered()
{

}

/*
 * 插槽：运行
 */
void MainWindow::on_actionRun_triggered()
{

}

/*
 * 插槽：上载
 */
void MainWindow::on_actionUpLoad_triggered()
{
    // 创建tmp目录
    QString tmpDir = QCoreApplication::applicationDirPath() + "/UploadProjects/tmp";
    QDir dir(tmpDir);
    if(!dir.exists()) {
        dir.mkpath(tmpDir);
    }

    ProjectUploadDialog *pDlg = new ProjectUploadDialog(this, m_strProjectName);
    if(pDlg->exec() == QDialog::Accepted) {
        QString desDir = pDlg->getProjectPath();
        QString program = QCoreApplication::applicationDirPath() + "/tar/tar.exe";
        QFile programFile(program);
        if(!programFile.exists()) {
            QMessageBox::information(this, "系统提示", "命令：" + program + "不存在！");
            return;
        }
        QProcess *tarProc = new QProcess;
        // 设置进程工作目录
        tarProc->setWorkingDirectory(QCoreApplication::applicationDirPath() + "/tar");
        QStringList arguments;
        arguments << "-xvf" << "../UploadProjects/RunProject.tar" << "-C" << "../UploadProjects/tmp";
        tarProc->start(program, arguments);
        if(tarProc->waitForStarted()) {
            if(tarProc->waitForFinished()) {
                //qDebug()<< "process finished.";
                QString strSrc = QCoreApplication::applicationDirPath() + "/UploadProjects/tmp/RunProject";

                qDebug()<<strSrc;
                qDebug()<<desDir;
                Helper::CopyDir(strSrc, desDir, true);
                Helper::DeleteDir(tmpDir);

                QString tarProj = QCoreApplication::applicationDirPath() + "/UploadProjects/RunProject.tar";
                QFile tarProjFile(tarProj);
                if(tarProjFile.exists()) {
                    tarProjFile.remove();
                }
            }
        } else {
            QMessageBox::information(this, "系统提示", "解压缩工程失败！");
        }

        delete tarProc;
    }
    delete pDlg;
}

/*
 * 插槽：U盘
 */
void MainWindow::on_actionUDisk_triggered()
{

}

/*
 * 插槽：下载
 */
void MainWindow::on_actionDownload_triggered()
{
    if(m_strProjectName == NULL)
        return;

    // 创建tmp目录
    QString tmpDir = QCoreApplication::applicationDirPath() + "/tmp";
    QDir dir(tmpDir);
    if(!dir.exists()) {
        dir.mkpath(tmpDir);
    }

    // 拷贝工程到tmp目录
    QString desDir = QCoreApplication::applicationDirPath() + "/tmp/RunProject";
    Helper::CopyRecursively(m_strProjectPath, desDir);

    // 打包工程到tmp目录
    QString program = QCoreApplication::applicationDirPath() + "/tar/tar.exe";
    QFile programFile(program);
    if(!programFile.exists()) {
        QMessageBox::information(this, "系统提示", "命令：" + program + "不存在！");
        return;
    }

    QProcess *tarProc = new QProcess;
    // 设置进程工作目录
    tarProc->setWorkingDirectory(QCoreApplication::applicationDirPath() + "/tar");
    QStringList arguments;
    arguments << "-cvf" << "../tmp/RunProject.tar" << "-C"<< "../tmp" << "RunProject";
    tarProc->start(program, arguments);
    if(tarProc->waitForStarted()) {
        //qDebug()<< "process start.";
        if(tarProc->waitForFinished()) {
            //qDebug()<< "process finished.";
            // 压缩完成准备传输文件
        }
        if(tarProc->exitStatus() == QProcess::NormalExit) {
            //qDebug()<< "process exitStatus is QProcess::NormalExit.";
        } else { // QProcess::CrashExit

            //qDebug()<< "process exitStatus is QProcess::CrashExit.";
        }
    } else {
        QMessageBox::information(this, "系统提示", "压缩工程失败！");
    }

    QDir dirRunProj(desDir);
    if(dirRunProj.exists()) {
        Helper::DeleteDir(desDir);
    }

    delete tarProc;

    ProjectDownloadDialog *pDlg = new ProjectDownloadDialog(this, m_strProjectName);
    pDlg->setProjFileName(QCoreApplication::applicationDirPath() + "/tmp/RunProject.tar");
    if(pDlg->exec() == QDialog::Accepted) {

    }
    delete pDlg;
}


/*
 * 插槽：增加
 */
void MainWindow::on_actionAddTag_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
        pVariableManagerWin->VariableAdd();
    }
#endif
}

/*
 * 插槽：追加
 */
void MainWindow::on_actionAppendTag_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
        pVariableManagerWin->VariableAppend();
    }
#endif
}

/*
 * 插槽：行拷贝
 */
void MainWindow::on_actionRowCopyTag_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
        pVariableManagerWin->VariableRowCopy();
    }
#endif
}

/*
 * 插槽：列拷贝
 */
void MainWindow::on_actionColumnCopyTag_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
        pVariableManagerWin->VariableColCopy();
    }
#endif
}

/*
 * 插槽：修改
 */
void MainWindow::on_actionModifyTag_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
        pVariableManagerWin->VariableModify();
    }
#endif
}

/*
 * 插槽：删除
 */
void MainWindow::on_actionDeleteTag_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
        pVariableManagerWin->VariableDelete();
    }
#endif
}

/*
 * 插槽：导出
 */
void MainWindow::on_actionExportTag_triggered()
{
#if 0
    QString dirUploadProjects = QCoreApplication::applicationDirPath();
    QString strSaveCsvPath = QFileDialog::getExistingDirectory(this, tr("选择导出csv路径"),
                                                       dirUploadProjects,
                                                       QFileDialog::ShowDirsOnly|
                                                       QFileDialog::DontResolveSymlinks);
    if(strSaveCsvPath == "")
        return;

    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
        pVariableManagerWin->exportToCsv(strSaveCsvPath, this->m_CurItem);
    }
#endif
}

/*
 * 插槽：导入
 */
void MainWindow::on_actionImportTag_triggered()
{
#if 0
    QString path = QCoreApplication::applicationDirPath();
    QString strSaveCsvFile = QFileDialog::getOpenFileName(this, tr("选择csv文件"),
                                                    path,
                                                    tr("csv file (*.csv)"));

    if(strSaveCsvFile == "")
        return;

    QString strCsvName= strSaveCsvFile.mid(strSaveCsvFile.lastIndexOf("/") + 1, strSaveCsvFile.indexOf(".") - strSaveCsvFile.lastIndexOf("/") - 1);
    QString strGroupName = strCsvName.right(strCsvName.length() - strCsvName.lastIndexOf("-") - 1);

    bool found = false;
    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList)
    {
        if(strGroupName == var->m_name)
        {
            found = true;
            break;
        }
    }
    if(found)
    {
        ChildBase* window = findMdiChild(this->m_CurItem);

        if(window != NULL)
        {
            VariableManagerWin * pVariableManagerWin = qobject_cast<VariableManagerWin *>(window);
            pVariableManagerWin->importFromCsv(strSaveCsvFile);
        }
    }
    else
    {
        DBVarGroup *pGroup = new DBVarGroup();
        pGroup->m_type = "WorkNode";
        pGroup->m_name = strGroupName;
        m_pIoDBVarGroups->m_VarBlockGroupList.append(pGroup);
        m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
        UpdateDeviceVariableTableGroup();

        // 工具条使能
        enableToolBar(strCsvName);

        ChildBase* window = findMdiChild(strCsvName);
        if(window != NULL)
        {
            setActiveSubWindow(window);
            return;
        }

        VariableManagerWin * pVariableManagerWin = new VariableManagerWin(this, strCsvName, m_strProjectName);
        ui->mdiArea->addSubWindow(pVariableManagerWin);
        setActiveSubWindow(pVariableManagerWin);
        pVariableManagerWin->importFromCsv(strSaveCsvFile);
    }

#endif
}

/*
 * 插槽：新建设备
 */
void MainWindow::on_actionDeviceNew_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        CommunicationDeviceWin * pCommunicationDeviceWin = qobject_cast<CommunicationDeviceWin *>(window);
        pCommunicationDeviceWin->NewDevice();
    }
#endif
}

/*
 * 插槽：修改设备
 */
void MainWindow::on_actionDeviceModify_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        CommunicationDeviceWin * pCommunicationDeviceWin = qobject_cast<CommunicationDeviceWin *>(window);
        pCommunicationDeviceWin->ModifyDevice();
    }
#endif
}

/*
 * 插槽：删除设备
 */
void MainWindow::on_actionDeviceDelete_triggered()
{
#if 0
    ChildBase* window = findMdiChild(this->m_CurItem);

    if(window != NULL)
    {
        CommunicationDeviceWin * pCommunicationDeviceWin = qobject_cast<CommunicationDeviceWin *>(window);
        pCommunicationDeviceWin->DeleteDevice();
    }
#endif
}

/*
 * 插槽：帮助
 */
void MainWindow::on_actionHelp_triggered()
{

}


/*
 * 插槽：关于
 */
void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *pDlg = new AboutDialog(this);
    if(pDlg->exec() == QDialog::Accepted) {

    }
    delete pDlg;
}

/*
 * 加载最近打开的工程列表
 */
void MainWindow::loadRecentProjectList()
{
    QString iniRecentProjectFileName = ConfigUtils::AppDir() + "/RecentProjectList.ini";
    QFile fileCfg(iniRecentProjectFileName);
    if(fileCfg.exists()) {
        QStringList slist;
        ConfigUtils::GetCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);

        for (int i=0; i<slist.count(); i++) {
            QAction *pAct = new QAction(slist.at(i), this);
            pAct->setStatusTip(tr(""));
            connect(pAct, &QAction::triggered, this, [=]{
                        this->doOpenProject(pAct->text());
                    });
            ui->menuProject->insertAction(ui->actionRecentProjectList, pAct);
        }
        ui->menuProject->removeAction(ui->actionRecentProjectList);
        return;
    }

    QList<QAction *> listActRemove;
    QList<QAction *> listAct = ui->menuProject->actions();
    for (int i = 0; i<listAct.size(); ++i) {
        QAction *pAct = listAct.at(i);
        if(pAct->isSeparator())
            listActRemove.append(pAct);
        if(pAct->text() == tr("最近文件列表"))
            listActRemove.append(pAct);
    }
    for (int j = 0; j<listActRemove.size(); ++j) {
        ui->menuProject->removeAction(listActRemove.at(j));
    }
}

/*
 * 更新最近打开的工程列表
 */
void MainWindow::updateRecentProjectList(QString newProj)
{
    //qDebug()<<newProj;
    bool bStart = false;
    bool bEnd = false;

    QString iniRecentProjectFileName = ConfigUtils::AppDir() + "/RecentProjectList.ini";
    QFile fileCfg(iniRecentProjectFileName);
    if(fileCfg.exists()) {
        QStringList slist;
        ConfigUtils::GetCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);

        for (int i=0; i<slist.count(); i++) {
            if(newProj == slist.at(i)) {
                return;
            }
        }

        if(slist.count() >= 5)
            slist.removeLast();

        slist.push_front(newProj);
        ConfigUtils::WriteCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);

        QList<QAction *> listActRemove;
        QList<QAction *> listAct = ui->menuProject->actions();
        for (int i = 0; i < listAct.size(); ++i) {
            QAction *pAct = listAct.at(i);
            if(pAct->isSeparator() && bStart == false && bEnd == false) {
                bStart = true;
                continue;
            }
            if(pAct->isSeparator() && bStart && bEnd == false) {
                bEnd = true;
                bStart = false;
                break;
            }
            if(bStart && bEnd == false) {
                listActRemove.append(pAct);
            }
            if(pAct->text() == tr("最近文件列表"))
                listActRemove.append(pAct);
        }
        for (int j = 0; j<listActRemove.size(); ++j) {
            ui->menuProject->removeAction(listActRemove.at(j));
        }

        /////////////////////////////////////////////////////

        bStart = bEnd = false;
        QAction *pActPos;
        listAct.clear();
        listAct = ui->menuProject->actions();
        for (int i = 0; i < listAct.size(); ++i) {
            QAction *pAct = listAct.at(i);
            if(pAct->isSeparator() && bStart == false && bEnd == false) {
                bStart = true;
                continue;
            }
            if(pAct->isSeparator() && bStart && bEnd == false) {
                bEnd = true;
                pActPos = pAct;
                bStart = false;
                break;
            }
        }

        for (int i=0; i<slist.count(); i++) {
            QAction *pAct = new QAction(slist.at(i), this);
            pAct->setStatusTip(tr(""));
            connect(pAct, &QAction::triggered, this, [=]{
                        this->doOpenProject(pAct->text());
                    });
            ui->menuProject->insertAction(pActPos, pAct);
        }
    } else {
        QStringList slist;
        ConfigUtils::WriteCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);
    }
}


/*
* 显示大图标
*/
void MainWindow::on_actionBigIcon_triggered()
{
    ui->actionBigIcon->setChecked(true);
    ui->actionSmallIcon->setChecked(false);
    ChildForm *findForm = findMdiChild(this->m_CurItem);
    if(findForm != NULL) {
        findForm->ShowLargeIcon();
    }
}

/*
* 显示小图标
*/
void MainWindow::on_actionSmallIcon_triggered()
{
    ui->actionBigIcon->setChecked(false);
    ui->actionSmallIcon->setChecked(true);
    ChildForm *findForm = findMdiChild(this->m_CurItem);
    if(findForm != NULL) {
        findForm->ShowSmallIcon();
    }
}
