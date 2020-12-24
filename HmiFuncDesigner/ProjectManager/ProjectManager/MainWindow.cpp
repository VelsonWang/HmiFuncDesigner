#include "MainWindow.h"
#include <QApplication>
#include <QCloseEvent>
#include <QGuiApplication>
#include <QScreen>
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
#include <QJsonObject>
#include <QJsonValue>
#include "AboutDialog.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "NewVariableGroupDialog.h"
#include "ProjectDownloadDialog.h"
#include "ProjectUploadDialog.h"
#include "RealTimeDatabaseChild.h"
#include "ScriptManageChild.h"
#include "TagManagerChild.h"
#include "MainWindow.h"
#include "Helper.h"
#include "projectinfomanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "ChildInterface.h"
#include "TagManagerChild.h"
#include "qsoftcore.h"
#include "VerifyPasswordDialog.h"
#include "../../libs/core/manhattanstyle.h"
#include "../../libs/core/newprojectdialog.h"
#include "../../libs/shared/pluginloader.h"
#include "../../libs/core/qabstractpage.h"
#include "../../libs/core/qsoftcore.h"
#include "../../libs/shared/qprojectcore.h"
#include "../../libs/shared/host/qabstracthost.h"
#include "../../libs/running/qrunningmanager.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFileInfo>
#include <QRect>
#include <QGraphicsView>
#include <QFileDialog>
#include <QScrollArea>
#include <QToolBar>
#include <QInputDialog>
#include <QCryptographicHash>
#include <QStackedWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_szCurItem(""),
      m_szCurTreeViewItem("")
{
    m_pCentralWidgetObj = new QStackedWidget(this);

    QMapIterator<QString, QAbstractPlugin*> it(PluginLoader::getPluginByType(PAGE_PLUGIN_NAME));
    while(it.hasNext()) {
        it.next();
        QAbstractPage* pPageObj = dynamic_cast<QAbstractPage*>(it.value());
        if(pPageObj) {
            QWidget *pWidgetObj = dynamic_cast<QWidget *>(pPageObj->getWidget());
            if(pWidgetObj) {
                m_pCentralWidgetObj->addWidget(pWidgetObj);
            }
            //qDebug() << "page name: "<< pPageObj->getPageName();
            m_mapNameToPage.insert(pPageObj->getPageName().toUpper(), pPageObj);
        }
    }

    initUI();
}


/**
 * @brief MainWindow::initUI
 * @details 初始化UI
 */
void MainWindow::initUI()
{
    m_pMdiAreaObj = new MdiArea(m_pCentralWidgetObj);
    QSizePolicy sizePolicyMdiArea(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicyMdiArea.setHorizontalStretch(0);
    sizePolicyMdiArea.setVerticalStretch(0);
    m_pMdiAreaObj->setSizePolicy(sizePolicyMdiArea);
    //m_pMdiAreaObj->setLineWidth(3);
    //m_pMdiAreaObj->setFrameShape(QFrame::Panel);
    //m_pMdiAreaObj->setFrameShadow(QFrame::Sunken);
    //m_pMdiAreaObj->setViewMode(QMdiArea::TabbedView);
    ((MdiArea*)m_pMdiAreaObj)->setupMdiArea();
    QObject::connect(m_pMdiAreaObj, SIGNAL(tabCloseRequested(int)), this, SLOT(onSlotTabCloseRequested(int)));
    qApp->installEventFilter(this);
    m_pMdiAreaObj->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pMdiAreaObj->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(m_pMdiAreaObj, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onSlotUpdateMenus()));
    m_windowMapper = new QSignalMapper(this);
    connect(m_windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(onSlotSetActiveSubWindow(QWidget*)));
    m_pCentralWidgetObj->addWidget(m_pMdiAreaObj);

    this->setCentralWidget(m_pCentralWidgetObj);

    // 工程管理器停靠控件
    m_pDockProjectMgrObj = new QDockWidget(this);
    m_pDockProjectMgrObj->setWindowTitle(tr("工程管理器"));
    this->addDockWidget(Qt::LeftDockWidgetArea, m_pDockProjectMgrObj);
    QWidget *dockWidgetContents = new QWidget();
    QVBoxLayout *dockWidgetContentsLayout = new QVBoxLayout(dockWidgetContents);
    dockWidgetContentsLayout->setSpacing(0);
    dockWidgetContentsLayout->setContentsMargins(0, 0, 0, 0);
    m_pTabProjectMgrObj = new QTabWidget(dockWidgetContents);

    QWidget *projectTabWidget = new QWidget();
    QVBoxLayout *projectTabWidgetLayout = new QVBoxLayout(projectTabWidget);
    projectTabWidgetLayout->setSpacing(0);
    projectTabWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_pProjectTreeViewObj = new ProjectTreeView(projectTabWidget);
    m_pProjectTreeViewObj->updateUI();
    connect(m_pProjectTreeViewObj, &ProjectTreeView::sigNotifyClicked, this, &MainWindow::onSlotTreeProjectViewClicked);
    connect(m_pProjectTreeViewObj, &ProjectTreeView::sigNotifySetWindowSetTitle, this, &MainWindow::onSlotSetWindowSetTitle);
    projectTabWidgetLayout->addWidget(m_pProjectTreeViewObj);

    m_pTabProjectMgrObj->addTab(projectTabWidget, QString(tr("工程")));
    QWidget *pWidgetCtrlObj = new QWidget();
    QVBoxLayout *pWidgetsLayoutObj = new QVBoxLayout(pWidgetCtrlObj);
    pWidgetsLayoutObj->setSpacing(0);
    pWidgetsLayoutObj->setContentsMargins(0, 0, 0, 0);
    pWidgetCtrlObj->setLayout(pWidgetsLayoutObj);

    // 图形元素控件
    QAbstractPage* pPageObj = m_mapNameToPage.value(QString("Designer").toUpper());
    if(pPageObj) {
        m_pDesignerWidgetObj = dynamic_cast<QWidget *>(pPageObj->getWidget());
        if(m_pDesignerWidgetObj) {
            QVariant variant = m_pDesignerWidgetObj->property("DesignerWidget");
            QWidget *pWidgetObj = variant.value<QWidget *>();
            pWidgetsLayoutObj->addWidget(pWidgetObj);
        }
    }
    m_pTabProjectMgrObj->addTab(pWidgetCtrlObj, QString(tr("画面")));
    dockWidgetContentsLayout->addWidget(m_pTabProjectMgrObj);
    m_pTabProjectMgrObj->setCurrentIndex(0);
    m_pDockProjectMgrObj->setWidget(dockWidgetContents);

    m_pUndoGroupObj = new QUndoGroup(this);

    // 创建状态栏
    createStatusBar();
    // 创建动作
    createActions();
    // 创建菜单
    createMenus();
    // 创建工具条
    createToolbars();

    m_bGraphPageGridVisible = true;
    m_iCurrentGraphPageIndex = 0;

    setContextMenuPolicy(Qt::DefaultContextMenu); // 右键菜单生效
    readSettings();  // 初始窗口时读取窗口设置信息
    loadRecentProjectList();

    //--------------------------------------------------------------------------

    slotUpdateActions();
    //connect(m_pGraphPageEditorObj, SIGNAL(currentChanged(int)), SLOT(slotChangeGraphPage(int)));

    QRect rect = QGuiApplication::primaryScreen()->geometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    this->resize(screenWidth*3/4, screenHeight*3/4);

    Helper::WidgetMoveCenter(this);
    //    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("HmiFuncDesigner组态软件"));

    // 当多文档区域的内容超出可视区域后，出现滚动条
    this->m_pMdiAreaObj->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->m_pMdiAreaObj->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //    this->m_pMdiAreaObj->setLineWidth(3);
    //    this->m_pMdiAreaObj->setFrameShape(QFrame::Panel);
    //    this->m_pMdiAreaObj->setFrameShadow(QFrame::Sunken);
    //    this->m_pMdiAreaObj->setViewMode(QMdiArea::TabbedView);

    this->m_pStatusBarObj->showMessage(tr("欢迎使用HmiFuncDesigner组态软件"));

    connect(m_pTabProjectMgrObj, SIGNAL(currentChanged(int)), SLOT(onSlotTabProjectMgrCurChanged(int)));
    onSlotTabProjectMgrCurChanged(0);
}


