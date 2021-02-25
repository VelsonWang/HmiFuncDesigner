#include "MainWindow.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QPluginLoader>
#include <QProcess>
#include <QSettings>
#include <QStandardItem>
#include <QStringList>
#include <QTime>
#include <QDir>
#include "AboutDialog.h"
#include "CommunicationDeviceWin.h"
#include "ConfigUtils.h"
#include "DrawPageWin.h"
#include "Helper.h"
#include "NewProjectDialog.h"
#include "NewVariableGroupDialog.h"
#include "ProjectData.h"
#include "ProjectDownloadDialog.h"
#include "ProjectMgrUtils.h"
#include "ProjectUploadDialog.h"
#include "RealTimeDatabaseWin.h"
#include "ScriptManageWin.h"
#include "ProjectData.h"
#include "TagManagerWin.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_strProjectPath(""),
      m_CurItem(""),
      m_CurTreeViewItem("")
{
    ui->setupUi(this);
    enableToolBar("");                             // 工具条使能
    setContextMenuPolicy(Qt::DefaultContextMenu);  // 右键菜单生效
    readSettings();  // 初始窗口时读取窗口设置信息
    initWindow();    // 初始化窗口
    setUpProjectTreeView();
    loadRecentProjectList();
    on_actionBigIcon_triggered();  // 大图标显示
}

MainWindow::~MainWindow()
{
    delete pTreeViewProjectModel;
    pTreeViewProjectModel = Q_NULLPTR;
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

    pDrawPage = new QStandardItem(QIcon(":/images/pm_draw.png"), tr("画面"));
    pDrawPage->setEditable(false);
    pProjectItem->appendRow(pDrawPage);

    pLogicProgram = new QStandardItem(QIcon(":/images/pm_script.png"), tr("逻辑编程"));
    pLogicProgram->setEditable(false);

    pScriptEditor = new QStandardItem(QIcon(":/images/pj_script.png"), tr("脚本编辑器"));
    pScriptEditor->setEditable(false);
    pLogicProgram->appendRow(pScriptEditor);
    pProjectItem->appendRow(pLogicProgram);

    pTreeViewProjectModel->appendRow(pProjectItem);
    ui->treeViewProject->setModel(pTreeViewProjectModel);
    ui->treeViewProject->expandAll();
}

ChildForm *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow()) {
        return qobject_cast<ChildForm *>(activeSubWindow->widget());
    }
    return nullptr;
}

void MainWindow::setActiveSubWindow(ChildForm *window)
{
    if (!window) return;
    window->showMaximized();
    m_CurItem = window->windowTitle();
    ui->mdiArea->setActiveSubWindow(Q_NULLPTR);  // Activates the subwindow window. If
    // window is 0, any current active window
    // is deactivated.
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

ChildForm *MainWindow::getActiveSubWindow()
{
    return qobject_cast<ChildForm *>(ui->mdiArea->activeSubWindow()->widget());
}

ChildForm *MainWindow::findMdiChild(const QString &windowTitle)
{
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        ChildForm *pChildWin = qobject_cast<ChildForm *>(window->widget());
        if (pChildWin->windowTitle() == windowTitle) return pChildWin;
    }
    return nullptr;
}

QMdiSubWindow *MainWindow::findMdiSubWindow(const QString &windowTitle)
{
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        ChildBase *pChildWin = qobject_cast<ChildBase *>(window->widget());
        if (pChildWin->windowTitle() == windowTitle) return window;
    }
    return nullptr;
}

/*
* 新建工程时，创建缺省IO变量组
*/
void MainWindow::CreateDefaultIOTagGroup()
{
    if (pDevVariable->rowCount() == 0) {
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
        pObj->m_id = 1;
        pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
        pObj->m_szShowName = QString(tr("IO设备"));
        tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
        if(pObj != Q_NULLPTR) {
            delete pObj;
            pObj = Q_NULLPTR;
        }
        UpdateDeviceVariableTableGroup();
    }
}

/*
* 增加组
*/
void MainWindow::tagIOGroupAdd()
{
    NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
    pDlg->SetDialogName("新建数据组");
    pDlg->SetLabelName("数据组名：");
    ChildForm *window = findMdiChild(this->m_CurItem);
    if (pDlg->exec() == QDialog::Accepted) {
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
        pObj->m_id = tagIOGroup.getGroupCount(ProjectData::getInstance()->dbData_) + 1;
        pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
        pObj->m_szShowName = pDlg->GetGroupName();
        tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
        UpdateDeviceVariableTableGroup();
        if(window != nullptr) {
            QString titleNew = QString("%1%2%3").arg("设备变量").arg("-").arg(pDlg->GetGroupName());
            window->SetTitle(titleNew);
        }
        if(pObj != Q_NULLPTR) {
            delete pObj;
            pObj = Q_NULLPTR;
        }
    }
}

