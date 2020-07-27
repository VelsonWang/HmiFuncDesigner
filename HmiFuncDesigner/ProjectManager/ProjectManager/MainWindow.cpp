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
#include "MainWindow.h"
#include "Helper.h"
#include "ProjectData.h"
#include "DrawListUtils.h"
#include "ProjectInfoManager.h"
#include "ProjectData.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "GraphPage.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFileInfo>
#include <QRect>
#include <QGraphicsView>
#include <QFileDialog>
#include <QScrollArea>
#include <QToolBar>
#include <QInputDialog>
#include <QDebug>
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_strProjectPath(""),
      m_CurItem(""),
      m_CurTreeViewItem("")
{
    ui->setupUi(this);

    currentGraphPage_ = Q_NULLPTR;
    currentView_ = Q_NULLPTR;
    gridVisible_ = true;
    currentGraphPageIndex_ = 0;

    enableToolBar("");                             // 工具条使能
    setContextMenuPolicy(Qt::DefaultContextMenu);  // 右键菜单生效
    readSettings();  // 初始窗口时读取窗口设置信息
    initWindow();    // 初始化窗口
    setUpProjectTreeView();
    loadRecentProjectList();
    on_actionBigIcon_triggered();  // 大图标显示

    //--------------------------------------------------------------------------

    szProjPath_ = m_strProjectPath;
    szProjName_ = ""; //szProjName;
    graphPageName_ =  ""; //szGraphPageName;

    undoGroup_ = new QUndoGroup(this);
    createActions();

    createMenus();
    createToolbars();
    initView();
    slotUpdateActions();

    setWindowIcon(QIcon(":/DrawAppImages/application.png"));

    connect(graphPageTabWidget_, SIGNAL(currentChanged(int)), SLOT(slotChangeGraphPage(int)));

    QDesktopWidget * pDesktopWidget = QApplication::desktop();
    QRect rect = pDesktopWidget->screenGeometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    this->resize(screenWidth*3/4, screenHeight*3/4);

    Helper::WidgetMoveCenter(this);

    DrawListUtils::setProjectPath(szProjPath_);

    ui->listWidgetGraphPages->setContextMenuPolicy(Qt::DefaultContextMenu);
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


#if 0

    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(tr("新建"), this);
    pNewAct->setStatusTip(tr("新建画面"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(onNewGraphPage()));
    pMenu->addAction(pNewAct);

    QAction *pRenameAct = new QAction(tr("重命名"), this);
    pRenameAct->setStatusTip(tr("重命名画面"));
    connect(pRenameAct, SIGNAL(triggered()), this, SLOT(onRenameGraphPage()));
    pMenu->addAction(pRenameAct);

    QAction *pDeleteAct = new QAction(tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除画面"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(onDeleteGraphPage()));
    pMenu->addAction(pDeleteAct);

    QAction *pCopyAct = new QAction(tr("复制"), this);
    pCopyAct->setStatusTip(tr("复制画面"));
    connect(pCopyAct, SIGNAL(triggered()), this, SLOT(onCopyGraphPage()));
    pMenu->addAction(pCopyAct);

    QAction *pPasteAct = new QAction(tr("粘贴"), this);
    pPasteAct->setStatusTip(tr("粘贴画面"));
    connect(pPasteAct, SIGNAL(triggered()), this, SLOT(onPasteGraphPage()));
    pMenu->addAction(pPasteAct);

    pMenu->move(cursor().pos());
    pMenu->show();

#endif
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


#if 0
    bool unsaved = false;

    QListIterator<GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        GraphPage *graphPage = it.next();
        if (!graphPage->undoStack()->isClean() || graphPage->getUnsavedFlag()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int ret = exitResponse();

        if (ret == QMessageBox::Yes) {
            slotSaveGraphPage();
            event->accept();
        } else if (ret == QMessageBox::No) {
            event->accept();
        }
    } else {
        event->accept();
    }
#endif
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
        QString program = QCoreApplication::applicationDirPath() + "/tar/tar.exe";
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


//------------------------------------------------------------------------------

void MainWindow::initView()
{
    graphPageTabWidget_ = new QTabWidget(this);
    graphPageTabWidget_->installEventFilter(this);
    ui->scrollArea->setWidget(graphPageTabWidget_);

    elementWidget_ = new ElementLibraryWidget();
    ui->ElemetsLayout->addWidget(elementWidget_);

    variantEditorFactory_ = new VariantFactory(this);

    //propertyEditor_ = new QtTreePropertyBrowser(dockProperty);
    propertyEditor_ = new QtTreePropertyBrowser(this);
    propertyEditor_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    propertyEditor_->setHeaderLabels(QStringList() << tr("属性") << tr("值"));
    //propertyEditor_->setColumnWidth(0, 60);
    //propertyEditor_->setColumnWidth(1, 200);


    ui->PropertyLayout->addWidget(propertyEditor_);

    VariantManager *pVariantManager  = new VariantManager(this);
    variantPropertyManager_ = pVariantManager;
    pVariantManager->setPropertyEditor(propertyEditor_);
    propertyEditor_->setFactoryForManager(variantPropertyManager_, variantEditorFactory_);

    connect(variantPropertyManager_, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));


    objTree_ = new ObjectsTreeView();
    ui->ObjectTreeLayout->addWidget(objTree_);
    slotShowTreeObj(false);
}

void MainWindow::createActions()
{
    actionShowGraphObj_ = new QAction(tr("图形元素"), this);
    actionShowGraphObj_->setCheckable(true);
    actionShowGraphObj_->setChecked(true);
    connect(actionShowGraphObj_, SIGNAL(triggered(bool)), SLOT(slotShowGraphObj(bool)));

    actionShowTreeObj_ = new QAction(tr("对象树"), this);
    actionShowTreeObj_->setCheckable(true);
    actionShowTreeObj_->setChecked(false);
    connect(actionShowTreeObj_, SIGNAL(triggered(bool)), SLOT(slotShowTreeObj(bool)));

    actionShowPropEditor_ = new QAction(tr("属性编辑器"), this);
    actionShowPropEditor_->setCheckable(true);
    actionShowPropEditor_->setChecked(true);
    connect(actionShowPropEditor_, SIGNAL(triggered(bool)), SLOT(slotShowPropEditor(bool)));

    actionNew_ = new QAction(QIcon(":/DrawAppImages/filenew.png"), tr("新建"), this);
    actionNew_->setShortcut(QString("Ctrl+N"));
    connect(actionNew_, SIGNAL(triggered()), SLOT(slotEditNew()));

    actionOpen_ = new QAction(QIcon(":/DrawAppImages/fileopen.png"), tr("打开"), this);
    actionOpen_->setShortcut(QString("Ctrl+O"));
    connect(actionOpen_, SIGNAL(triggered()), SLOT(slotEditOpen()));

    actionSaveGraphPage_ = new QAction(QIcon(":/DrawAppImages/saveproject.png"), tr("保存"), this);
    actionSaveGraphPage_->setShortcut(QKeySequence::Save);
    connect(actionSaveGraphPage_, SIGNAL(triggered()), SLOT(slotSaveGraphPage()));

    actionCloseGraphPage_ = new QAction(tr("关闭"), this);
    connect(actionCloseGraphPage_, SIGNAL(triggered()), SLOT(slotCloseGraphPage()));

    actionCloseAll_ = new QAction(tr("关闭所有"), this);
    connect(actionCloseAll_, SIGNAL(triggered()), SLOT(slotCloseAll()));

    actionExit_ = new QAction(tr("退出"),this);
    actionExit_->setShortcut(QKeySequence::Quit);
    connect(actionExit_,SIGNAL(triggered()),SLOT(slotExit()));

    actionShowGrid_ = new QAction(QIcon(":/DrawAppImages/showgrid.png"), tr("显示栅格"), this);
    actionShowGrid_->setCheckable(true);
    actionShowGrid_->setChecked(gridVisible_);
    connect(actionShowGrid_, SIGNAL(triggered(bool)), SLOT(slotShowGrid(bool)));

    actionShowLinear_ = new QAction(QIcon(":/DrawAppImages/ruler.png"), tr("显示线条"), this);
    actionShowLinear_->setCheckable(true);
    actionShowLinear_->setChecked(false);
    connect(actionShowLinear_, SIGNAL(triggered(bool)), SLOT(slotShowLinear(bool)));

    actionZoomIn_ = new QAction(QIcon(":/DrawAppImages/zoom-in.png"), tr("放大"), this);
    connect(actionZoomIn_, SIGNAL(triggered()), SLOT(slotZoomIn()));

    actionZoomOut_ = new QAction(QIcon(":/DrawAppImages/zoom-out.png"), tr("缩小"), this);
    connect(actionZoomOut_, SIGNAL(triggered()), SLOT(slotZoomOut()));

    actionUndo_ = undoGroup_->createUndoAction(this);
    actionUndo_->setIcon(QIcon(":/DrawAppImages/undo.png"));
    actionUndo_->setText(tr("撤销"));
    actionUndo_->setShortcut(QKeySequence::Undo);

    actionRedo_ = undoGroup_->createRedoAction(this);
    actionRedo_->setText(tr("重做"));
    actionRedo_->setIcon(QIcon(":/DrawAppImages/redo.png"));
    actionRedo_->setShortcut(QKeySequence::Redo);

    actionDelete_ = new QAction(QIcon(":/DrawAppImages/delete.png"), tr("删除"));
    actionDelete_->setShortcut(QKeySequence::Delete);
    connect(actionDelete_, SIGNAL(triggered()), SLOT(slotEditDelete()));

    actionCopy_ = new QAction(QIcon(":/DrawAppImages/editcopy.png"),tr("拷贝"));
    actionCopy_->setShortcut(QKeySequence::Copy);
    connect(actionCopy_, SIGNAL(triggered()), SLOT(slotEditCopy()));

    actionPaste_ = new QAction(QIcon(":/DrawAppImages/editpaste.png"),tr("粘贴"));
    actionPaste_->setShortcut(QKeySequence::Paste);
    connect(actionPaste_, SIGNAL(triggered()), SLOT(slotEditPaste()));

    // 顶部对齐
    alignTopAction_ = new QAction(QIcon(":/DrawAppImages/align-top.png"), tr("顶部对齐"));
    alignTopAction_->setData(Qt::AlignTop);
    connect(alignTopAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 底部对齐
    alignDownAction_ = new QAction(QIcon(":/DrawAppImages/align-bottom.png"), tr("底部对齐"));
    alignDownAction_->setData(Qt::AlignBottom);
    connect(alignDownAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 右对齐
    alignRightAction_ = new QAction(QIcon(":/DrawAppImages/align-right.png"), tr("右对齐"));
    alignRightAction_->setData(Qt::AlignRight);
    connect(alignRightAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 左对齐
    alignLeftAction_ = new QAction(QIcon(":/DrawAppImages/align-left.png"), tr("左对齐"));
    alignLeftAction_->setData(Qt::AlignLeft);
    connect(alignLeftAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 水平均匀分布
    hUniformDistributeAction_ = new QAction(QIcon(":/DrawAppImages/align_hsame.png"), tr("水平均匀分布"));
    connect(hUniformDistributeAction_, SIGNAL(triggered()), SLOT(slotHUniformDistributeElements()));

    // 垂直均匀分布
    vUniformDistributeAction_ = new QAction(QIcon(":/DrawAppImages/align_vsame.png"), tr("垂直均匀分布"));
    connect(vUniformDistributeAction_, SIGNAL(triggered()), SLOT(slotVUniformDistributeElements()));

    // 设置选中控件大小一致
    setTheSameSizeAction_ = new QAction(QIcon(":/DrawAppImages/the-same-size.png"), tr("大小一致"));
    connect(setTheSameSizeAction_, SIGNAL(triggered()), SLOT(slotSetTheSameSizeElements()));

    // 上移一层
    upLayerAction_ = new QAction(QIcon(":/DrawAppImages/posfront.png"), tr("上移一层"));
    connect(upLayerAction_, SIGNAL(triggered()), SLOT(slotUpLayerElements()));

    // 下移一层
    downLayerAction_ = new QAction(QIcon(":/DrawAppImages/posback.png"), tr("下移一层"));
    connect(downLayerAction_, SIGNAL(triggered()), SLOT(slotDownLayerElements()));

}

void MainWindow::createMenus()
{
    QMenu *filemenu = new QMenu(tr("文件"), this);
#if 0  // for test we need this
    filemenu->addAction(actionNew);
    filemenu->addAction(actionOpen);
#endif
    filemenu->addAction(actionSaveGraphPage_);
    filemenu->addSeparator();
    filemenu->addAction(actionCloseGraphPage_);
    filemenu->addAction(actionCloseAll_);
    filemenu->addSeparator();
    filemenu->addAction(actionExit_);

    QMenu *windowMenu = new QMenu(tr("窗口"), this);
    windowMenu->addAction(actionShowGraphObj_);
    windowMenu->addAction(actionShowTreeObj_);
    windowMenu->addAction(actionShowPropEditor_);

    QMainWindow::menuBar()->addMenu(filemenu);
    QMainWindow::menuBar()->addMenu(windowMenu);
}

void MainWindow::createToolbars()
{
    ui->toolBar->addAction(actionSaveGraphPage_);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionShowGrid_);
    //toolBar->addAction(actionShowLinear);
    ui->toolBar->addAction(actionZoomOut_);
    ui->toolBar->addAction(actionZoomIn_);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionUndo_);
    ui->toolBar->addAction(actionRedo_);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionCopy_); // 拷贝
    ui->toolBar->addAction(actionPaste_); // 粘贴
    ui->toolBar->addAction(actionDelete_); // 删除
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(alignTopAction_); // 顶部对齐
    ui->toolBar->addAction(alignDownAction_); // 底部对齐
    ui->toolBar->addAction(alignLeftAction_); // 左对齐
    ui->toolBar->addAction(alignRightAction_); // 右对齐
    ui->toolBar->addAction(hUniformDistributeAction_); // 水平均匀分布
    ui->toolBar->addAction(vUniformDistributeAction_); // 垂直均匀分布
    ui->toolBar->addAction(setTheSameSizeAction_); // 设置选中控件大小一致
    ui->toolBar->addAction(upLayerAction_); // 上移一层
    ui->toolBar->addAction(downLayerAction_); // 下移一层
    ui->toolBar->addSeparator();
}



/**
 * @brief openGraphPage
 * @details 打开画面
 * @param pagePath 画面路径
 * @param pagePath 画面名称
 */
void MainWindow::openGraphPage(const QString &szProjPath,
                               const QString &szProjName,
                               const QString &szPageName)
{
    DrawListUtils::loadDrawList(szProjPath);
    foreach(QString szPageId, DrawListUtils::drawList_) {
        ui->listWidgetGraphPages->addItem(szPageId);
        QString fileName = szProjPath + "/" + szPageId + ".drw";

        if (fileName.toLower().endsWith(".drw")) {
            QGraphicsView *view = createTabView();

            if (graphPageTabWidget_->indexOf(view) != -1) {
                delete view;
                return;
            }

            GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
            if (!createDocument(graphPage, view, fileName)) {
                return;
            }

            if(szPageId == szPageName) {
                currentGraphPage_ = graphPage;
                currentView_ = dynamic_cast<QGraphicsView *>(view);
            }

            graphPage->setProjectPath(szProjPath);
            graphPage->setProjectName(szProjName);
            graphPage->setGridVisible(gridVisible_);
            graphPage->loadAsXML(fileName);
            view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
            graphPage->setFileName(szPageId + ".drw");
            graphPage->setGraphPageId(szPageId);
            graphPage->fillGraphPagePropertyModel();
        }
    }

    QList<QListWidgetItem*> listWidgetItem = ui->listWidgetGraphPages->findItems(szPageName, Qt::MatchCaseSensitive);
    if ( listWidgetItem.size() > 0 ) {
        ui->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
    }

}


void MainWindow::slotEditNew()
{
    addNewGraphPage();
}

QString MainWindow::fixedWindowTitle(const QGraphicsView *viewGraphPage) const
{
    QString title = currentGraphPage_->getGraphPageId();

    if (title.isEmpty()) {
        title = "Untitled";
    } else {
        title = QFileInfo(title).fileName();
    }

    QString result;

    for (int i = 0; ;++i) {
        result = title;

        if (i > 0) {
            result += QString::number(i);
        }

        bool unique = true;

        for (int j = 0; j < graphPageTabWidget_->count(); ++j) {
            const QWidget *widget = graphPageTabWidget_->widget(j);

            if (widget == viewGraphPage) {
                continue;
            }

            if (result == graphPageTabWidget_->tabText(j)) {
                unique = false;
                break;
            }
        }

        if (unique) {
            break;
        }
    }

    return result;
}

bool MainWindow::isGraphPageOpen(const QString &filename)
{
    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        if (filename == it.next()->getFileName()) {
            return true;
        }
    }

    return false;
}

void MainWindow::addNewGraphPage()
{
    QGraphicsView *view = createTabView();

    if (graphPageTabWidget_->indexOf(view) != -1) {
        delete view;
        return;
    }

    GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
    graphPage->setProjectPath(szProjPath_);
    graphPage->setProjectName(szProjName_);
    graphPage->setGridVisible(gridVisible_);
    currentGraphPage_ = graphPage;
    view->setScene(graphPage);
    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    currentView_ = dynamic_cast<QGraphicsView *>(view);
    QString title = fixedWindowTitle(view);
    graphPage->setFileName(title + ".drw");
    graphPage->setGraphPageId(title);
    graphPageTabWidget_->addTab(currentView_, title);
    graphPageTabWidget_->setCurrentWidget(currentView_);
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    undoGroup_->addStack(graphPage->undoStack());
    undoGroup_->setActiveStack(graphPage->undoStack());

    connectGraphPage(graphPage);
}

void MainWindow::connectGraphPage(GraphPage *graphPage)
{
    connect(graphPage->undoStack(), SIGNAL(indexChanged(int)), SLOT(slotUpdateActions()));
    connect(graphPage->undoStack(), SIGNAL(cleanChanged(bool)), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(newElementAdded()), SLOT(slotNewElementAdded()));
    connect(graphPage, SIGNAL(elementsDeleted()), SLOT(slotElementsDeleted()));
    connect(graphPage, SIGNAL(elementIdChanged()), SLOT(slotElementIdChanged()));
    connect(graphPage, SIGNAL(changeGraphPageName()), SLOT(slotChangeGraphPageName()));
    connect(graphPage, SIGNAL(selectionChanged()), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(elementPropertyChanged()), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(GraphPagePropertyChanged()), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(GraphPageSaved()), SLOT(slotUpdateActions()));
}

void MainWindow::disconnectGraphPage(GraphPage *graphPage)
{
    disconnect(graphPage->undoStack(), SIGNAL(indexChanged(int)), this, SLOT(slotUpdateActions()));
    disconnect(graphPage->undoStack(), SIGNAL(cleanChanged(bool)), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(newElementAdded()), this, SLOT(slotNewElementAdded()));
    disconnect(graphPage, SIGNAL(elementsDeleted()), this, SLOT(slotElementsDeleted()));
    disconnect(graphPage, SIGNAL(elementIdChanged()), this, SLOT(slotElementIdChanged()));
    disconnect(graphPage, SIGNAL(changeGraphPageName()), this, SLOT(slotChangeGraphPageName()));
    disconnect(graphPage, SIGNAL(selectionChanged()), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(elementPropertyChanged()), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(GraphPagePropertyChanged()), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(GraphPageSaved()), this, SLOT(slotUpdateActions()));
}

QGraphicsView *MainWindow::createTabView()
{
    QGraphicsView *view = new QGraphicsView();
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    return view;
}

void MainWindow::slotUpdateActions()
{
    static const QIcon unsaved(":/DrawAppImages/filesave.png");

    for (int i = 0; i < graphPageTabWidget_->count(); i++) {
        QGraphicsView *view = dynamic_cast<QGraphicsView*>(graphPageTabWidget_->widget(i));
        if (!dynamic_cast<GraphPage *>(view->scene())->undoStack()->isClean() ||
            dynamic_cast<GraphPage *>(view->scene())->getUnsavedFlag()) {
            graphPageTabWidget_->setTabIcon(graphPageTabWidget_->indexOf(view), unsaved);
        } else {
            graphPageTabWidget_->setTabIcon(graphPageTabWidget_->indexOf(view), QIcon());
        }
    }

    actionZoomIn_->setEnabled(graphPageTabWidget_->count() ? true : false);
    actionZoomOut_->setEnabled(graphPageTabWidget_->count() ? true : false);
    actionShowGrid_->setEnabled(graphPageTabWidget_->count() ? true : false);

    if (!currentGraphPage_) {
        return;
    }

    undoGroup_->setActiveStack(currentGraphPage_->undoStack());

    if (!currentGraphPage_->undoStack()->isClean() || currentGraphPage_->getUnsavedFlag()) {
        actionSaveGraphPage_->setEnabled(true);
    } else {
        actionSaveGraphPage_->setEnabled(false);
    }
}

void MainWindow::slotChangeGraphPage(int GraphPageNum)
{
    if (GraphPageNum == -1) {
        objTree_->clearModel();
        return;
    }

    ui->listWidgetGraphPages->setCurrentRow(GraphPageNum);

    for (int i = 0; i < graphPageTabWidget_->count(); i++) {
        QGraphicsView *view = dynamic_cast<QGraphicsView *>(graphPageTabWidget_->widget(i));
        dynamic_cast<GraphPage *>(view->scene())->setActive(false);
    }

    currentView_ = dynamic_cast<QGraphicsView *>(graphPageTabWidget_->widget(GraphPageNum));
    currentGraphPage_ = dynamic_cast<GraphPage *>(currentView_->scene());
    currentGraphPage_->setActive(true);
    //currentGraphPage_->fillGraphPagePropertyModel();
    objTree_->setContentList(currentGraphPage_->items());
    objTree_->updateContent();
    currentGraphPageIndex_ = GraphPageNum;
    slotUpdateActions();
}

void MainWindow::slotChangeGraphPageName()
{
    graphPageTabWidget_->setTabText(currentGraphPageIndex_, currentGraphPage_->getGraphPageId());
    //int index = GraphPageManager::getInstance()->getIndexByGraphPage(currentGraphPage_);
}

void MainWindow::updateObjectTree()
{
    objTree_->setContentList(currentGraphPage_->items());
    objTree_->updateContent();
}

void MainWindow::slotElementIdChanged()
{
    updateObjectTree();
}

void MainWindow::slotElementPropertyChanged()
{

}

void MainWindow::slotGraphPagePropertyChanged()
{

}

void MainWindow::propertyValueChanged(QtProperty *property, const QVariant &value)
{
    Q_UNUSED(property)
    Q_UNUSED(value)
}

void MainWindow::slotNewElementAdded()
{
    updateObjectTree();
}

void MainWindow::slotElementsDeleted()
{
    updateObjectTree();
}

void MainWindow::slotShowGrid(bool on)
{
    QListIterator <GraphPage*> iter(GraphPageManager::getInstance()->getGraphPageList());

    while (iter.hasNext()) {
        iter.next()->setGridVisible(on);
    }

    gridVisible_ = on;
}

void MainWindow::slotShowGraphObj(bool on)
{
    on ? ui->dockElemets->show() : ui->dockElemets->hide();
}

void MainWindow::slotShowTreeObj(bool on)
{
    on ? ui->dockObjectTree->show() : ui->dockObjectTree->hide();
}

void MainWindow::slotShowPropEditor(bool on)
{
    on ? ui->dockProperty->show() : ui->dockProperty->hide();
}


void MainWindow::slotCloseAll()
{
    while (graphPageTabWidget_->count()) {
        QGraphicsView *view = static_cast<QGraphicsView*>(graphPageTabWidget_->widget(graphPageTabWidget_->currentIndex()));
        removeGraphPage(view);
        delete view;
    }

    currentView_ = Q_NULLPTR;
    currentGraphPage_ = Q_NULLPTR;
    slotUpdateActions();
}

void MainWindow::removeGraphPage(QGraphicsView *view)
{
    int index = graphPageTabWidget_->indexOf(view);
    GraphPage *graphPage = static_cast<GraphPage*>(view->scene());

    if (index == -1)
        return;

    if (!graphPage->undoStack()->isClean()) {
        int ret = exitResponse();

        if (ret == QMessageBox::Yes) {
            slotSaveGraphPage();
        }
    }

    graphPageTabWidget_->removeTab(index);
    undoGroup_->removeStack(graphPage->undoStack());
    GraphPageManager::getInstance()->removeGraphPage(graphPage);
    disconnectGraphPage(graphPage);
    delete graphPage;
}

void MainWindow::slotCloseGraphPage()
{
    QGraphicsView *view = currentView_;
    removeGraphPage(view);
    delete view;

    if (graphPageTabWidget_->count() == 0) {
        currentGraphPage_ = Q_NULLPTR;
        currentView_ = Q_NULLPTR;
    }

    slotUpdateActions();
}

void MainWindow::slotEditOpen()
{
    const QString &filename = QFileDialog::getOpenFileName(this,
                                                           tr("Open"),
                                                           ".",
                                                           tr("GraphPage (*.drw)"));
    if (filename.isEmpty())
        return;
#if 0
    if (filename.toLower().endsWith(".drwb")) {

        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF());
        if (!createDocument(graphPage,view,filename)) {
            return;
        }
        graphPage->loadAsBinary(filename);
    }
#endif
    if (filename.toLower().endsWith(".drw")) {

        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
        if (!createDocument(graphPage, view, filename)) {
            return;
        }

        currentGraphPage_ = graphPage;
        currentView_ = dynamic_cast<QGraphicsView *>(view);
        graphPage->setProjectPath(szProjPath_);
        graphPage->setProjectName(szProjName_);
        graphPage->loadAsXML(filename);
        int pos = filename.lastIndexOf("/");
        QString pageFileName = "";
        if (pos != -1) {
            pageFileName = filename.right(filename.length() - pos - 1);
        }
        graphPage->setFileName(pageFileName);
        graphPage->setGraphPageId(pageFileName.left(pageFileName.length() - 4));
    }
}

bool MainWindow::createDocument(GraphPage *graphPage,
                                QGraphicsView *view,
                                const QString &filename)
{
    if (isGraphPageOpen(filename)) {
        QMessageBox::information(this,
                                 tr("打开文件错误"),
                                 tr("文件已打开"),
                                 QMessageBox::Ok);
        delete graphPage;
        delete view;
        return false;
    }

    graphPage->setGridVisible(gridVisible_);
    view->setScene(graphPage);
    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    graphPageTabWidget_->addTab(view, graphPage->getGraphPageId());
    graphPageTabWidget_->setCurrentWidget(view);
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    undoGroup_->addStack(graphPage->undoStack());
    undoGroup_->setActiveStack(graphPage->undoStack());

    connectGraphPage(graphPage);

    graphPage->undoStack()->setClean();

    return true;
}


QString MainWindow::getFileName()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save as"),
                                                    QString("./%1").arg(currentGraphPage_->getGraphPageId()),
                                                    tr("GraphPage(*.drw)"));
    return filename;
}