MainWindow::~MainWindow() {

}


/**
 * @brief MainWindow::createStatusBar
 * @details 创建状态栏
 */
void MainWindow::createStatusBar()
{
    m_pStatusBarObj = new QStatusBar(this);
    this->setStatusBar(m_pStatusBarObj);
}


/**
 * @brief MainWindow::createActions
 * @details 创建动作
 */
void MainWindow::createActions()
{
    QAction *pActObj = Q_NULLPTR;

    // 新建工程
    pActObj = new QAction(QIcon(":/images/newproject.png"), tr("新建"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+N"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onNewPoject);
        QSoftCore::getCore()->insertAction("Project.New", pActObj);
    }

    //  打开工程
    pActObj = new QAction(QIcon(":/images/openproject.png"), tr("打开"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+O"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onOpenProject);
        QSoftCore::getCore()->insertAction("Project.Open", pActObj);
    }

    // 关闭工程
    pActObj = new QAction(QIcon(":/images/projectexit.png"), tr("关闭"), Q_NULLPTR);
    if(pActObj) {
        connect(pActObj, &QAction::triggered, this, &MainWindow::onCloseProject);
        QSoftCore::getCore()->insertAction("Project.Close", pActObj);
    }

    // 保存工程
    pActObj = new QAction(QIcon(":/images/saveproject.png"), tr("保存"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+S"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onSaveProject);
        QSoftCore::getCore()->insertAction("Project.Save", pActObj);
    }

    // 设置打开工程的密码
    pActObj = new QAction(tr("设置打开工程的密码"), Q_NULLPTR);
    if(pActObj) {
        connect(pActObj, &QAction::triggered, this, &MainWindow::onSetOpenProjPassword);
        QSoftCore::getCore()->insertAction("Project.OpenPassword", pActObj);
        pActObj->setEnabled(false);
    }

    // 最近打开的工程
    pActObj = new QAction(tr("最近打开的工程"), Q_NULLPTR);
    if(pActObj) {
        QSoftCore::getCore()->insertAction("Project.LastOpen", pActObj);
        QJsonObject datJson;
        datJson.insert("which", QJsonValue("LastOpen"));
        QVariant datUser = QVariant(datJson);
        pActObj->setData(datUser);
    }

    // 退出
    pActObj = new QAction(QIcon(":/images/programexit.png"), tr("退出"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+Q"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onExit);
        QSoftCore::getCore()->insertAction("Project.Exit", pActObj);
    }

    //-----------------------------<视图>---------------------------------------

    // 视图工具栏
    pActObj = new QAction(tr("视图"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.Toolbar", pActObj);
    }

    // 状态栏
    pActObj = new QAction(tr("状态栏"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.StatusBar", pActObj);
    }

    // 工作区
    pActObj = new QAction(tr("工作区"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.WorkSpace", pActObj);
    }

    // 显示区
    pActObj = new QAction(tr("显示区"), Q_NULLPTR);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.DisplayArea", pActObj);
    }

    //-----------------------------<画面编辑器>----------------------------------

    // 关闭画面
    pActObj = new QAction(tr("关闭"), Q_NULLPTR);
    if(pActObj) {
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotCloseGraphPage()));
        QSoftCore::getCore()->insertAction("GraphPage.Close", pActObj);
    }

    // 关闭所有画面
    pActObj = new QAction(tr("关闭所有"), Q_NULLPTR);
    if(pActObj) {
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotCloseAll()));
        QSoftCore::getCore()->insertAction("GraphPage.CloseAll", pActObj);
    }

    // 撤销
    pActObj = m_pUndoGroupObj->createUndoAction(Q_NULLPTR);
    if(pActObj) {
        pActObj->setIcon(QIcon(":/DrawAppImages/undo.png"));
        pActObj->setText(tr("撤销"));
        pActObj->setShortcut(QKeySequence::Undo);
        QSoftCore::getCore()->insertAction("GraphPage.Undo", pActObj);
    }

    // 重做
    pActObj = m_pUndoGroupObj->createRedoAction(Q_NULLPTR);
    if(pActObj) {
        pActObj->setText(tr("重做"));
        pActObj->setIcon(QIcon(":/DrawAppImages/redo.png"));
        pActObj->setShortcut(QKeySequence::Redo);
        QSoftCore::getCore()->insertAction("GraphPage.Redo", pActObj);
    }

    // 删除画面
    pActObj = new QAction(QIcon(":/DrawAppImages/delete.png"), tr("删除"));
    if(pActObj) {
        pActObj->setShortcut(QKeySequence::Delete);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotEditDelete()));
        QSoftCore::getCore()->insertAction("GraphPage.Delete", pActObj);
    }

    // 拷贝画面
    pActObj = new QAction(QIcon(":/DrawAppImages/editcopy.png"), tr("拷贝"));
    if(pActObj) {
        pActObj->setShortcut(QKeySequence::Copy);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotEditCopy()));
        QSoftCore::getCore()->insertAction("GraphPage.Copy", pActObj);
    }

    // 粘贴画面
    pActObj = new QAction(QIcon(":/DrawAppImages/editpaste.png"), tr("粘贴"));
    if(pActObj) {
        pActObj->setShortcut(QKeySequence::Paste);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotEditPaste()));
        QSoftCore::getCore()->insertAction("GraphPage.Paste", pActObj);
    }

    //-----------------------------<工具菜单>----------------------------------
    // 模拟仿真
    pActObj = new QAction(QIcon(":/images/offline.png"), tr("模拟仿真"));
    if(pActObj) {
        pActObj->setEnabled(false);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotSimulate()));
        QSoftCore::getCore()->insertAction("Tools.Simulate", pActObj);
    }

    // 运行工程
    pActObj = new QAction(QIcon(":/images/online.png"), tr("运行"));
    if(pActObj) {
        pActObj->setEnabled(true);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotRunProject()));
        QSoftCore::getCore()->insertAction("Tools.Run", pActObj);
    }

    // 下载工程
    pActObj = new QAction(QIcon(":/images/download.png"), tr("下载"));
    if(pActObj) {
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotDownloadProject()));
        QSoftCore::getCore()->insertAction("Tools.Download", pActObj);
    }

    // 上载工程
    pActObj = new QAction(QIcon(":/images/upload.png"), tr("上载"));
    if(pActObj) {
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotUpLoadProject()));
        QSoftCore::getCore()->insertAction("Tools.UpLoad", pActObj);
    }


    //-----------------------------<窗口菜单>----------------------------------

    // 关闭活动窗口
    pActObj = new QAction(tr("关闭"));
    if(pActObj) {
        pActObj->setStatusTip(tr("关闭活动窗口"));
        connect(pActObj, &QAction::triggered, m_pMdiAreaObj, &QMdiArea::closeActiveSubWindow);
        QSoftCore::getCore()->insertAction("Window.Close", pActObj);
    }

    // 关闭所有窗口
    pActObj = new QAction(tr("关闭所有"));
    if(pActObj) {
        pActObj->setStatusTip(tr("关闭所有窗口"));
        connect(pActObj, &QAction::triggered, m_pMdiAreaObj, &QMdiArea::closeAllSubWindows);
        QSoftCore::getCore()->insertAction("Window.CloseAll", pActObj);
    }

    // 平铺所有窗口
    pActObj = new QAction(tr("平铺"));
    if(pActObj) {
        pActObj->setStatusTip(tr("平铺所有窗口"));
        connect(pActObj, &QAction::triggered, m_pMdiAreaObj, &QMdiArea::tileSubWindows);
        QSoftCore::getCore()->insertAction("Window.Tile", pActObj);
    }

    // 层叠所有窗口
    pActObj = new QAction(tr("层叠"));
    if(pActObj) {
        pActObj->setStatusTip(tr("层叠所有窗口"));
        connect(pActObj, &QAction::triggered, m_pMdiAreaObj, &QMdiArea::cascadeSubWindows);
        QSoftCore::getCore()->insertAction("Window.Cascade", pActObj);
    }

    // 下一窗口
    pActObj = new QAction(tr("下一窗口"));
    if(pActObj) {
        pActObj->setShortcuts(QKeySequence::NextChild);
        pActObj->setStatusTip(tr("下一窗口"));
        connect(pActObj, &QAction::triggered, m_pMdiAreaObj, &QMdiArea::activateNextSubWindow);
        QSoftCore::getCore()->insertAction("Window.Next", pActObj);
    }

    // 前一窗口
    pActObj = new QAction(tr("前一窗口"));
    if(pActObj) {
        pActObj->setShortcuts(QKeySequence::PreviousChild);
        pActObj->setStatusTip(tr("前一窗口"));
        connect(pActObj, &QAction::triggered, m_pMdiAreaObj, &QMdiArea::activatePreviousSubWindow);
        QSoftCore::getCore()->insertAction("Window.Previous", pActObj);
    }

    pActObj = new QAction();
    if(pActObj) {
        pActObj->setSeparator(true);
        QSoftCore::getCore()->insertAction("Window.Separator1", pActObj);
    }

    //-----------------------------<帮助菜单>----------------------------------
    // 帮助
    pActObj = new QAction(tr("帮助"));
    if(pActObj) {
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotHelp()));
        QSoftCore::getCore()->insertAction("Help.Help", pActObj);
    }

    // 关于
    pActObj = new QAction(tr("关于"));
    if(pActObj) {
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotAbout()));
        QSoftCore::getCore()->insertAction("Help.About", pActObj);
    }

}