/*
* 重命名组
*/
void MainWindow::tagIOGroupRename()
{
    QModelIndex index = ui->treeViewProject->currentIndex();
    QString text = this->pTreeViewProjectModel->itemFromIndex(index)->text();
    ChildForm *window = findMdiChild(this->m_CurItem);

    NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
    pDlg->SetDialogName("新建数据组");
    pDlg->SetLabelName("数据组名：");
    pDlg->SetGroupName(text);
    if (pDlg->exec() == QDialog::Accepted) {
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        TagIOGroupDBItem *pObj = tagIOGroup.getGroupObjByShowName(ProjectData::getInstance()->dbData_, text);

        if(window != Q_NULLPTR) {
            QString titleNew = QString("%1%2%3")
                    .arg("设备变量")
                    .arg("-")
                    .arg(pDlg->GetGroupName());
            window->SetTitle(titleNew);
        }
        pObj->m_szShowName = pDlg->GetGroupName();
        tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
        if(pObj != Q_NULLPTR) {
            delete pObj;
            pObj = Q_NULLPTR;
        }
        UpdateDeviceVariableTableGroup();
    }
}

/*
* 删除组
*/
void MainWindow::tagIODeleteGroup()
{
    QModelIndex ModelIndex = ui->treeViewProject->selectionModel()->currentIndex();
    QStandardItem *qTiem = pTreeViewProjectModel->itemFromIndex(ModelIndex);

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    TagIOGroupDBItem *pObj = tagIOGroup.getGroupObjByShowName(ProjectData::getInstance()->dbData_, qTiem->text());
    tagIOGroup.del(ProjectData::getInstance()->dbData_, pObj);
    if(pObj != Q_NULLPTR) {
        delete pObj;
        pObj = Q_NULLPTR;
    }
    UpdateDeviceVariableTableGroup();
}