void MainWindow::updateGraphPageViewInfo(const QString &fileName)
{
    int index = graphPageTabWidget_->indexOf(currentView_);
    QFileInfo file(fileName);
    currentGraphPage_->setGraphPageId(file.baseName());
    graphPageTabWidget_->setTabText(index,file.baseName());
    slotChangeGraphPageName();
}

void MainWindow::slotSaveGraphPage()
{
    if (!currentGraphPage_) {
        return;
    }

    for (;;) {
        QString fileName = currentGraphPage_->getFileName();

        if (fileName.isEmpty())
            fileName = getFileName();

        if (fileName.isEmpty())
            break;

        currentGraphPage_->setFileName(fileName);
        updateGraphPageViewInfo(fileName);
        currentGraphPage_->saveAsXML(szProjPath_ + "/" + fileName);
#if 0
        if (fileName.toLower().endsWith(".drw")) {
            QString binaryFileName = fileName.toLower()+ "b"; // ".drw"==>".drwb"
            currentGraphPage->saveAsBinary(szProjPath_ + "/" + binaryFileName);
        }
#endif
        break;

    }
}

void MainWindow::slotExit()
{
    if (graphPageTabWidget_->count() == 0) {
        QApplication::quit();
        return;
    }

    bool unsaved = false;

    QListIterator<GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        if (!it.next()->undoStack()->isClean()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int ret = exitResponse();
        if (ret == QMessageBox::Yes) {
            slotSaveGraphPage();
            QApplication::quit();
        } else if (ret == QMessageBox::No) {
            QApplication::quit();
        }
    }

    QApplication::quit();
}