/**
 * @brief MainWindow::createMenus
 * @details 创建菜单
 */
void MainWindow::createMenus()
{
    // 工程菜单
    m_pMenuProjectObj = this->menuBar()->addMenu(tr("工程"));
    m_pMenuProjectObj->addAction(QSoftCore::getCore()->getAction("Project.New"));
    m_pMenuProjectObj->addAction(QSoftCore::getCore()->getAction("Project.Open"));
    m_pMenuProjectObj->addAction(QSoftCore::getCore()->getAction("Project.Close"));
    m_pMenuProjectObj->addAction(QSoftCore::getCore()->getAction("Project.Save"));
    m_pMenuProjectObj->addAction(QSoftCore::getCore()->getAction("Project.OpenPassword"));
    m_pMenuProjectObj->addSeparator();
    m_pMenuProjectObj->addAction(QSoftCore::getCore()->getAction("Project.LastOpen"));
    m_pMenuProjectObj->addSeparator();
    m_pMenuProjectObj->addAction(QSoftCore::getCore()->getAction("Project.Exit"));

    // 视图菜单
    m_pMenuViewObj = this->menuBar()->addMenu(tr("视图"));
    m_pMenuViewObj->addAction(QSoftCore::getCore()->getAction("Widget.Toolbar"));
    m_pMenuViewObj->addAction(QSoftCore::getCore()->getAction("Widget.StatusBar"));
    m_pMenuViewObj->addAction(QSoftCore::getCore()->getAction("Widget.WorkSpace"));
    m_pMenuViewObj->addAction(QSoftCore::getCore()->getAction("Widget.DisplayArea"));

    //-----------------------------<画面编辑器>----------------------------------
    // 画面
    QMenu *filemenu = this->menuBar()->addMenu(tr("画面"));
    filemenu->addAction(QSoftCore::getCore()->getAction("GraphPage.Close")); // 画面.关闭
    filemenu->addAction(QSoftCore::getCore()->getAction("GraphPage.CloseAll")); // 画面.关闭所有

    //-----------------------------<工具菜单>----------------------------------
    m_pMenuToolsObj = this->menuBar()->addMenu(tr("工具"));
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.Simulate")); // 模拟仿真
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.Run")); // 运行工程
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.Download")); // 下载工程
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.UpLoad")); // 上传工程

    //-----------------------------<窗口菜单>----------------------------------
    m_pActWindowMenuObj = this->menuBar()->addMenu(tr("窗口"));
    connect(m_pActWindowMenuObj, &QMenu::aboutToShow, this, &MainWindow::onSlotUpdateWindowMenu);
    QAction *pActObj = new QAction();
    if(pActObj) {
        pActObj->setSeparator(true);
        QSoftCore::getCore()->insertAction("Window.Separator0", pActObj);
    }
    onSlotUpdateWindowMenu();
    menuBar()->addSeparator();

    //-----------------------------<帮助菜单>----------------------------------
    m_pMenuHelpObj = this->menuBar()->addMenu(tr("帮助"));
    m_pMenuHelpObj->addAction(QSoftCore::getCore()->getAction("Help.Help")); // 帮助
    m_pMenuHelpObj->addAction(QSoftCore::getCore()->getAction("Help.About")); // 关于
}


/**
    * @brief MainWindow::createToolbars
    * @details 创建工具条
    */
void MainWindow::createToolbars()
{
    QToolBar *pToolBarObj = new QToolBar();
    //-----------------------------<工程工具栏>-----------------------------------
    pToolBarObj = new QToolBar(this);
    QSoftCore::getCore()->insertToolBar("ToolBar.Project", pToolBarObj);
    pToolBarObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Project.New"));
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Project.Open"));
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Project.Close"));
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Project.Save"));
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Project.Exit"));

    //-----------------------------<画面编辑器>----------------------------------
    pToolBarObj = new QToolBar(this);
    QSoftCore::getCore()->insertToolBar("ToolBar.GraphPage", pToolBarObj);
    pToolBarObj->addSeparator();
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("GraphPage.Undo")); // 撤销
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("GraphPage.Redo")); // 重做
    pToolBarObj->addSeparator();
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("GraphPage.Copy")); // 拷贝画面
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("GraphPage.Paste")); // 粘贴画面
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("GraphPage.Delete")); // 删除画面
    pToolBarObj->addSeparator();

    //-----------------------------<工具>----------------------------------
    pToolBarObj = new QToolBar(this);
    QSoftCore::getCore()->insertToolBar("ToolBar.Tools", pToolBarObj);
    pToolBarObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.Simulate")); // 模拟仿真
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.Run")); // 运行工程
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.Download")); // 下载工程
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.UpLoad")); // 上传工程

    //--------------------------------------------------------------------------

    this->addToolBar(Qt::TopToolBarArea, QSoftCore::getCore()->getToolBar("ToolBar.Project"));
    this->addToolBar(Qt::TopToolBarArea, QSoftCore::getCore()->getToolBar("ToolBar.Tools"));
    //addToolBarBreak();
    this->addToolBar(Qt::TopToolBarArea, QSoftCore::getCore()->getToolBar("ToolBar.GraphPage"));
}

