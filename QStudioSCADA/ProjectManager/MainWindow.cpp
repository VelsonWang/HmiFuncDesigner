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
    m_CurItem(""),
    m_CurTreeViewItem("")
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


ChildForm* MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow()) {
        return qobject_cast<ChildForm *>(activeSubWindow->widget());
    }
    return nullptr;
}

void MainWindow::setActiveSubWindow(ChildForm *window)
{
    if(!window)
        return;
    window->showMaximized();
    m_CurItem = window->windowTitle();
    ui->mdiArea->setActiveSubWindow(0); // Activates the subwindow window. If window is 0, any current active window is deactivated.
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

ChildForm* MainWindow::getActiveSubWindow()
{
    return qobject_cast<ChildForm*>(ui->mdiArea->activeSubWindow()->widget());
}

ChildForm* MainWindow::findMdiChild(const QString &windowTitle)
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        ChildForm *pChildWin = qobject_cast<ChildForm *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return pChildWin;
    }
    return nullptr;
}


QMdiSubWindow* MainWindow::findMdiSubWindow(const QString &windowTitle)
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        ChildBase * pChildWin = qobject_cast<ChildBase *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return window;
    }
    return nullptr;
}

/*
* 新建工程时，创建缺省IO变量组
*/
void MainWindow::CreateDefaultIOTagGroup()
{
    if(pDevVariable->rowCount() == 0) {
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
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(pDlg->exec() == QDialog::Accepted) {
        DBVarGroup *pGroup = new DBVarGroup();
        pGroup->m_type = "WorkNode";
        pGroup->m_name = pDlg->GetGroupName();
        pGroup->m_iPageID = m_pIoDBVarGroups->m_VarBlockGroupList.count();
        m_pIoDBVarGroups->m_VarBlockGroupList.append(pGroup);
        m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
        UpdateDeviceVariableTableGroup();       
        if(window != NULL) {
            QString titleNew = QString("%1%2%3").arg("设备变量").arg("-").arg(pDlg->GetGroupName());
            window->SetTitle(titleNew);
        }
    }
}

/*
* 重命名组
*/
void MainWindow::VariableGroupRename()
{
    QModelIndex index = ui->treeViewProject->currentIndex();
    QString text = this->pTreeViewProjectModel->itemFromIndex(index)->text();
    ChildForm* window = findMdiChild(this->m_CurItem);

    foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
        if(text == var->m_name) {
            NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
            pDlg->SetDialogName("新建数据组");
            pDlg->SetLabelName("数据组名：");
            pDlg->SetGroupName(text);
            if(pDlg->exec() == QDialog::Accepted) {
                if(window != NULL) {
                    QString titleNew = QString("%1%2%3").arg("设备变量").arg("-").arg(pDlg->GetGroupName());
                    window->SetTitle(titleNew);
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
}

/*
* 删除组
*/
void MainWindow::VariableDeleteGroup()
{
    QModelIndex ModelIndex = ui->treeViewProject->selectionModel()->currentIndex();
    QStandardItem *qTiem = pTreeViewProjectModel->itemFromIndex(ModelIndex);

    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
        if(qTiem->text() == var->m_name) {
            ChildForm *findForm = findMdiChild(m_strProjectName);
            if(findForm != NULL) {
                findForm->hide();
            }
            QString file = "";
            file = m_strProjectName.left(m_strProjectName.lastIndexOf("/")) + "/DevVarList-" + var->m_name + ".odb";
            QFile delFile(file);
            if(delFile.exists()) {
                delFile.remove();
            }
            m_pIoDBVarGroups->m_VarBlockGroupList.removeOne(var);
            break;
        }
    }
    m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
    UpdateDeviceVariableTableGroup();
}

/*
* 复制组
*/
void MainWindow::VariableGroupCopy()
{
    QModelIndex ModelIndex = ui->treeViewProject->selectionModel()->currentIndex();
    QStandardItem *qTiem = pTreeViewProjectModel->itemFromIndex(ModelIndex);

    // check the same name first
    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
        if(var->m_name == QString("复制_%1").arg(qTiem->text())) {
            QMessageBox::information(this, "系统提示", "同名文件存在，请先修改名称！");
            return;
        }
    }

    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
        if(qTiem->text() == var->m_name) {
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
}

/*
* 修改ID
*/
void MainWindow::VariableModifyGroupID()
{
    QModelIndex index = ui->treeViewProject->currentIndex();
    QString text = this->pTreeViewProjectModel->itemFromIndex(index)->text();
    foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
        if(text == var->m_name) {
            NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
            pDlg->SetDialogName("设置设备ID");
            pDlg->SetLabelName("设备ID：");
            if(pDlg->exec() == QDialog::Accepted) {
                //id = pDlg->GetGroupName();
                //m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
                UpdateDeviceVariableTableGroup();
            }
        }
    }
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

/**
 * @brief MainWindow::closeEvent  关闭事件
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QString strFile = Helper::AppDir() + "/lastpath.ini";
    if(pProjectItem->text() != tr("未创建工程"))
        ConfigUtils::setCfgStr(strFile, "PathInfo", "Path", m_strProjectPath);
    ui->mdiArea->closeAllSubWindows();
    writeSettings();
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        event->accept();
    }
}

/**
 * @brief MainWindow::writeSettings 写入窗口设置
 */
void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

/**
 * @brief MainWindow::readSettings 读取窗口设置
 */
void MainWindow::readSettings()
{
    //qDebug()<<QCoreApplication::organizationName() << QCoreApplication::applicationName();
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() * 3 / 4, availableGeometry.height() * 3 / 4);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

/**
 * @brief MainWindow::initWindow 初始化窗口
 */
void MainWindow::initWindow()
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
    if(pProjectItem->text() != tr("未创建工程")) {
        QMessageBox::information(this, tr("系统提示"), tr("工程文件已建立，请手动关闭当前工程文件后重新建立！"));
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
        QMessageBox::information(this, tr("系统提示"), tr("工程：") + proj + tr("不存在！"));
        return;
    }

    UpdateProjectName(proj);
    QString strFile = Helper::AppDir() + "/lastpath.ini";
    ConfigUtils::setCfgStr(strFile, "PathInfo", "Path", m_strProjectPath);
    // 加载设备变量组信息
    m_pIoDBVarGroups = new DBVarGroups(m_strProjectName);
    m_pIoDBVarGroups->loadFromFile(DATA_SAVE_FORMAT);
    UpdateDeviceVariableTableGroup();
    updateRecentProjectList(proj);
}


void MainWindow::on_actionOpenProject_triggered()
{
    QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";    
    QString path = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "C:/");
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
    bool bTagIOOrTmp = (text == tr("中间变量")) | (text.startsWith(tr("设备变量")));
    ui->TagOperateToolBar->setEnabled(bTagIOOrTmp);

    bool bdevice = (text == tr("串口设备")) | (text == tr("网络设备")) | (text == tr("总线设备")) | (text == tr("OPC设备"));
    ui->DeviceOperateToolBar->setEnabled(bdevice);

}


void MainWindow::on_treeViewProject_clicked(const QModelIndex &index)
{
    if(m_strProjectName == NULL)
        return;

    QStandardItem *item = pTreeViewProjectModel->itemFromIndex(index);
    QString winTittle = item->text();

    ////////////////////////////////////////////////////////////////////////

    ChildForm *findForm = findMdiChild(m_strProjectName);
    if(findForm == NULL) {
        findForm = new ChildForm(this, m_strProjectName);
        findForm->setWindowTitle(m_strProjectName);
        ui->mdiArea->addSubWindow(findForm);
        connect(this, SIGNAL(treeItemClicked(const QString &)),
                findForm, SLOT(treeItemClicked(const QString &)));
    }

    ////////////////////////////////////////////////////////////////////////

    if(item->text() == tr("变量管理") || item->text() == tr("设备变量")) {
        if(findForm)
            findForm->hide();
        return;
    }

    if(item->text() == tr("中间变量") || item->text() == tr("系统变量")) {
        winTittle = item->text();
    } else {
        // 设备变量
        foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
            if(item->text() == var->m_name) {
                winTittle = QString("%1%2%3").arg(tr("设备变量")).arg("-").arg(item->text());
            }
        }
    }

    // 工具条使能
    enableToolBar(winTittle);

    ////////////////////////////////////////////////////////////////////////

    bool VarFound = false;
    if(item->text() == tr("中间变量") || item->text() == tr("系统变量")) {
        VarFound = true;
    } else {
        // 设备变量
        foreach (DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
            if(item->text() == var->m_name)
                VarFound = true;
        }
    }

    ////////////////////////////////////////////////////////////////////////

    if(item->text() == tr("系统参数")) {
        findForm->switchPage(PAGE_SYSTEM_PARAMETER);
    } else if(item->text() == tr("通讯设备") || item->text() == tr("串口设备") || item->text() == tr("网络设备")) {
        findForm->switchPage(PAGE_COMMUNICATE_DEVICE);
    } else if(VarFound) { // 变量
        if(item->text() == tr("中间变量") || item->text() == tr("系统变量")) {
            winTittle = item->text();
        } else {
            winTittle = QString("%1%2%3").arg(tr("设备变量")).arg("-").arg(item->text());
        }
        findForm->switchPage(PAGE_VARIABLE_MANAGER);
        if(item->text() == tr("设备变量"))
            findForm->switchPage(PAGE_NONE);
    } else if(item->text() == tr("画面")) {
        findForm->switchPage(PAGE_DRAW_PAGE);
    } else if(item->text() == tr("实时数据库")) {
        findForm->switchPage(PAGE_RTDB);
    } else if(item->text() == tr("历史数据库")) {
        findForm->switchPage(PAGE_NONE);
        findForm->hide();
    } else if(item->text() == tr("脚本编辑器")) {
        findForm->switchPage(PAGE_SCRIPT_MANAGER);
    }

    setActiveSubWindow(findForm);
    emit treeItemClicked(winTittle);
}

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
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->addVariableTag();
    }
}