int MainWindow::exitResponse()
{
    int ret = QMessageBox::information(this,
                                       tr("退出程序"),
                                       tr("文件已修改。是否保存?"),
                                       QMessageBox::Yes | QMessageBox::No);
    return ret;
}

void MainWindow::slotShowLinear(bool on) {
    Q_UNUSED(on)
}

void MainWindow::slotZoomIn()
{
    if(currentGraphPage_ != nullptr) {
        int width = currentGraphPage_->getGraphPageWidth();
        int height = currentGraphPage_->getGraphPageHeight();
        currentGraphPage_->setGraphPageWidth(static_cast<int>(width * 1.25));
        currentGraphPage_->setGraphPageHeight(static_cast<int>(height * 1.25));
        currentGraphPage_->setGridVisible(currentGraphPage_->isGridVisible());
    }
    if (currentView_ != nullptr) {
        currentView_->scale(1.25, 1.25);
        currentView_->setFixedSize(currentGraphPage_->getGraphPageWidth(), currentGraphPage_->getGraphPageHeight());
    }
}

void MainWindow::slotZoomOut()
{
    if(currentGraphPage_ != nullptr) {
        int width = currentGraphPage_->getGraphPageWidth();
        int height = currentGraphPage_->getGraphPageHeight();
        currentGraphPage_->setGraphPageWidth(static_cast<int>(width * 1/1.25));
        currentGraphPage_->setGraphPageHeight(static_cast<int>(height * 1/1.25));
        currentGraphPage_->setGridVisible(currentGraphPage_->isGridVisible());
    }
    if (currentView_ != nullptr) {
        currentView_->scale(1/1.25, 1/1.25);
        currentView_->setFixedSize(currentGraphPage_->getGraphPageWidth(), currentGraphPage_->getGraphPageHeight());
    }
}