/*
* 复制组
*/
void MainWindow::tagIOGroupCopy()
{
    QModelIndex ModelIndex = ui->treeViewProject->selectionModel()->currentIndex();
    QStandardItem *qTiem = pTreeViewProjectModel->itemFromIndex(ModelIndex);

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    // check the same name first
    QString szName = QString("复制_%1").arg(qTiem->text());
    int iCnt = tagIOGroup.getGroupCountByShowName(ProjectData::getInstance()->dbData_, szName);
    if(iCnt > 0) {
        QMessageBox::information(this, "系统提示", "同名文件存在，请先修改名称！");
        return;
    }

    TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
    pObj->m_id = tagIOGroup.getGroupCount(ProjectData::getInstance()->dbData_) + 1;
    pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
    pObj->m_szShowName = szName;
    tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
    UpdateDeviceVariableTableGroup();
    if(pObj != Q_NULLPTR) {
        delete pObj;
        pObj = Q_NULLPTR;
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

    if (m_CurTreeViewItem == "设备变量")
        found = true;

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    tagIOGroup.load(ProjectData::getInstance()->dbData_);

    foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
        if (m_CurTreeViewItem == pObj->m_szShowName)
            found = true;
    }

    qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
    tagIOGroup.listTagIOGroupDBItem_.clear();

    if (!found)
        return;

    QMenu *pMenu = new QMenu(this);

    QAction *pAddGroupAct = new QAction(tr("增加组"), this);
    pAddGroupAct->setStatusTip(tr("增加组"));
    connect(pAddGroupAct, SIGNAL(triggered()), this, SLOT(tagIOGroupAdd()));
    pMenu->addAction(pAddGroupAct);

    QAction *pRenameGroupAct = new QAction(tr("重命名"), this);
    pRenameGroupAct->setStatusTip(tr("重命名"));
    connect(pRenameGroupAct, SIGNAL(triggered()), this, SLOT(tagIOGroupRename()));
    pMenu->addAction(pRenameGroupAct);

    QAction *pDeleteGroupAct = new QAction(tr("删除组"), this);
    pDeleteGroupAct->setStatusTip(tr("删除组"));
    connect(pDeleteGroupAct, SIGNAL(triggered()), this, SLOT(tagIODeleteGroup()));
    pMenu->addAction(pDeleteGroupAct);

    QAction *pCopyGroupAct = new QAction(tr("复制组"), this);
    pCopyGroupAct->setStatusTip(tr("复制组"));
    connect(pCopyGroupAct, SIGNAL(triggered()), this, SLOT(tagIOGroupCopy()));
    pMenu->addAction(pCopyGroupAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}

/**
 * @brief MainWindow::closeEvent  关闭事件
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    QString strFile = Helper::AppDir() + "/lastpath.ini";
    if (pProjectItem->text() != tr("未创建工程"))
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
void MainWindow::initWindow() {
    setCentralWidget(ui->mdiArea);
    //    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("HmiFuncDesigner跨平台自动化软件"));

    // 当多文档区域的内容超出可视区域后，出现滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//    ui->mdiArea->setLineWidth(3);
//    ui->mdiArea->setFrameShape(QFrame::Panel);
//    ui->mdiArea->setFrameShadow(QFrame::Sunken);
//    ui->mdiArea->setViewMode(QMdiArea::TabbedView);

    ui->statusBar->showMessage(tr("欢迎使用HmiFuncDesigner组态系统"));
}

void MainWindow::on_actionNewPoject_triggered() {
    if (pProjectItem->text() != tr("未创建工程")) {
        QMessageBox::information(
                    this, tr("系统提示"),
                    tr("工程文件已建立，请手动关闭当前工程文件后重新建立！"));
        return;
    }

    NewProjectDialog *pNewProjectDlg = new NewProjectDialog(this);
    if (pNewProjectDlg->exec() == QDialog::Accepted) {
        UpdateProjectName(pNewProjectDlg->GetProjectName());

        QString szPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
        QString szName = ProjectMgrUtils::getProjectNameWithOutSuffix(m_strProjectName);
        ProjectData::getInstance()->createOrOpenProjectData(szPath, szName);
        pNewProjectDlg->save();

        updateRecentProjectList(pNewProjectDlg->GetProjectName());
        CreateDefaultIOTagGroup();
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
    QString szPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
    QString szName = ProjectMgrUtils::getProjectNameWithOutSuffix(m_strProjectName);
    ProjectData::getInstance()->createOrOpenProjectData(szPath, szName);

    // 更新工程名称和工程目录(目录和名称可能人为修改)
    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    projInfoMgr.load(ProjectData::getInstance()->dbData_);
    projInfoMgr.setProjectPath(szPath);
    projInfoMgr.setProjectName(szName);
    projInfoMgr.save(ProjectData::getInstance()->dbData_);

    // 加载设备变量组信息
    UpdateDeviceVariableTableGroup();
    updateRecentProjectList(proj);
}

void MainWindow::on_actionOpenProject_triggered() {
    QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
    QString path = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "C:/");
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("选择工程文件"), path, tr("project file (*.pdt)"));
    if(fileName != nullptr) {
        doOpenProject(fileName);
    }
}

void MainWindow::on_actionWorkSpace_triggered(bool checked) {
    if (checked)
        ui->dockProjectManager->show();
    else
        ui->dockProjectManager->hide();
}

/*
* 工具条使能
*/
void MainWindow::enableToolBar(QString text) {
    bool bTagIOOrTmp = (text == tr("中间变量")) | (text.startsWith(tr("设备变量")));
    ui->TagOperateToolBar->setEnabled(bTagIOOrTmp);

    bool bdevice = (text == tr("串口设备")) | (text == tr("网络设备")) |
            (text == tr("总线设备")) | (text == tr("OPC设备"));
    ui->DeviceOperateToolBar->setEnabled(bdevice);
}

void MainWindow::onTreeViewProjectClicked(const QString &szItemText)
{
    QString winTittle = szItemText;

    ChildForm *findForm = findMdiChild(m_strProjectName);
    if(findForm == Q_NULLPTR) {
        findForm = new ChildForm(this, m_strProjectName);
        findForm->setWindowTitle(m_strProjectName);
        ui->mdiArea->addSubWindow(findForm);
        connect(this, SIGNAL(treeItemClicked(const QString &)),
                findForm, SLOT(treeItemClicked(const QString &)), Qt::DirectConnection);
    }

    ////////////////////////////////////////////////////////////////////////

    if(szItemText == tr("变量管理") || szItemText == tr("设备变量")) {
        if (findForm) findForm->hide();
        return;
    }

    if(szItemText == tr("中间变量") || szItemText == tr("系统变量")) {
        winTittle = szItemText;
    } else {
        // 设备变量
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        tagIOGroup.load(ProjectData::getInstance()->dbData_);

        foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
            if (szItemText == pObj->m_szShowName)
                winTittle = QString("%1%2%3").arg(tr("设备变量")).arg("-").arg(szItemText);
        }

        qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
        tagIOGroup.listTagIOGroupDBItem_.clear();
    }

    // 工具条使能
    enableToolBar(winTittle);

    ////////////////////////////////////////////////////////////////////////

    bool VarFound = false;
    if(szItemText == tr("中间变量") || szItemText == tr("系统变量")) {
        VarFound = true;
    } else {
        // 设备变量
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        tagIOGroup.load(ProjectData::getInstance()->dbData_);

        foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
            if (szItemText == pObj->m_szShowName)
                VarFound = true;
        }

        qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
        tagIOGroup.listTagIOGroupDBItem_.clear();
    }

    ////////////////////////////////////////////////////////////////////////

    if(szItemText == tr("系统参数")) {
        findForm->switchPage(PAGE_SYSTEM_PARAMETER);
    } else if(szItemText == tr("通讯设备") || szItemText == tr("串口设备") || szItemText == tr("网络设备")) {
        findForm->switchPage(PAGE_COMMUNICATE_DEVICE);
    } else if(VarFound) { // 变量
        if(szItemText == tr("中间变量") || szItemText == tr("系统变量")) {
            winTittle = szItemText;
        } else {
            winTittle = QString("%1%2%3").arg(tr("设备变量")).arg("-").arg(szItemText);
        }
        findForm->switchPage(PAGE_VARIABLE_MANAGER);
        if(szItemText == tr("设备变量"))
            findForm->switchPage(PAGE_NONE);
    } else if(szItemText == tr("画面")) {
        findForm->switchPage(PAGE_DRAW_PAGE);
    } else if(szItemText == tr("实时数据库")) {
        findForm->switchPage(PAGE_RTDB);
    } else if(szItemText == tr("历史数据库")) {
        findForm->switchPage(PAGE_NONE);
        findForm->hide();
    } else if(szItemText == tr("脚本编辑器")) {
        findForm->switchPage(PAGE_SCRIPT_MANAGER);
    }

    setActiveSubWindow(findForm);
    emit treeItemClicked(winTittle);
}