/**
 * @brief MainWindow::onSlotUpdateWindowMenu
 * @details 更新窗口菜单
 */
void MainWindow::onSlotUpdateWindowMenu()
{
    m_pActWindowMenuObj->clear();
    m_pActWindowMenuObj->addAction(QSoftCore::getCore()->getAction("Window.Close"));
    m_pActWindowMenuObj->addAction(QSoftCore::getCore()->getAction("Window.CloseAll"));
    m_pActWindowMenuObj->addSeparator();
    m_pActWindowMenuObj->addAction(QSoftCore::getCore()->getAction("Window.Tile"));
    m_pActWindowMenuObj->addAction(QSoftCore::getCore()->getAction("Window.Cascade"));
    m_pActWindowMenuObj->addSeparator();
    m_pActWindowMenuObj->addAction(QSoftCore::getCore()->getAction("Window.Next"));
    m_pActWindowMenuObj->addAction(QSoftCore::getCore()->getAction("Window.Previous"));
    m_pActWindowMenuObj->addAction(QSoftCore::getCore()->getAction("Window.Separator1"));

    QList<QMdiSubWindow*> windows = m_pMdiAreaObj->subWindowList();
    QAction *pActObj = QSoftCore::getCore()->getAction("Window.Separator1");
    if(pActObj) pActObj->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        QWidget* pWidgwtObj = windows.at(i)->widget();
        if(!pWidgwtObj) continue;

        ChildInterface* pIFaceChildObj = qobject_cast<ChildInterface*>(pWidgwtObj);
        if(!pIFaceChildObj) continue;

        QString szText = "";
        if (i < 9) {
            szText = tr("&%1 %2").arg(i + 1).arg(pIFaceChildObj->wndTitle());
        } else {
            szText = tr("%1 %2").arg(i + 1).arg(pIFaceChildObj->wndTitle());
        }

        QAction* pActObj  = m_pActWindowMenuObj->addAction(szText);
        pActObj->setCheckable(true);
        pActObj->setChecked(pWidgwtObj == activeMdiChild());
        connect(pActObj, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
        m_windowMapper->setMapping(pActObj, windows.at(i));
    }
}


QWidget *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = this->m_pMdiAreaObj->activeSubWindow()) {
        return qobject_cast<QWidget *>(activeSubWindow->widget());
    }
    return Q_NULLPTR;
}


void MainWindow::onSlotSetActiveSubWindow(QWidget *window)
{
    if (!window) return;

    QMdiSubWindow *pMdiSubWndObj = qobject_cast<QMdiSubWindow *>(window);
    if(pMdiSubWndObj) {
        QWidget *pWidgetObj = qobject_cast<QWidget *>(pMdiSubWndObj->widget());
        if(pWidgetObj) {
            m_szCurItem = window->windowTitle();
        }
    }
    this->m_pMdiAreaObj->setActiveSubWindow(pMdiSubWndObj);

    QList<QMdiSubWindow *> subWindowList = this->m_pMdiAreaObj->subWindowList(QMdiArea::ActivationHistoryOrder);
    int iSubWndSize = subWindowList.size();
    QWidget *pLastActiveMdiChildObj = Q_NULLPTR;
    if(iSubWndSize > 1) {
        pLastActiveMdiChildObj = subWindowList.at(iSubWndSize-2)->widget();
        if (ChildInterface* pIFaceChildOldObj = qobject_cast<ChildInterface*>(pLastActiveMdiChildObj)) {
            setUpdatesEnabled(false);
            pIFaceChildOldObj->removeUserInterface(this);
            setUpdatesEnabled(true);
        }
    }

    QWidget* pChildWndObj = Q_NULLPTR;
    pChildWndObj = subWindowList.at(iSubWndSize-1)->widget();;
    if (ChildInterface* pIFaceChildNowObj = qobject_cast<ChildInterface*>(pChildWndObj)) {
        m_childCurrent = pChildWndObj;
        setUpdatesEnabled(false);
        pIFaceChildNowObj->buildUserInterface(this);
        setUpdatesEnabled(true);
    }
}

QWidget *MainWindow::getActiveSubWindow()
{
    return this->m_pMdiAreaObj->activeSubWindow()->widget();
}


QMdiSubWindow *MainWindow::findMdiChild(const QString &szWndTitle)
{
    foreach (QMdiSubWindow *wnd, this->m_pMdiAreaObj->subWindowList()) {
        ChildInterface *ifChild = qobject_cast<ChildInterface *>(wnd->widget());
        if (ifChild && ifChild->wndTitle() == szWndTitle) return wnd;
    }
    return Q_NULLPTR;
}


/*
   * 新建工程时，创建缺省IO变量组
   */
void MainWindow::CreateDefaultIOTagGroup()
{
    if (this->m_pProjectTreeViewObj->getDevTagGroupCount() == 0) {
//        TagIOGroup &tagIOGroup = QSoftCore::getCore()->getProjectCore()->tagIOGroup_;
//        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
//        pObj->m_id = 1;
//        pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
//        pObj->m_szShowName = QString(tr("IO设备"));
//        tagIOGroup.listTagIOGroupDBItem_.append(pObj);
        UpdateDeviceVariableTableGroup();
    }
}


bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    switch(event->type()) {
    case QEvent::ChildRemoved :
        if (QChildEvent* pEvent= static_cast<QChildEvent*>(event)) {
            if (qobject_cast<QMdiSubWindow*>(pEvent->child()) && m_childCurrent &&
                    m_pMdiAreaObj->subWindowList().size() == 1) {
                if (ChildInterface* pIFaceChildObj = qobject_cast<ChildInterface*>(m_childCurrent))
                    pIFaceChildObj->removeUserInterface(this);
                m_childCurrent = Q_NULLPTR;
            }
        }
        break;
    default:
        break;
    }
    return QMainWindow::eventFilter(obj, event);
}


/**
    * @brief MainWindow::closeEvent  关闭事件
    * @param event
    */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QString strFile = Helper::AppDir() + "/lastpath.ini";
    if (this->m_pProjectTreeViewObj->getProjectName() != tr("未创建工程"))
        ConfigUtils::setCfgStr(strFile, "PathInfo", "Path", QSoftCore::getCore()->getProjectCore()->m_szProjPath);
    this->m_pMdiAreaObj->closeAllSubWindows();
    writeSettings();
    m_pMdiAreaObj->closeAllSubWindows();
    if (m_pMdiAreaObj->currentSubWindow()) {
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
 * @brief MainWindow::copySystemTags
 * @dettails 拷贝系统变量
 */
void MainWindow::copySystemTags()
{
    QString szTagFile = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
    QString szTags = "";
    QFile readFile(szTagFile);
    if (readFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&readFile);
        in.setCodec("utf-8");
        szTags = in.readAll();
        readFile.close();
        XMLObject xml;
        if(xml.load(szTags, Q_NULLPTR)) {
            QSoftCore::getCore()->getProjectCore()->tagMgr_.openFromXml(&xml);
        }
    }
}

/**
    * @brief MainWindow::onNewPoject
    * @details 新建工程
    */