/**
 * @brief MainWindow::slotAlignElements
 * @details 顶部对齐, 底部对齐, 右对齐, 左对齐
 */
void MainWindow::slotAlignElements()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    Qt::Alignment alignment = static_cast<Qt::Alignment>(action->data().toInt());
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onAlignElements(alignment, items);
    }
}


/**
 * @brief MainWindow::slotHUniformDistributeElements
 * @details 水平均匀分布
 */
void MainWindow::slotHUniformDistributeElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onHUniformDistributeElements(items);
    }
}


/**
 * @brief MainWindow::slotVUniformDistributeElements
 * @details 垂直均匀分布
 */
void MainWindow::slotVUniformDistributeElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onVUniformDistributeElements(items);
    }
}


/**
 * @brief MainWindow::slotSetTheSameSizeElements
 * @details 设置选中控件大小一致
 */
void MainWindow::slotSetTheSameSizeElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onSetTheSameSizeElements(items);
    }
}


/**
 * @brief MainWindow::slotUpLayerElements
 * @details 上移一层
 */
void MainWindow::slotUpLayerElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onUpLayerElements(items);
    }
}


/**
 * @brief MainWindow::slotDownLayerElements
 * @details 下移一层
 */
void MainWindow::slotDownLayerElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onDownLayerElements(items);
    }
}