void MainWindow::on_treeViewProject_clicked(const QModelIndex &index)
{
    if(m_strProjectName == "")
        return;

    QStandardItem *item = pTreeViewProjectModel->itemFromIndex(index);
    onTreeViewProjectClicked(item->text());
}

void MainWindow::UpdateProjectName(QString name)
{
    if(!name.isEmpty()) {
        m_strProjectName = name;
        m_strProjectPath = ProjectMgrUtils::getProjectPath(name);
        QString strName = name.mid(name.lastIndexOf("/") + 1, name.indexOf(".") - name.lastIndexOf("/") - 1);
        pProjectItem->setText(strName);
        ui->actionRun->setEnabled(true);
    } else {
        m_strProjectName = "";
        m_strProjectPath = "";
        ui->actionRun->setEnabled(false);
        pTreeViewProjectModel->clear();
        ui->treeViewProject->reset();
        delete pTreeViewProjectModel;
        pTreeViewProjectModel = nullptr;
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
    pDevVariable->removeRows(0, pDevVariable->rowCount());

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    tagIOGroup.load(ProjectData::getInstance()->dbData_);

    foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
        QStandardItem *pDevVarTab = new QStandardItem(QIcon(":/images/pj_zone.png"), pObj->m_szShowName);
        pDevVarTab->setEditable(false);
        pDevVariableTabList.append(pDevVarTab);
        pDevVariable->appendRow(pDevVarTab);
    }
    QApplication::processEvents();

    qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
    tagIOGroup.listTagIOGroupDBItem_.clear();
}

/*
 * 所有活动窗口执行保存
 */
void MainWindow::on_actionSaveProject_triggered()
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        ChildForm *findForm = qobject_cast<ChildForm *>(window->widget());
        if(findForm != nullptr)
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
        if(findForm != nullptr)
            findForm->save();
        window->close();
    }
    UpdateProjectName(QString());
}

/*
* 插槽：退出
*/
void MainWindow::on_actionExit_triggered() {
    on_actionSaveProject_triggered();
    qApp->exit();
}

void MainWindow::on_treeViewProject_activated(const QModelIndex & /*index*/) {}

/*
 * 插槽：模拟
 */
void MainWindow::on_actionSimulate_triggered() {}