void MainWindow::onNewPoject()
{
    if (this->m_pProjectTreeViewObj->getProjectName() != tr("未创建工程")) {
        QMessageBox::information(this,
                                 tr("系统提示"),
                                 tr("工程文件已建立，请手动关闭当前工程文件后重新建立！"));
        return;
    }

    NewProjectDialog *pNewProjectDlg = new NewProjectDialog(this);
    if (pNewProjectDlg->exec() == QDialog::Accepted) {
        UpdateProjectName(QSoftCore::getCore()->getProjectCore()->m_szProjFile);
        pNewProjectDlg->save();
        updateRecentProjectList(QSoftCore::getCore()->getProjectCore()->m_szProjFile);
        CreateDefaultIOTagGroup();
        UpdateDeviceVariableTableGroup();
        // 拷贝系统变量
        copySystemTags();

        QSoftCore::getCore()->getProjectCore()->createNewProj(QSoftCore::getCore()->getProjectCore()->m_szProjFile);
    }
}

void MainWindow::doOpenProject(QString proj)
{
    QFile fileProj(proj);
    if(!fileProj.exists()) {
        QMessageBox::information(this, tr("系统提示"), tr("工程：") + proj + tr("不存在！"));
        return;
    }

    ////////////////////////////////////////////////////////////////////////////

    QFileInfo fileInfoSrc(proj);
    if(fileInfoSrc.size() <= 512) {
        QMessageBox::information(this, tr("系统提示"), tr("读取工程数据出错，文件头数据有误！"));
        return;
    }

    // 读取文件头信息
    if(!fileProj.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("系统提示"), tr("打开工程：") + proj + tr("失败！"));
        return;
    }

    quint8 buf[1024] = {0};
    quint16 wSize = (sizeof(TFileHeader) < 512) ? 512 : sizeof(TFileHeader);

    if(fileProj.read((char *)buf, wSize) != wSize) {
        fileProj.close();
        QMessageBox::information(this, tr("系统提示"), tr("读取文件头数据出错！"));
        return;
    }
    fileProj.close();

    TFileHeader headerObj;
    memcpy((void *)&headerObj, (void *)buf, sizeof(TFileHeader));

    if(headerObj.byOpenVerifyPassword != 0) {
        QByteArray baPwd;
        baPwd.append((const char *)headerObj.szPassword, headerObj.byOpenVerifyPassword);
        VerifyPasswordDialog dlg;
        dlg.setTargetPassword(baPwd.toHex());
        if(dlg.exec() == QDialog::Rejected) {
            return;
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    QSoftCore::getCore()->getProjectCore()->m_szProjFile = proj;
    UpdateProjectName(proj);
    QString strFile = Helper::AppDir() + "/lastpath.ini";
    ConfigUtils::setCfgStr(strFile, "PathInfo", "Path", QSoftCore::getCore()->getProjectCore()->m_szProjPath);

    QSoftCore::getCore()->getProjectCore()->m_szProjPath = QSoftCore::getCore()->getProjectCore()->getProjectPath(proj);
    QSoftCore::getCore()->getProjectCore()->m_szProjName = QSoftCore::getCore()->getProjectCore()->getProjectNameWithOutSuffix(proj);
    QSoftCore::getCore()->getProjectCore()->openFromXml(QSoftCore::getCore()->getProjectCore()->m_szProjFile);

    // 加载设备变量组信息
    UpdateDeviceVariableTableGroup();
    updateRecentProjectList(proj);

    QAction *pActObj = QSoftCore::getCore()->getAction("Project.OpenPassword");
    if(pActObj) pActObj->setEnabled(true);
}

/**
    * @brief MainWindow::onOpenProject
    * @details 打开工程
    */
void MainWindow::onOpenProject()
{
    QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
    QString path = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "C:/");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("选择工程文件"),
                                                    path,
                                                    tr("project file (*.pdt)"));
    if(fileName != Q_NULLPTR) {
        doOpenProject(fileName);
    }
}

void MainWindow::on_actionWorkSpace_triggered(bool checked)
{
    this->m_pDockProjectMgrObj->setVisible(checked);
}


/**
    * @brief MainWindow::onSlotProjectTreeViewClicked
    * @details 工程树节点被单击
    * @param index
    */
void MainWindow::onSlotTreeProjectViewClicked(const QString &szItemText)
{
    if(QSoftCore::getCore()->getProjectCore()->m_szProjFile == "") return;

    QStringList szListUserData = szItemText.split("|");
    if(szListUserData.size() < 2) return;

    QAbstractPage* pPageObj = m_mapNameToPage.value(szListUserData.at(0).toUpper());
    if(pPageObj) {
        QWidget *pWidgetObj = dynamic_cast<QWidget *>(pPageObj->getWidget());
        if(pWidgetObj) {
            pWidgetObj->setProperty("UserData", szListUserData);
            if(m_pCentralWidgetObj->indexOf(pWidgetObj) >= 0) {
                m_pCentralWidgetObj->setCurrentWidget(pWidgetObj);
                return;
            }
        }
    }

    QMdiSubWindow *pWndObj = findMdiChild(szListUserData.at(1));
    if(pWndObj == Q_NULLPTR) {
        ChildInterface *pIFaceChildObj = Q_NULLPTR;
        if(szListUserData.at(0) == QString("TagMgr").toUpper()) { // 变量管理
            TagManagerChild *pObj = new TagManagerChild(this);
            pWndObj = this->m_pMdiAreaObj->addSubWindow(pObj);
            pObj->setWindowTitle(szListUserData.at(1));
            pObj->showMaximized();
            pIFaceChildObj = pObj;
            pIFaceChildObj->m_szProjectName = QSoftCore::getCore()->getProjectCore()->m_szProjFile;
            pIFaceChildObj->m_szItemName = szListUserData.at(0);
        } else if(szListUserData.at(0) == QString("RealTimeDatabase").toUpper()) { // 实时数据库
            RealTimeDatabaseChild *pObj = new RealTimeDatabaseChild(this);
            pWndObj = this->m_pMdiAreaObj->addSubWindow(pObj);
            pObj->setWindowTitle(szListUserData.at(1));
            pObj->showMaximized();
            pIFaceChildObj = pObj;
            pIFaceChildObj->m_szProjectName = QSoftCore::getCore()->getProjectCore()->m_szProjFile;
            pIFaceChildObj->m_szItemName = szListUserData.at(0);
        } else if(szListUserData.at(0) == QString("ScriptEditor").toUpper()) { // 脚本编辑器
            ScriptManageChild *pObj = new ScriptManageChild(this);
            pWndObj = this->m_pMdiAreaObj->addSubWindow(pObj);
            pObj->setWindowTitle(szListUserData.at(1));
            pObj->showMaximized();
            pIFaceChildObj = pObj;
            pIFaceChildObj->m_szProjectName = QSoftCore::getCore()->getProjectCore()->m_szProjFile;
            pIFaceChildObj->m_szItemName = szListUserData.at(0);
        }
    }

    onSlotSetActiveSubWindow(pWndObj);
}


/**
 * @brief MainWindow::UpdateProjectName
 * @details 更新工程名称
 * @param szName 工程名称
 */
void MainWindow::UpdateProjectName(const QString &szName)
{
    QAction *pActObj = Q_NULLPTR;
    pActObj = QSoftCore::getCore()->getAction("Tools.Run");
    if(!szName.isEmpty()) {
        QString szNameTmp = szName.mid(szName.lastIndexOf("/") + 1, szName.indexOf(".") - szName.lastIndexOf("/") - 1);
        this->m_pProjectTreeViewObj->setProjectName(szNameTmp);
        if(pActObj) pActObj->setEnabled(true);
    } else {
        QSoftCore::getCore()->getProjectCore()->m_szProjFile = "";
        QSoftCore::getCore()->getProjectCore()->m_szProjPath = "";
        if(pActObj) pActObj->setEnabled(false);
        this->m_pProjectTreeViewObj->updateUI();
    }
}