/**
 * @brief MainWindow::slotEditDelete
 * @details 删除
 */
void MainWindow::slotEditDelete()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onEditDelete(items);
    }
}


/**
 * @brief MainWindow::slotEditCopy
 * @details 拷贝
 */
void MainWindow::slotEditCopy()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onEditCopy(items);
    }
}


/**
 * @brief MainWindow::slotEditPaste
 * @details 粘贴
 */
void MainWindow::slotEditPaste()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onEditPaste();
    }
}


/**
 * @brief MainWindow::on_listWidgetGraphPages_itemClicked
 * @details 画面名称被单击
 * @param item
 */
void MainWindow::on_listWidgetGraphPages_currentTextChanged(const QString &currentText)
{
    Q_UNUSED(currentText)
    graphPageTabWidget_->setCurrentIndex(ui->listWidgetGraphPages->currentRow());
}


/**
 * @brief MainWindow::createEmptyGraphpage
 * @details 创建空的画面页
 * @param projPath 工程路径
 * @param graphPageName 画面名称
 * @param width 画面宽度
 * @param height 画面高度
 */
void MainWindow::createEmptyGraphpage(const QString &projPath,
                                      const QString &graphPageName,
                                      int width,
                                      int height)
{
    QString fileName = projPath + "/" + graphPageName + ".drw";
    QString szContent = QString(
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<graphPage fileName=\"%1.drw\" graphPageId=\"%1\" "
                "width=\"%2\" height=\"%3\" background=\"#ffffff\">\n"
                "</graphPage>")
            .arg(graphPageName)
            .arg(QString::number(width))
            .arg(QString::number(height));

    Helper::writeString(fileName, szContent);
}