/*
 * 插槽：追加
 */
void MainWindow::on_actionAppendTag_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->appendVariableTag();
    }
}

/*
 * 插槽：行拷贝
 */
void MainWindow::on_actionRowCopyTag_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->rowCopyVariableTag();
    }
}

/*
 * 插槽：列拷贝
 */
void MainWindow::on_actionColumnCopyTag_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->columnCopyVariableTag();
    }
}

/*
 * 插槽：修改
 */
void MainWindow::on_actionModifyTag_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->modifyVariableTag();
    }
}

/*
 * 插槽：删除
 */
void MainWindow::on_actionDeleteTag_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->deleteVariableTag();
    }
}

/*
 * 插槽：导出
 */
void MainWindow::on_actionExportTag_triggered()
{
    QString dirUploadProjects = QCoreApplication::applicationDirPath();
    QString strSaveCsvPath = QFileDialog::getExistingDirectory(this, tr("选择导出csv路径"),
                                                       dirUploadProjects,
                                                       QFileDialog::ShowDirsOnly|
                                                       QFileDialog::DontResolveSymlinks);
    if(strSaveCsvPath == "")
        return;

    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->variableTagExportToCsv(strSaveCsvPath);
    }
}

/*
 * 插槽：导入
 */
void MainWindow::on_actionImportTag_triggered()
{
    QString path = QCoreApplication::applicationDirPath();
    QString strSaveCsvFile = QFileDialog::getOpenFileName(this, tr("选择csv文件"),
                                                    path,
                                                    tr("csv file (*.csv)"));

    if(strSaveCsvFile == "")
        return;

    QString strCsvName= strSaveCsvFile.mid(strSaveCsvFile.lastIndexOf("/") + 1, strSaveCsvFile.indexOf(".") - strSaveCsvFile.lastIndexOf("/") - 1);
    QString strGroupName = strCsvName.right(strCsvName.length() - strCsvName.lastIndexOf("-") - 1);

    bool found = false;
    foreach(DBVarGroup *var, m_pIoDBVarGroups->m_VarBlockGroupList) {
        if(strGroupName == var->m_name) {
            found = true;
            break;
        }
    }
    if(!found) {
        DBVarGroup *pGroup = new DBVarGroup();
        pGroup->m_type = "WorkNode";
        pGroup->m_name = strGroupName;
        m_pIoDBVarGroups->m_VarBlockGroupList.append(pGroup);
        m_pIoDBVarGroups->saveToFile(DATA_SAVE_FORMAT);
        UpdateDeviceVariableTableGroup();
        enableToolBar(strCsvName);
    }

    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        QString titleNew = QString("%1%2%3").arg("设备变量").arg("-").arg(strGroupName);
        window->SetTitle(titleNew);
        window->variableTagImportFromCsv(strSaveCsvFile);
    } 
}