/*
 * 插槽：运行
 */
void MainWindow::on_actionRun_triggered()
{
    QString fileRuntimeApplication = "";
#ifdef Q_OS_WIN
    fileRuntimeApplication = QDir::cleanPath(Helper::AppDir() + "/../../HmiRunTimeBin/HmiRunTime.exe");
#endif

#ifdef Q_OS_LINUX
    fileRuntimeApplication = QDir::cleanPath(Helper::AppDir() + "/../../HmiRunTimeBin/HmiRunTime");
#endif

    QFile file(fileRuntimeApplication);
    if (file.exists()) {
        QProcess *process = new QProcess();
        process->setWorkingDirectory(Helper::AppDir() + "/");
        QStringList argv;
        argv << m_strProjectPath;
        process->startDetached(fileRuntimeApplication, argv);
    }
}

/*
 * 插槽：上载
 */
void MainWindow::on_actionUpLoad_triggered()
{
    // 创建tmp目录
    QString tmpDir = QCoreApplication::applicationDirPath() + "/UploadProjects/tmp";
    QDir dir(tmpDir);
    if (!dir.exists()) {
        dir.mkpath(tmpDir);
    }

    ProjectUploadDialog *pDlg = new ProjectUploadDialog(this, m_strProjectName);
    if (pDlg->exec() == QDialog::Accepted) {
        QString desDir = pDlg->getProjectPath();
#ifdef Q_OS_WIN
        QString program = QCoreApplication::applicationDirPath() + "/tar/tar.exe";
#else
		QString program = "tar";
#endif
        QFile programFile(program);
        if (!programFile.exists()) {
            QMessageBox::information(this, "系统提示", "命令：" + program + "不存在！");
            return;
        }
        QProcess *tarProc = new QProcess;
        // 设置进程工作目录
        tarProc->setWorkingDirectory(QCoreApplication::applicationDirPath() + "/tar");
        QStringList arguments;
        arguments << "-xvf"
                  << "../UploadProjects/RunProject.tar"
                  << "-C"
                  << "../UploadProjects/tmp";
        tarProc->start(program, arguments);
        if (tarProc->waitForStarted()) {
            if (tarProc->waitForFinished()) {
                QString strSrc = QCoreApplication::applicationDirPath() +
                        "/UploadProjects/tmp/RunProject";

                Helper::CopyDir(strSrc, desDir, true);
                Helper::DeleteDir(tmpDir);

                QString tarProj = QCoreApplication::applicationDirPath() +
                        "/UploadProjects/RunProject.tar";
                QFile tarProjFile(tarProj);
                if (tarProjFile.exists()) {
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
void MainWindow::on_actionUDisk_triggered() {}

/*
 * 插槽：下载
 */
void MainWindow::on_actionDownload_triggered()
{
    if(m_strProjectName == nullptr)
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
#ifdef Q_OS_WIN
    QString program = QCoreApplication::applicationDirPath() + "/tar/tar.exe";
#else
    QString program = "tar";
#endif
    QFile programFile(program);
    if(!programFile.exists()) {
        QMessageBox::information(this, "系统提示", "命令：" + program + "不存在！");
        return;
    }

    QProcess *tarProc = new QProcess;
    // 设置进程工作目录
    tarProc->setWorkingDirectory(QCoreApplication::applicationDirPath() + "/tar");
    QStringList arguments;
    arguments << "-cvf"
              << "../tmp/RunProject.tar"
              << "-C"
              << "../tmp"
              << "RunProject";
    tarProc->start(program, arguments);
    if (tarProc->waitForStarted()) {
        if (tarProc->waitForFinished(-1)) {
            // 压缩完成准备传输文件

        }
        if (tarProc->exitStatus() == QProcess::NormalExit) {

        } else {  // QProcess::CrashExit

        }
    } else {
        QMessageBox::information(this, "系统提示", "压缩工程失败！");
    }

    QDir dirRunProj(desDir);
    if (dirRunProj.exists()) {
        Helper::DeleteDir(desDir);
    }

    delete tarProc;

    ProjectDownloadDialog *pDlg =
            new ProjectDownloadDialog(this, m_strProjectName);
    pDlg->setProjFileName(QCoreApplication::applicationDirPath() + "/tmp/RunProject.tar");
    if (pDlg->exec() == QDialog::Accepted) {
    }
    delete pDlg;
}

/*
 * 插槽：增加
 */
void MainWindow::on_actionAddTag_triggered() {
    ChildForm *window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->addVariableTag();
    }
}

/*
 * 插槽：追加
 */
void MainWindow::on_actionAppendTag_triggered() {
    ChildForm *window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->appendVariableTag();
    }
}

/*
 * 插槽：行拷贝
 */
void MainWindow::on_actionRowCopyTag_triggered() {
    ChildForm *window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->rowCopyVariableTag();
    }
}

/*
 * 插槽：列拷贝
 */
void MainWindow::on_actionColumnCopyTag_triggered() {
    ChildForm *window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->columnCopyVariableTag();
    }
}

/*
 * 插槽：修改
 */
void MainWindow::on_actionModifyTag_triggered() {
    ChildForm *window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->modifyVariableTag();
    }
}