/**
 * @brief MainWindow::onNewGraphPage
 * @details 新建画面
 */
void MainWindow::onNewGraphPage()
{
    int last = DrawListUtils::getMaxDrawPageNum("draw");
    QString szGraphPageName = QString("draw%1").arg(last);

    QInputDialog dlg(this);
    dlg.setWindowTitle(tr("画面名称"));
    dlg.setLabelText(tr("请输入画面名称"));
    dlg.setOkButtonText(tr("确定"));
    dlg.setCancelButtonText(tr("取消"));
    dlg.setTextValue(szGraphPageName);

reInput:
    if ( dlg.exec() == QDialog::Accepted ) {
        szGraphPageName = dlg.textValue();
        if ( szGraphPageName == "" ) {
            goto reInput;
        }

        QList<GraphPage*> listGraphPage = GraphPageManager::getInstance()->getGraphPageList();

        int width = 480;
        int height = 272;
        if ( listGraphPage.size() > 0 ) {
            GraphPage* pGraphPage = listGraphPage.at(0);
            width = pGraphPage->getGraphPageWidth();
            height = pGraphPage->getGraphPageHeight();
        }

        createEmptyGraphpage(szProjPath_, szGraphPageName, width, height);
        DrawListUtils::drawList_.append(szGraphPageName);
        DrawListUtils::saveDrawList(szProjPath_);

        ui->listWidgetGraphPages->addItem(szGraphPageName);
        QString fileName = szProjPath_ + "/" + szGraphPageName + ".drw";

        if (fileName.toLower().endsWith(".drw")) {
            QGraphicsView *view = createTabView();

            if (graphPageTabWidget_->indexOf(view) != -1) {
                delete view;
                return;
            }

            GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
            if (!createDocument(graphPage, view, fileName)) {
                return;
            }

            currentGraphPage_ = graphPage;
            currentView_ = dynamic_cast<QGraphicsView *>(view);
            graphPage->setProjectPath(szProjPath_);
            graphPage->setProjectName(szProjName_);
            graphPage->loadAsXML(fileName);
            view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
            graphPage->setFileName(szGraphPageName + ".drw");
            graphPage->setGraphPageId(szGraphPageName);
        }

        QList<QListWidgetItem*> listWidgetItem = ui->listWidgetGraphPages->findItems(szGraphPageName, Qt::MatchCaseSensitive);
        if ( listWidgetItem.size() > 0 ) {
            ui->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
            graphPageTabWidget_->setCurrentIndex(ui->listWidgetGraphPages->currentRow());
        }
    }
}