/**
 * @brief MainWindow::on_actionDeviceNew_triggered 新建设备
 */
void MainWindow::on_actionDeviceNew_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->newDevice();
    }
}

/*
 * 插槽：修改设备
 */
void MainWindow::on_actionDeviceModify_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->modifyDevice();
    }
}

/*
 * 插槽：删除设备
 */
void MainWindow::on_actionDeviceDelete_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != NULL) {
        window->deleteDevice();
    }
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
    QString iniRecentProjectFileName = Helper::AppDir() + "/RecentProjectList.ini";
    QFile fileCfg(iniRecentProjectFileName);
    if(fileCfg.exists()) {
        QStringList slist;
        ConfigUtils::getCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);

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

    QString iniRecentProjectFileName = Helper::AppDir() + "/RecentProjectList.ini";
    QFile fileCfg(iniRecentProjectFileName);
    if(fileCfg.exists()) {
        QStringList slist;
        ConfigUtils::getCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);

        for (int i=0; i<slist.count(); i++) {
            if(newProj == slist.at(i)) {
                return;
            }
        }

        if(slist.count() >= 5)
            slist.removeLast();

        slist.push_front(newProj);
        ConfigUtils::writeCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);

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
        ConfigUtils::writeCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);
    }
}


/**
 * @brief MainWindow::on_actionBigIcon_triggered 显示大图标
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


/**
 * @brief MainWindow::on_actionSmallIcon_triggered 显示小图标
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