/*
    * 更新设备变量组
    */
void MainWindow::UpdateDeviceVariableTableGroup()
{
    this->m_pProjectTreeViewObj->updateDeviceTagGroup();
}


/**
    * @brief MainWindow::onSaveProject
    * @details 保存工程
    */
void MainWindow::onSaveProject()
{
    QSoftCore::getCore()->getProjectCore()->saveToXml(QSoftCore::getCore()->getProjectCore()->m_szProjFile);
    foreach (QMdiSubWindow* window, this->m_pMdiAreaObj->subWindowList()) {
        ChildInterface *pIFaceChildObj = qobject_cast<ChildInterface *>(window->widget());
        //if(pIFaceChildObj != Q_NULLPTR) pIFaceChildObj->save();
    }
}

///
/// \brief MainWindow::onSetOpenProjPassword
/// \details 设置打开工程的密码
///
void MainWindow::onSetOpenProjPassword()
{
    VerifyPasswordDialog dlg(false, this);
    if(dlg.exec() == QDialog::Accepted) {
        QString szPwd = dlg.getSetPassword();
        if(szPwd.isEmpty()) {
            QSoftCore::getCore()->getProjectCore()->headerObj_.byOpenVerifyPassword = 0;
            memset(QSoftCore::getCore()->getProjectCore()->headerObj_.szPassword, 0, 32);
        } else {
            memset(QSoftCore::getCore()->getProjectCore()->headerObj_.szPassword, 0, 32);
            QCryptographicHash crypt(QCryptographicHash::Md5);
            crypt.reset();
            crypt.addData(szPwd.toStdString().c_str(), szPwd.toStdString().length());
            QByteArray baPwd = crypt.result();
            QSoftCore::getCore()->getProjectCore()->headerObj_.byOpenVerifyPassword = baPwd.length();
            memcpy_s(QSoftCore::getCore()->getProjectCore()->headerObj_.szPassword, 32, baPwd.data(), baPwd.length());
        }
    }
}

/**
    * @brief MainWindow::onCloseProject
    * @details 关闭工程
    */
void MainWindow::onCloseProject()
{
    if(this->m_pProjectTreeViewObj->getProjectName() == tr("未创建工程"))
        return;
    foreach (QMdiSubWindow* window, this->m_pMdiAreaObj->subWindowList()) {
        ChildInterface *pIFaceChildObj = qobject_cast<ChildInterface *>(window->widget());
        //if(pIFaceChildObj != Q_NULLPTR) pIFaceChildObj->save();
        window->close();
    }
    UpdateProjectName(QString());
    // 关闭所有画面
    onSlotCloseAll();
    onSlotTabProjectMgrCurChanged(0);
    m_pTabProjectMgrObj->setCurrentIndex(0);

    QAction *pActObj = QSoftCore::getCore()->getAction("Project.OpenPassword");
    if(pActObj) pActObj->setEnabled(false);
}


/**
    * @brief MainWindow::onExit
    * @details 退出
    */
void MainWindow::onExit()
{
    onSaveProject();
    qApp->exit();
}


/**
    * @brief MainWindow::onSlotSimulate
    * @details 模拟仿真
    */
void MainWindow::onSlotSimulate()
{

}

/**
    * @brief MainWindow::onSlotRunProject
    * @details 运行工程
    */
void MainWindow::onSlotRunProject()
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
        argv << QSoftCore::getCore()->getProjectCore()->m_szProjPath;
        process->startDetached(fileRuntimeApplication, argv);
    }
}

/**
    * @brief MainWindow::onSlotUpLoadProject
    * @details 上载工程
    */
void MainWindow::onSlotUpLoadProject()
{
    // 创建tmp目录
    QString tmpDir = QCoreApplication::applicationDirPath() + "/UploadProjects/tmp";
    QDir dir(tmpDir);
    if (!dir.exists()) {
        dir.mkpath(tmpDir);
    }

    ProjectUploadDialog *pDlg = new ProjectUploadDialog(this, QSoftCore::getCore()->getProjectCore()->m_szProjFile);
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


/**
    * @brief MainWindow::onSlotDownloadProject
    * @details 下载工程
    */
void MainWindow::onSlotDownloadProject()
{
    if(QSoftCore::getCore()->getProjectCore()->m_szProjFile.isEmpty()) return;

    // 创建tmp目录
    QString tmpDir = QCoreApplication::applicationDirPath() + "/tmp";
    QDir dir(tmpDir);
    if(!dir.exists()) {
        dir.mkpath(tmpDir);
    }

    // 拷贝工程到tmp目录
    QString desDir = QCoreApplication::applicationDirPath() + "/tmp/RunProject";
    Helper::CopyRecursively(QSoftCore::getCore()->getProjectCore()->m_szProjPath, desDir);

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

    ProjectDownloadDialog *pDlg = new ProjectDownloadDialog(this, QSoftCore::getCore()->getProjectCore()->m_szProjFile);
    pDlg->setProjFileName(QCoreApplication::applicationDirPath() + "/tmp/RunProject.tar");
    if (pDlg->exec() == QDialog::Accepted) {
    }
    delete pDlg;
}


/**
    * @brief MainWindow::onSlotHelp
    * @details 帮助
    */
void MainWindow::onSlotHelp()
{

}


/**
    * @brief MainWindow::onSlotAbout
    * @details 关于
    */
void MainWindow::onSlotAbout()
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
            QAction *pAct = new QAction(slist.at(i));
            pAct->setStatusTip(tr(""));
            connect(pAct, &QAction::triggered, this, [=]{
                this->doOpenProject(pAct->text());
            });
            this->m_pMenuProjectObj->insertAction(QSoftCore::getCore()->getAction("Project.LastOpen"), pAct);
        }
        this->m_pMenuProjectObj->removeAction(QSoftCore::getCore()->getAction("Project.LastOpen"));
        return;
    }

    QList<QAction *> listActRemove;
    QList<QAction *> listAct = this->m_pMenuProjectObj->actions();
    for (int i = 0; i<listAct.size(); ++i) {
        QAction *pActObj = listAct.at(i);
        if(pActObj->isSeparator()) listActRemove.append(pActObj);
        QVariant datUser = pActObj->data();
        if(datUser.isValid()) {
            QJsonObject datJson = datUser.toJsonObject();
            if(datJson["which"].toString() == "LastOpen")
                listActRemove.append(pActObj);
        }
    }
    for (int j = 0; j<listActRemove.size(); ++j) {
        this->m_pMenuProjectObj->removeAction(listActRemove.at(j));
    }
}

/**
 * @brief MainWindow::updateRecentProjectList
 * @details 更新最近打开的工程列表
 * @param newProj
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
        QList<QAction *> listAct = this->m_pMenuProjectObj->actions();
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
            this->m_pMenuProjectObj->removeAction(listActRemove.at(j));
        }

        /////////////////////////////////////////////////////

        bStart = bEnd = false;
        QAction *pActPos = Q_NULLPTR;
        listAct.clear();
        listAct = this->m_pMenuProjectObj->actions();
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
            this->m_pMenuProjectObj->insertAction(pActPos, pAct);
        }
    } else {
        QStringList slist;
        ConfigUtils::writeCfgList(iniRecentProjectFileName, "RecentProjects", "project", slist);
    }
}


//------------------------------------------------------------------------------


/**
 * @brief MainWindow::getAllElementIDName
 * @details 获取工程所有控件的ID名称
 * @param szIDList 所有控件的ID名称
 */