/**
 * @brief MainWindow::onRenameGraphPage
 * @details 修改画面名称
 */
void MainWindow::onRenameGraphPage()
{
    QString szOldGraphPageName = ui->listWidgetGraphPages->currentItem()->text();

    QInputDialog dlg(this);
    dlg.setWindowTitle(tr("修改画面名称"));
    dlg.setLabelText(tr("请输入画面名称"));
    dlg.setOkButtonText(tr("确定"));
    dlg.setCancelButtonText(tr("取消"));
    dlg.setTextValue(szOldGraphPageName);

reInput:
    if (dlg.exec() == QDialog::Accepted) {
        QString szNewGraphPageName = dlg.textValue();

        if (szNewGraphPageName == "") {
            goto reInput;
        }

        for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
            if ( szOldGraphPageName == DrawListUtils::drawList_.at(i) ) {
                DrawListUtils::drawList_.replace(i, szNewGraphPageName);
                QString szOldName = szProjPath_ + "/" + szOldGraphPageName + ".drw";
                QString szNewName = szProjPath_ + "/" + szNewGraphPageName + ".drw";
                QFile::rename(szOldName, szNewName);
                DrawListUtils::saveDrawList(szProjPath_);
                ui->listWidgetGraphPages->currentItem()->setText(szNewGraphPageName);
                GraphPage *pGraphPage = GraphPageManager::getInstance()->getGraphPageById(szOldGraphPageName);
                pGraphPage->setFileName(szNewGraphPageName + ".drw");
                pGraphPage->setGraphPageId(szNewGraphPageName);
                graphPageTabWidget_->setTabText(graphPageTabWidget_->currentIndex(), szNewGraphPageName);
                currentGraphPage_->setUnsavedFlag(true);
                slotUpdateActions();
                break;
            }
        }
    }
}