/*
 * 插槽：删除
 */
void MainWindow::on_actionDeleteTag_triggered() {
    ChildForm *window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->deleteVariableTag();
    }
}

/*
 * 插槽：导出
 */
void MainWindow::on_actionExportTag_triggered() {
    QString dirUploadProjects = QCoreApplication::applicationDirPath();
    QString strSaveCsvPath = QFileDialog::getExistingDirectory(
                this, tr("选择导出csv路径"), dirUploadProjects,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (strSaveCsvPath == "") return;

    ChildForm *window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
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

    if(strCsvName.startsWith(tr("中间变量"))) {
        ChildForm* pFindForm = findMdiChild(this->m_CurItem);
        if(pFindForm != nullptr) {
            QString titleNew = tr("中间变量");
            pFindForm->SetTitle(titleNew);
            pFindForm->variableTagImportFromCsv(strSaveCsvFile);
        }
    } else if(strCsvName.startsWith(tr("设备变量"))) {
        bool found = false;

        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        tagIOGroup.load(ProjectData::getInstance()->dbData_);

        foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
            if(strGroupName == pObj->m_szShowName) {
                found = true;
                break;
            }
        }

        qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
        tagIOGroup.listTagIOGroupDBItem_.clear();

        if(!found) {
            TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
            TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
            pObj->m_id = tagIOGroup.getGroupCount(ProjectData::getInstance()->dbData_) + 1;
            pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
            pObj->m_szShowName = strGroupName;
            tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
            if(pObj != Q_NULLPTR) {
                delete pObj;
                pObj = Q_NULLPTR;
            }
            UpdateDeviceVariableTableGroup();
            enableToolBar(strCsvName);

            onTreeViewProjectClicked(tr("设备变量"));
            QApplication::processEvents();
            onTreeViewProjectClicked(strGroupName);
            QApplication::processEvents();

            ChildForm* pFindForm = findMdiChild(this->m_CurItem);
            if(pFindForm != nullptr) {
                pFindForm->variableTagImportFromCsv(strSaveCsvFile);
            }
            return;
        }

        ChildForm* pFindForm = findMdiChild(this->m_CurItem);
        if(pFindForm != nullptr) {
            QString szWinTittle = QString("%1%2%3").arg("设备变量").arg("-").arg(strGroupName);
            pFindForm->SetTitle(szWinTittle);
            pFindForm->variableTagImportFromCsv(strSaveCsvFile);
        }
    }
}

/**
 * @brief MainWindow::on_actionDeviceNew_triggered 新建设备
 */
void MainWindow::on_actionDeviceNew_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->newDevice();
    }
}

/*
 * 插槽：修改设备
 */
void MainWindow::on_actionDeviceModify_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->modifyDevice();
    }
}

/*
 * 插槽：删除设备
 */
void MainWindow::on_actionDeviceDelete_triggered()
{
    ChildForm* window = findMdiChild(this->m_CurItem);
    if(window != nullptr) {
        window->deleteDevice();
    }
}

/*
 * 插槽：帮助
 */
void MainWindow::on_actionHelp_triggered() {}

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
    bool bStart = false;
    bool bEnd = false;

    QString iniRecentProjectFileName = Helper::AppDir() + "/RecentProjectList.ini";
    QFile fileCfg(iniRecentProjectFileName);
    if(fileCfg.exists()) {
        QStringList slist;
        ConfigUtils::getCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);

        for (int i = 0; i < slist.count(); i++) {
            if (newProj == slist.at(i)) {
                return;
            }
        }

        if (slist.count() >= 5) slist.removeLast();

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
        QAction *pActPos = Q_NULLPTR;
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
    if(findForm != nullptr) {
        findForm->showLargeIcon();
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
    if(findForm != nullptr) {
        findForm->showSmallIcon();
    }
}