void MainWindow::getAllElementIDName(QStringList &szIDList) {
//    GraphPageManager::getInstance()->getAllElementIDName(szIDList);
}

/**
 * @brief MainWindow::getAllElementIDName
 * @details 获取工程所有画面名称
 * @param szList 所有画面名称
 */
void MainWindow::getAllGraphPageName(QStringList &szList) {
//    GraphPageManager::getInstance()->getAllGraphPageName(szList);
}


void MainWindow::slotUpdateActions()
{
//    QAction *pActObj = Q_NULLPTR;
//    static const QIcon unsaved(":/DrawAppImages/filesave.png");

//    for (int i = 0; i < m_pGraphPageEditorObj->count(); i++) {
//        QGraphicsView *view = dynamic_cast<QGraphicsView*>(m_pGraphPageEditorObj->widget(i));
//        if (!dynamic_cast<GraphPage *>(view->scene())->undoStack()->isClean() ||
//                dynamic_cast<GraphPage *>(view->scene())->getUnsavedFlag()) {
//            m_pGraphPageEditorObj->setTabIcon(m_pGraphPageEditorObj->indexOf(view), unsaved);
//        } else {
//            m_pGraphPageEditorObj->setTabIcon(m_pGraphPageEditorObj->indexOf(view), QIcon());
//        }
//    }

//    if (!m_pCurrentGraphPageObj) return;

//    m_pUndoGroupObj->setActiveStack(m_pCurrentGraphPageObj->undoStack());

//    pActObj = QSoftCore::getCore()->getAction("Project.Save");
//    if (pActObj && (!m_pCurrentGraphPageObj->undoStack()->isClean() || m_pCurrentGraphPageObj->getUnsavedFlag())) {
//        pActObj->setEnabled(true);
//    } else {
//        pActObj->setEnabled(false);
//    }
}

void MainWindow::slotChangeGraphPage(int iGraphPageNum)
{
//    if (iGraphPageNum == -1) return;

//    if(iGraphPageNum != this->m_pListWidgetGraphPagesObj->currentRow())
//        this->m_pListWidgetGraphPagesObj->setCurrentRow(iGraphPageNum);

//    for (int i = 0; i < m_pGraphPageEditorObj->count(); i++) {
//        QGraphicsView *view = dynamic_cast<QGraphicsView *>(m_pGraphPageEditorObj->widget(i));
//        dynamic_cast<GraphPage *>(view->scene())->setActive(false);
//    }

//    m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(m_pGraphPageEditorObj->widget(iGraphPageNum));
//    m_pCurrentGraphPageObj = dynamic_cast<GraphPage *>(m_pCurrentViewObj->scene());
//    m_pCurrentGraphPageObj->setActive(true);
//    //currentGraphPage_->fillGraphPagePropertyModel();
//    m_iCurrentGraphPageIndex = iGraphPageNum;
//    slotUpdateActions();
}

void MainWindow::slotChangeGraphPageName()
{
//    m_pGraphPageEditorObj->setTabText(m_iCurrentGraphPageIndex, m_pCurrentGraphPageObj->getGraphPageId());
//    //int index = GraphPageManager::getInstance()->getIndexByGraphPage(currentGraphPage_);
}


void MainWindow::slotElementIdChanged()
{

}

void MainWindow::slotElementPropertyChanged()
{

}

void MainWindow::slotGraphPagePropertyChanged()
{

}


/**
    * @brief MainWindow::onSlotCloseAll
    * @details 关闭所有画面
    */
void MainWindow::onSlotCloseAll()
{
//    while(m_pGraphPageEditorObj->count()) {
//        QGraphicsView *view = static_cast<QGraphicsView*>(m_pGraphPageEditorObj->widget(m_pGraphPageEditorObj->currentIndex()));
//        removeGraphPage(view);
//        delete view;
//    }

//    m_pCurrentViewObj = Q_NULLPTR;
//    m_pCurrentGraphPageObj = Q_NULLPTR;
//    slotUpdateActions();
}


/**
    * @brief MainWindow::onSlotCloseGraphPage
    * @details 关闭画面
    */
void MainWindow::onSlotCloseGraphPage()
{
//    QGraphicsView *view = m_pCurrentViewObj;
//    removeGraphPage(view);
//    delete view;

//    if (m_pGraphPageEditorObj->count() == 0) {
//        m_pCurrentGraphPageObj = Q_NULLPTR;
//        m_pCurrentViewObj = Q_NULLPTR;
//    }

//    slotUpdateActions();
}


void MainWindow::updateGraphPageViewInfo(const QString &fileName)
{
//    int index = m_pGraphPageEditorObj->indexOf(m_pCurrentViewObj);
//    QFileInfo file(fileName);
//    m_pCurrentGraphPageObj->setGraphPageId(file.baseName());
//    m_pGraphPageEditorObj->setTabText(index,file.baseName());
//    slotChangeGraphPageName();
}


int MainWindow::exitResponse()
{
    int ret = QMessageBox::information(this,
                                       tr("退出程序"),
                                       tr("文件已修改。是否保存?"),
                                       QMessageBox::Yes | QMessageBox::No);
    return ret;
}

/**
    * @brief MainWindow::slotEditDelete
    * @details 删除画面
    */
void MainWindow::onSlotEditDelete()
{
//    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
//        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
//        m_pCurrentGraphPageObj->onEditDelete(items);
//    }
}


/**
    * @brief MainWindow::slotEditCopy
    * @details 拷贝画面
    */
void MainWindow::onSlotEditCopy()
{
//    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
//        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
//        m_pCurrentGraphPageObj->onEditCopy(items);
//    }
}


/**
    * @brief MainWindow::slotEditPaste
    * @details 粘贴画面
    */
void MainWindow::onSlotEditPaste()
{
//    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
//        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
//        m_pCurrentGraphPageObj->onEditPaste();
//    }
}


/**
    * @brief MainWindow::onRenameGraphPage
    * @details 修改画面名称
    */
void MainWindow::onRenameGraphPage()
{
//    QString szOldGraphPageName = this->m_pListWidgetGraphPagesObj->currentItem()->text();

//    QInputDialog dlg(this);
//    dlg.setWindowTitle(tr("修改画面名称"));
//    dlg.setLabelText(tr("请输入画面名称"));
//    dlg.setOkButtonText(tr("确定"));
//    dlg.setCancelButtonText(tr("取消"));
//    dlg.setTextValue(szOldGraphPageName);

//reInput:
//    if (dlg.exec() == QDialog::Accepted) {
//        QString szNewGraphPageName = dlg.textValue();

//        if (szNewGraphPageName == "") {
//            goto reInput;
//        }

//        QStringList szGraphPageNameList;
//        QSoftCore::getCore()->getProjectCore()->getAllGraphPageName(szGraphPageNameList);
//        for (int i = 0; i < szGraphPageNameList.count(); i++) {
//            if (szOldGraphPageName == szGraphPageNameList.at(i) ) {
//                szGraphPageNameList.replace(i, szNewGraphPageName);
//                QString szOldName = QSoftCore::getCore()->getProjectCore()->szProjPath_ + "/" + szOldGraphPageName + ".drw";
//                QString szNewName = QSoftCore::getCore()->getProjectCore()->szProjPath_ + "/" + szNewGraphPageName + ".drw";
//                QFile::rename(szOldName, szNewName);
//                //DrawListUtils::saveDrawList(QSoftCore::getCore()->getProjectCore()->szProjPath_);
//                this->m_pListWidgetGraphPagesObj->currentItem()->setText(szNewGraphPageName);
//                GraphPage *pGraphPage = GraphPageManager::getInstance()->getGraphPageById(szOldGraphPageName);
//                pGraphPage->setGraphPageId(szNewGraphPageName);
//                m_pGraphPageEditorObj->setTabText(m_pGraphPageEditorObj->currentIndex(), szNewGraphPageName);
//                m_pCurrentGraphPageObj->setUnsavedFlag(true);
//                slotUpdateActions();
//                break;
//            }
//        }
//    }
}