/**
 * @brief MainWindow::onDeleteGraphPage
 * @details 删除画面
 */
void MainWindow::onDeleteGraphPage()
{
    QString szGraphPageName = ui->listWidgetGraphPages->currentItem()->text();

    for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
        if ( szGraphPageName == DrawListUtils::drawList_.at(i) ) {
            DrawListUtils::drawList_.removeAt(i);

            QString fileName = szProjPath_ + "/" + szGraphPageName + ".drw";
            QFile file(fileName);
            if (file.exists()) {
                file.remove();
            }

            graphPageTabWidget_->removeTab(ui->listWidgetGraphPages->currentRow());

            GraphPage *pGraphPageObj = GraphPageManager::getInstance()->getGraphPageById(szGraphPageName);
            if ( pGraphPageObj != Q_NULLPTR ) {
                GraphPageManager::getInstance()->removeGraphPage(pGraphPageObj);
                delete pGraphPageObj;
                pGraphPageObj = Q_NULLPTR;
            }

            DrawListUtils::saveDrawList(szProjPath_);

            ui->listWidgetGraphPages->clear();
            foreach(QString szPageId, DrawListUtils::drawList_) {
                ui->listWidgetGraphPages->addItem(szPageId);
            }

            if (ui->listWidgetGraphPages->count() > 0) {
                ui->listWidgetGraphPages->setCurrentRow(0);
                graphPageTabWidget_->setCurrentIndex(0);
            }

            break;
        }
    }
}


/**
 * @brief MainWindow::onCopyGraphPage
 * @details 复制画面
 */
void MainWindow::onCopyGraphPage()
{
    szCopyGraphPageFileName_ = ui->listWidgetGraphPages->currentItem()->text();
}


/**
 * @brief MainWindow::onPasteGraphPage
 * @details 粘贴画面
 */
void MainWindow::onPasteGraphPage()
{
    int iLast = 0;

reGetNum:
    iLast = DrawListUtils::getMaxDrawPageNum(szCopyGraphPageFileName_);
    QString strDrawPageName = szCopyGraphPageFileName_ + QString("-%1").arg(iLast);
    if ( DrawListUtils::drawList_.contains(strDrawPageName )) {
        szCopyGraphPageFileName_ = strDrawPageName;
        goto reGetNum;
    }

    ui->listWidgetGraphPages->addItem(strDrawPageName);
    DrawListUtils::drawList_.append(strDrawPageName);
    DrawListUtils::saveDrawList(szProjPath_);
    QString szFileName = szProjPath_ + "/" + szCopyGraphPageFileName_ + ".drw";
    QFile file(szFileName);
    QString szPasteFileName = szProjPath_ + "/" + strDrawPageName + ".drw";
    file.copy(szPasteFileName);

    if (szPasteFileName.toLower().endsWith(".drw")) {
        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
        if (!createDocument(graphPage, view, szPasteFileName)) {
            return;
        }

        currentGraphPage_ = graphPage;
        currentView_ = dynamic_cast<QGraphicsView *>(view);
        graphPage->setProjectPath(szProjPath_);
        graphPage->setProjectName(szProjName_);
        graphPage->loadAsXML(szPasteFileName);
        view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
        graphPage->setFileName(strDrawPageName + ".drw");
        graphPage->setGraphPageId(strDrawPageName);
    }

    QList<QListWidgetItem*> listWidgetItem = ui->listWidgetGraphPages->findItems(strDrawPageName, Qt::MatchCaseSensitive);
    if ( listWidgetItem.size() > 0 ) {
        ui->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
        graphPageTabWidget_->setCurrentIndex(ui->listWidgetGraphPages->currentRow());
    }

    currentGraphPage_->setUnsavedFlag(true);
    slotUpdateActions();
}