/**
    * @brief MainWindow::onDeleteGraphPage
    * @details 删除画面
    */
void MainWindow::onDeleteGraphPage()
{

#if 0
    for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
        if ( szGraphPageName == DrawListUtils::drawList_.at(i) ) {
            DrawListUtils::drawList_.removeAt(i);

            QString fileName = QSoftCore::getCore()->getProjectCore()->szProjPath_ + "/" + szGraphPageName + ".drw";
            QFile file(fileName);
            if (file.exists()) {
                file.remove();
            }

            m_pGraphPageEditorObj->removeTab(this->m_pListWidgetGraphPagesObj->currentRow());

            GraphPage *pGraphPageObj = GraphPageManager::getInstance()->getGraphPageById(szGraphPageName);
            if ( pGraphPageObj != Q_NULLPTR ) {
                GraphPageManager::getInstance()->removeGraphPage(pGraphPageObj);
                delete pGraphPageObj;
                pGraphPageObj = Q_NULLPTR;
            }

            //DrawListUtils::saveDrawList(QSoftCore::getCore()->getProjectCore()->szProjPath_);

            this->m_pListWidgetGraphPagesObj->clear();
            //foreach(QString szPageId, DrawListUtils::drawList_) {
                this->m_pListWidgetGraphPagesObj->addItem(szPageId);
            //}

            if (this->m_pListWidgetGraphPagesObj->count() > 0) {
                this->m_pListWidgetGraphPagesObj->setCurrentRow(0);
                m_pGraphPageEditorObj->setCurrentIndex(0);
            }

            break;
        }
    }
#endif
}


/**
    * @brief MainWindow::onCopyGraphPage
    * @details 复制画面
    */
void MainWindow::onCopyGraphPage()
{

}


/**
    * @brief MainWindow::onPasteGraphPage
    * @details 粘贴画面
    */
void MainWindow::onPasteGraphPage()
{
//    int iLast = 0;

//reGetNum:
//    //iLast = DrawListUtils::getMaxDrawPageNum(m_szCopyGraphPageFileName);
//    QString strDrawPageName = m_szCopyGraphPageFileName + QString("-%1").arg(iLast);
//    //if ( DrawListUtils::drawList_.contains(strDrawPageName )) {
//        m_szCopyGraphPageFileName = strDrawPageName;
//        goto reGetNum;
//    //}

//    this->m_pListWidgetGraphPagesObj->addItem(strDrawPageName);
//    //DrawListUtils::drawList_.append(strDrawPageName);
//    //DrawListUtils::saveDrawList(QSoftCore::getCore()->getProjectCore()->szProjPath_);
//    QString szFileName = QSoftCore::getCore()->getProjectCore()->szProjPath_ + "/" + m_szCopyGraphPageFileName + ".drw";
//    QFile file(szFileName);
//    QString szPasteFileName = QSoftCore::getCore()->getProjectCore()->szProjPath_ + "/" + strDrawPageName + ".drw";
//    file.copy(szPasteFileName);

//    if (szPasteFileName.toLower().endsWith(".drw")) {
//        QGraphicsView *view = createTabView();

//        if (m_pGraphPageEditorObj->indexOf(view) != -1) {
//            delete view;
//            return;
//        }

//        GraphPage *graphPage = new GraphPage(QRectF(), m_pVariantPropertyMgrObj, m_pPropertyEditorObj);
//        if (!createDocument(graphPage, view)) {
//            return;
//        }

//        m_pCurrentGraphPageObj = graphPage;
//        m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(view);
//        //graphPage->loadAsXML(szPasteFileName);
//        view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
//        graphPage->setGraphPageId(strDrawPageName);
//    }

//    QList<QListWidgetItem*> listWidgetItem = this->m_pListWidgetGraphPagesObj->findItems(strDrawPageName, Qt::MatchCaseSensitive);
//    if ( listWidgetItem.size() > 0 ) {
//        this->m_pListWidgetGraphPagesObj->setCurrentItem(listWidgetItem.at(0));
//        m_pGraphPageEditorObj->setCurrentIndex(this->m_pListWidgetGraphPagesObj->currentRow());
//    }

//    m_pCurrentGraphPageObj->setUnsavedFlag(true);
//    slotUpdateActions();
}


/**
    * @brief MainWindow::onSlotSetWindowSetTitle
    * @details 设置窗口标题
    * @param szTitle 标题
    */
void MainWindow::onSlotSetWindowSetTitle(const QString &szTitle)
{
    //    ChildForm *window = findMdiChild(this->m_szCurItem);
    //    if(window != Q_NULLPTR) {
    //        window->SetTitle(szTitle);
    //    }
}


/**
    * @brief MainWindow::onSlotTabProjectMgrCurChanged
    * @details 工程管理器标签改变
    * @param index 0：工程标签页, 1：画面标签页
    */
void MainWindow::onSlotTabProjectMgrCurChanged(int index)
{
    QToolBar *pToolBarObj = QSoftCore::getCore()->getToolBar("ToolBar.GraphPage");
    if(QSoftCore::getCore()->getProjectCore()->m_szProjFile == "") {
        m_pMdiAreaObj->setVisible(true);
        if(m_pDesignerWidgetObj) m_pDesignerWidgetObj->setVisible(false);
        if(pToolBarObj) pToolBarObj->setVisible(false); // 画面编辑工具条
    } else {
        if(pToolBarObj) pToolBarObj->setVisible(index == 1);
        switch (index) {
        case 0: {
            m_pCentralWidgetObj->setCurrentWidget(m_pMdiAreaObj);
        }break;
        case 1: {
            if(m_pDesignerWidgetObj) {
                m_pCentralWidgetObj->setCurrentWidget(m_pDesignerWidgetObj);
                QAbstractPage* pPageObj = m_mapNameToPage.value("Designer");
                if(pPageObj) {
                    QSoftCore::getCore()->setActivityStack(pPageObj->getUndoStack());
                }
            }
        }break;
        default: break;
        }
    }
}

/**
    * @brief MainWindow::onSlotTabCloseRequested
    * @details 子窗口关闭请求
    */
void MainWindow::onSlotTabCloseRequested(int index)
{
    QList<QMdiSubWindow*> list = m_pMdiAreaObj->subWindowList();
    if (index < 0 || index >= list.count()) return;
    QMdiSubWindow* mdiSubWindow = list[index];
    mdiSubWindow->close();
}


/**
    * @brief MainWindow::onSlotUpdateMenus
    * @details 更新菜单
    */
void MainWindow::onSlotUpdateMenus()
{
    QAction *pActObj = Q_NULLPTR;

    bool hasMdiChild = (activeMdiChild() != Q_NULLPTR);

    pActObj = QSoftCore::getCore()->getAction("Project.Save");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("Project.Close");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("Window.Close");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("Window.CloseAll");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("Window.Tile");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("Window.Cascade");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("Window.Next");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("Window.Previous");
    if(pActObj) pActObj->setEnabled(hasMdiChild);

    pActObj = QSoftCore::getCore()->getAction("GraphPage.ShowGrid");
    if(pActObj) pActObj->setVisible(hasMdiChild);
}



