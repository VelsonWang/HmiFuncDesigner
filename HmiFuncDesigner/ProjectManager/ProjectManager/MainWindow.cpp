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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_strProjectPath(""),
      m_CurItem(""),
      m_CurTreeViewItem("")
{
    setupUi();

    // 创建动作
    createActions();
    // 创建菜单
    createMenus();
    // 创建工具条
    createToolbars();



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

    listWidgetGraphPages->setContextMenuPolicy(Qt::DefaultContextMenu);
}


void MainWindow::setupUi()
{
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    actionNewPoject = new QAction(this);
    actionNewPoject->setObjectName(QString::fromUtf8("actionNewPoject"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/images/newproject.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewPoject->setIcon(icon1);
    actionNewPoject->setIconVisibleInMenu(true);
    actionOpenProject = new QAction(this);
    actionOpenProject->setObjectName(QString::fromUtf8("actionOpenProject"));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/images/openproject.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionOpenProject->setIcon(icon2);
    actionCloseProject = new QAction(this);
    actionCloseProject->setObjectName(QString::fromUtf8("actionCloseProject"));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/images/projectexit.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionCloseProject->setIcon(icon3);
    actionSaveProject = new QAction(this);
    actionSaveProject->setObjectName(QString::fromUtf8("actionSaveProject"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/images/saveproject.PNG"), QSize(), QIcon::Normal, QIcon::Off);
    actionSaveProject->setIcon(icon4);
    actionRecentProjectList = new QAction(this);
    actionRecentProjectList->setObjectName(QString::fromUtf8("actionRecentProjectList"));
    actionExit = new QAction(this);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/images/programexit.PNG"), QSize(), QIcon::Normal, QIcon::Off);
    actionExit->setIcon(icon5);
    actionToolBar = new QAction(this);
    actionToolBar->setObjectName(QString::fromUtf8("actionToolBar"));
    actionToolBar->setCheckable(true);
    actionStatusBar = new QAction(this);
    actionStatusBar->setObjectName(QString::fromUtf8("actionStatusBar"));
    actionStatusBar->setCheckable(true);
    actionWorkSpace = new QAction(this);
    actionWorkSpace->setObjectName(QString::fromUtf8("actionWorkSpace"));
    actionWorkSpace->setCheckable(true);
    actionWorkSpace->setChecked(true);
    actionDisplayArea_I = new QAction(this);
    actionDisplayArea_I->setObjectName(QString::fromUtf8("actionDisplayArea_I"));
    actionDisplayArea_I->setCheckable(true);
    actionBigIcon = new QAction(this);
    actionBigIcon->setObjectName(QString::fromUtf8("actionBigIcon"));
    actionBigIcon->setCheckable(true);
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/images/viewbig.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionBigIcon->setIcon(icon6);
    actionSmallIcon = new QAction(this);
    actionSmallIcon->setObjectName(QString::fromUtf8("actionSmallIcon"));
    actionSmallIcon->setCheckable(true);
    QIcon icon7;
    icon7.addFile(QString::fromUtf8(":/images/viewlittle.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSmallIcon->setIcon(icon7);
    actionEdit = new QAction(this);
    actionEdit->setObjectName(QString::fromUtf8("actionEdit"));
    actionEdit->setCheckable(true);
    QIcon icon8;
    icon8.addFile(QString::fromUtf8(":/images/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionEdit->setIcon(icon8);
    actionSimulate = new QAction(this);
    actionSimulate->setObjectName(QString::fromUtf8("actionSimulate"));
    actionSimulate->setEnabled(false);
    QIcon icon9;
    icon9.addFile(QString::fromUtf8(":/images/offline.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSimulate->setIcon(icon9);
    actionRun = new QAction(this);
    actionRun->setObjectName(QString::fromUtf8("actionRun"));
    actionRun->setEnabled(true);
    QIcon icon10;
    icon10.addFile(QString::fromUtf8(":/images/online.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionRun->setIcon(icon10);
    actionDownload = new QAction(this);
    actionDownload->setObjectName(QString::fromUtf8("actionDownload"));
    QIcon icon11;
    icon11.addFile(QString::fromUtf8(":/images/download.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDownload->setIcon(icon11);
    actionUpLoad = new QAction(this);
    actionUpLoad->setObjectName(QString::fromUtf8("actionUpLoad"));
    QIcon icon12;
    icon12.addFile(QString::fromUtf8(":/images/upload.PNG"), QSize(), QIcon::Normal, QIcon::Off);
    actionUpLoad->setIcon(icon12);
    actionUDisk = new QAction(this);
    actionUDisk->setObjectName(QString::fromUtf8("actionUDisk"));
    QIcon icon13;
    icon13.addFile(QString::fromUtf8(":/images/downusb.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionUDisk->setIcon(icon13);
    actionAddTag = new QAction(this);
    actionAddTag->setObjectName(QString::fromUtf8("actionAddTag"));
    QIcon icon14;
    icon14.addFile(QString::fromUtf8(":/images/data_add.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionAddTag->setIcon(icon14);
    actionAppendTag = new QAction(this);
    actionAppendTag->setObjectName(QString::fromUtf8("actionAppendTag"));
    QIcon icon15;
    icon15.addFile(QString::fromUtf8(":/images/data_supadd.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionAppendTag->setIcon(icon15);
    actionRowCopyTag = new QAction(this);
    actionRowCopyTag->setObjectName(QString::fromUtf8("actionRowCopyTag"));
    QIcon icon16;
    icon16.addFile(QString::fromUtf8(":/images/data_rowcopy.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionRowCopyTag->setIcon(icon16);
    actionColumnCopyTag = new QAction(this);
    actionColumnCopyTag->setObjectName(QString::fromUtf8("actionColumnCopyTag"));
    QIcon icon17;
    icon17.addFile(QString::fromUtf8(":/images/data_colcopy.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionColumnCopyTag->setIcon(icon17);
    actionModifyTag = new QAction(this);
    actionModifyTag->setObjectName(QString::fromUtf8("actionModifyTag"));
    QIcon icon18;
    icon18.addFile(QString::fromUtf8(":/images/icon_modify.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionModifyTag->setIcon(icon18);
    actionDeleteTag = new QAction(this);
    actionDeleteTag->setObjectName(QString::fromUtf8("actionDeleteTag"));
    QIcon icon19;
    icon19.addFile(QString::fromUtf8(":/images/icon_delete.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDeleteTag->setIcon(icon19);
    actionExportTag = new QAction(this);
    actionExportTag->setObjectName(QString::fromUtf8("actionExportTag"));
    QIcon icon20;
    icon20.addFile(QString::fromUtf8(":/images/data_export.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionExportTag->setIcon(icon20);
    actionImportTag = new QAction(this);
    actionImportTag->setObjectName(QString::fromUtf8("actionImportTag"));
    QIcon icon21;
    icon21.addFile(QString::fromUtf8(":/images/data_inport.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionImportTag->setIcon(icon21);
    actionDeviceNew = new QAction(this);
    actionDeviceNew->setObjectName(QString::fromUtf8("actionDeviceNew"));
    QIcon icon22;
    icon22.addFile(QString::fromUtf8(":/images/icon_new.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDeviceNew->setIcon(icon22);
    actionDeviceModify = new QAction(this);
    actionDeviceModify->setObjectName(QString::fromUtf8("actionDeviceModify"));
    actionDeviceModify->setIcon(icon18);
    actionDeviceDelete = new QAction(this);
    actionDeviceDelete->setObjectName(QString::fromUtf8("actionDeviceDelete"));
    actionDeviceDelete->setIcon(icon19);
    actionHelp = new QAction(this);
    actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
    actionAbout = new QAction(this);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    verticalLayout_7 = new QVBoxLayout(centralWidget);
    verticalLayout_7->setSpacing(6);
    verticalLayout_7->setContentsMargins(11, 11, 11, 11);
    verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
    scrollArea = new QScrollArea(centralWidget);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1036, 69));
    scrollArea->setWidget(scrollAreaWidgetContents);

    verticalLayout_7->addWidget(scrollArea);

    mdiArea = new QMdiArea(centralWidget);
    mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(mdiArea->sizePolicy().hasHeightForWidth());
    mdiArea->setSizePolicy(sizePolicy);

    verticalLayout_7->addWidget(mdiArea);

    this->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(this);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 1337, 23));
    menuProject = new QMenu(menuBar);
    menuProject->setObjectName(QString::fromUtf8("menuProject"));
    menuView = new QMenu(menuBar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menu_T = new QMenu(menuBar);
    menu_T->setObjectName(QString::fromUtf8("menu_T"));
    menu_D = new QMenu(menuBar);
    menu_D->setObjectName(QString::fromUtf8("menu_D"));
    menu = new QMenu(menuBar);
    menu->setObjectName(QString::fromUtf8("menu"));
    this->setMenuBar(menuBar);
    ProjectToolBar = new QToolBar(this);
    ProjectToolBar->setObjectName(QString::fromUtf8("ProjectToolBar"));
    ProjectToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->addToolBar(Qt::TopToolBarArea, ProjectToolBar);
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    this->setStatusBar(statusBar);
    ViewToolBar = new QToolBar(this);
    ViewToolBar->setObjectName(QString::fromUtf8("ViewToolBar"));
    ViewToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->addToolBar(Qt::TopToolBarArea, ViewToolBar);

    // 工程管理器停靠控件
    m_pDockProjectMgrObj = new QDockWidget(this);
    m_pDockProjectMgrObj->setObjectName(QString::fromUtf8("dockProjectManager"));
    m_pDockProjectMgrObj->setWindowTitle(tr("工程管理器"));
    this->addDockWidget(Qt::LeftDockWidgetArea, m_pDockProjectMgrObj);
    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    verticalLayout_4 = new QVBoxLayout(dockWidgetContents);
    verticalLayout_4->setSpacing(6);
    verticalLayout_4->setContentsMargins(0, 0, 0, 0);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    m_pTabProjectMgrObj = new QTabWidget(dockWidgetContents);
    m_pTabProjectMgrObj->setObjectName(QString::fromUtf8("TabProjectMgr"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    verticalLayout_2 = new QVBoxLayout(tab);
    verticalLayout_2->setSpacing(0);
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    treeViewProject = new QTreeView(tab);
    treeViewProject->setObjectName(QString::fromUtf8("treeViewProject"));
    QFont font;
    font.setPointSize(10);
    treeViewProject->setFont(font);
    verticalLayout_2->addWidget(treeViewProject);

    m_pTabProjectMgrObj->addTab(tab, QString(tr("工程")));
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    verticalLayout_3 = new QVBoxLayout(tab_2);
    verticalLayout_3->setSpacing(0);
    verticalLayout_3->setContentsMargins(2, 2, 2, 2);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    verticalLayout_3->setContentsMargins(0, 0, 0, 0);
    listWidgetGraphPages = new QListWidget(tab_2);
    listWidgetGraphPages->setObjectName(QString::fromUtf8("listWidgetGraphPages"));
    verticalLayout_3->addWidget(listWidgetGraphPages);
    m_pTabProjectMgrObj->addTab(tab_2, QString(tr("画面")));
    verticalLayout_4->addWidget(m_pTabProjectMgrObj);
    m_pTabProjectMgrObj->setCurrentIndex(0);

    // 图形元素停靠控件
    m_pDockElemetsObj = new QDockWidget(this);
    m_pDockElemetsObj->setObjectName(QString::fromUtf8("dockElemets"));
    m_pDockElemetsObj->setWindowTitle(tr("图形元素"));
    this->addDockWidget(Qt::RightDockWidgetArea, m_pDockElemetsObj);
    dockWidgetContents_8 = new QWidget();
    dockWidgetContents_8->setObjectName(QString::fromUtf8("dockWidgetContents_8"));
    verticalLayout_5 = new QVBoxLayout(dockWidgetContents_8);
    verticalLayout_5->setSpacing(6);
    verticalLayout_5->setContentsMargins(11, 11, 11, 11);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    verticalLayout_5->setContentsMargins(0, 0, 0, 0);
    ElemetsLayout = new QVBoxLayout();
    ElemetsLayout->setSpacing(6);
    ElemetsLayout->setObjectName(QString::fromUtf8("ElemetsLayout"));
    verticalLayout_5->addLayout(ElemetsLayout);
    m_pDockElemetsObj->setWidget(dockWidgetContents_8);


    // 属性停靠控件
    m_pDockPropertyObj = new QDockWidget(this);
    m_pDockPropertyObj->setObjectName(QString::fromUtf8("dockProperty"));
    m_pDockPropertyObj->setWindowTitle(tr("属性编辑"));
    this->addDockWidget(Qt::RightDockWidgetArea, m_pDockPropertyObj);
    dockPropertyLayout = new QWidget();
    dockPropertyLayout->setObjectName(QString::fromUtf8("dockPropertyLayout"));
    verticalLayout_6 = new QVBoxLayout(dockPropertyLayout);
    verticalLayout_6->setSpacing(1);
    verticalLayout_6->setContentsMargins(11, 11, 11, 11);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    verticalLayout_6->setContentsMargins(1, 1, 1, 1);
    PropertyLayout = new QVBoxLayout();
    PropertyLayout->setSpacing(6);
    PropertyLayout->setObjectName(QString::fromUtf8("PropertyLayout"));
    verticalLayout_6->addLayout(PropertyLayout);
    m_pDockPropertyObj->setWidget(dockPropertyLayout);
    m_pDockProjectMgrObj->setWidget(dockWidgetContents);


    RunToolBar = new QToolBar(this);
    RunToolBar->setObjectName(QString::fromUtf8("RunToolBar"));
    RunToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->addToolBar(Qt::TopToolBarArea, RunToolBar);
    TagOperateToolBar = new QToolBar(this);
    TagOperateToolBar->setObjectName(QString::fromUtf8("TagOperateToolBar"));
    TagOperateToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->addToolBar(Qt::TopToolBarArea, TagOperateToolBar);
    DeviceOperateToolBar = new QToolBar(this);
    DeviceOperateToolBar->setObjectName(QString::fromUtf8("DeviceOperateToolBar"));
    DeviceOperateToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->addToolBar(Qt::TopToolBarArea, DeviceOperateToolBar);
    toolBar = new QToolBar(this);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    this->addToolBar(Qt::TopToolBarArea, toolBar);

    menuBar->addAction(menuProject->menuAction());
    menuBar->addAction(menuView->menuAction());
    menuBar->addAction(menu_T->menuAction());
    menuBar->addAction(menu_D->menuAction());
    menuBar->addAction(menu->menuAction());
    menuProject->addAction(actionNewPoject);
    menuProject->addAction(actionOpenProject);
    menuProject->addAction(actionCloseProject);
    menuProject->addAction(actionSaveProject);
    menuProject->addSeparator();
    menuProject->addAction(actionRecentProjectList);
    menuProject->addSeparator();
    menuProject->addAction(actionExit);
    menuView->addAction(actionToolBar);
    menuView->addAction(actionStatusBar);
    menuView->addAction(actionWorkSpace);
    menuView->addAction(actionDisplayArea_I);
    menuView->addAction(actionBigIcon);
    menuView->addAction(actionSmallIcon);
    menuView->addAction(actionEdit);
    menu_T->addAction(actionSimulate);
    menu_T->addAction(actionRun);
    menu_T->addAction(actionDownload);
    menu_T->addAction(actionUpLoad);
    menu_T->addAction(actionUDisk);
    menu_D->addAction(actionAddTag);
    menu_D->addAction(actionAppendTag);
    menu_D->addAction(actionRowCopyTag);
    menu_D->addAction(actionColumnCopyTag);
    menu_D->addAction(actionModifyTag);
    menu_D->addAction(actionDeleteTag);
    menu_D->addAction(actionExportTag);
    menu_D->addAction(actionImportTag);
    menu->addAction(actionHelp);
    menu->addAction(actionAbout);
    ProjectToolBar->addAction(actionNewPoject);
    ProjectToolBar->addAction(actionOpenProject);
    ProjectToolBar->addAction(actionCloseProject);
    ProjectToolBar->addAction(actionSaveProject);
    ProjectToolBar->addAction(actionExit);
    ViewToolBar->addAction(actionBigIcon);
    ViewToolBar->addAction(actionSmallIcon);
    ViewToolBar->addAction(actionEdit);
    RunToolBar->addAction(actionSimulate);
    RunToolBar->addAction(actionRun);
    RunToolBar->addAction(actionDownload);
    RunToolBar->addAction(actionUpLoad);
    RunToolBar->addAction(actionUDisk);
    TagOperateToolBar->addAction(actionAddTag);
    TagOperateToolBar->addAction(actionAppendTag);
    TagOperateToolBar->addAction(actionRowCopyTag);
    TagOperateToolBar->addAction(actionColumnCopyTag);
    TagOperateToolBar->addAction(actionModifyTag);
    TagOperateToolBar->addAction(actionDeleteTag);
    TagOperateToolBar->addAction(actionExportTag);
    TagOperateToolBar->addAction(actionImportTag);
    DeviceOperateToolBar->addAction(actionDeviceNew);
    DeviceOperateToolBar->addAction(actionDeviceModify);
    DeviceOperateToolBar->addAction(actionDeviceDelete);

    retranslateUi();

    QMetaObject::connectSlotsByName(this);
} // setupUi

void MainWindow::retranslateUi()
{
    this->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    actionNewPoject->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272(&N)", nullptr));
#if QT_CONFIG(shortcut)
    actionNewPoject->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
    actionOpenProject->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200(&O)", nullptr));
#if QT_CONFIG(shortcut)
    actionOpenProject->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
    actionCloseProject->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
    actionSaveProject->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230(&S)", nullptr));
#if QT_CONFIG(shortcut)
    actionSaveProject->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
    actionRecentProjectList->setText(QCoreApplication::translate("MainWindow", "\346\234\200\350\277\221\346\226\207\344\273\266\345\210\227\350\241\250", nullptr));
    actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272(&Q)", nullptr));
#if QT_CONFIG(shortcut)
    actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
    actionToolBar->setText(QCoreApplication::translate("MainWindow", "\345\267\245\345\205\267\346\240\217(&T)", nullptr));
#if QT_CONFIG(shortcut)
    actionToolBar->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+T", nullptr));
#endif // QT_CONFIG(shortcut)
    actionStatusBar->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201\346\240\217(&U)", nullptr));
#if QT_CONFIG(shortcut)
    actionStatusBar->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+U", nullptr));
#endif // QT_CONFIG(shortcut)
    actionWorkSpace->setText(QCoreApplication::translate("MainWindow", "\345\267\245\344\275\234\345\214\272(&W)", nullptr));
#if QT_CONFIG(shortcut)
    actionWorkSpace->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+W", nullptr));
#endif // QT_CONFIG(shortcut)
    actionDisplayArea_I->setText(QCoreApplication::translate("MainWindow", "\346\230\276\347\244\272\345\214\272(&I)", nullptr));
#if QT_CONFIG(shortcut)
    actionDisplayArea_I->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+I", nullptr));
#endif // QT_CONFIG(shortcut)
    actionBigIcon->setText(QCoreApplication::translate("MainWindow", "\345\244\247\345\233\276\346\240\207(&B)", nullptr));
#if QT_CONFIG(shortcut)
    actionBigIcon->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+B", nullptr));
#endif // QT_CONFIG(shortcut)
    actionSmallIcon->setText(QCoreApplication::translate("MainWindow", "\345\260\217\345\233\276\346\240\207(&S)", nullptr));
#if QT_CONFIG(shortcut)
    actionSmallIcon->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+L", nullptr));
#endif // QT_CONFIG(shortcut)
    actionEdit->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221(&E)", nullptr));
#if QT_CONFIG(shortcut)
    actionEdit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
    actionSimulate->setText(QCoreApplication::translate("MainWindow", "\346\250\241\346\213\237", nullptr));
#if QT_CONFIG(shortcut)
    actionSimulate->setShortcut(QCoreApplication::translate("MainWindow", "F5", nullptr));
#endif // QT_CONFIG(shortcut)
    actionRun->setText(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214", nullptr));
#if QT_CONFIG(shortcut)
    actionRun->setShortcut(QCoreApplication::translate("MainWindow", "F6", nullptr));
#endif // QT_CONFIG(shortcut)
    actionDownload->setText(QCoreApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
#if QT_CONFIG(shortcut)
    actionDownload->setShortcut(QCoreApplication::translate("MainWindow", "F9", nullptr));
#endif // QT_CONFIG(shortcut)
    actionUpLoad->setText(QCoreApplication::translate("MainWindow", "\344\270\212\350\275\275", nullptr));
#if QT_CONFIG(shortcut)
    actionUpLoad->setShortcut(QCoreApplication::translate("MainWindow", "F10", nullptr));
#endif // QT_CONFIG(shortcut)
    actionUDisk->setText(QCoreApplication::translate("MainWindow", "U\347\233\230", nullptr));
#if QT_CONFIG(shortcut)
    actionUDisk->setShortcut(QCoreApplication::translate("MainWindow", "F11", nullptr));
#endif // QT_CONFIG(shortcut)
    actionAddTag->setText(QCoreApplication::translate("MainWindow", "\345\242\236\345\212\240", nullptr));
#if QT_CONFIG(shortcut)
    actionAddTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+A", nullptr));
#endif // QT_CONFIG(shortcut)
    actionAppendTag->setText(QCoreApplication::translate("MainWindow", "\350\277\275\345\212\240", nullptr));
#if QT_CONFIG(shortcut)
    actionAppendTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+S", nullptr));
#endif // QT_CONFIG(shortcut)
    actionRowCopyTag->setText(QCoreApplication::translate("MainWindow", "\350\241\214\346\213\267", nullptr));
#if QT_CONFIG(shortcut)
    actionRowCopyTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+R", nullptr));
#endif // QT_CONFIG(shortcut)
    actionColumnCopyTag->setText(QCoreApplication::translate("MainWindow", "\345\210\227\346\213\267", nullptr));
#if QT_CONFIG(shortcut)
    actionColumnCopyTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+C", nullptr));
#endif // QT_CONFIG(shortcut)
    actionModifyTag->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271", nullptr));
#if QT_CONFIG(shortcut)
    actionModifyTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+M", nullptr));
#endif // QT_CONFIG(shortcut)
    actionDeleteTag->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
#if QT_CONFIG(shortcut)
    actionDeleteTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+D", nullptr));
#endif // QT_CONFIG(shortcut)
    actionExportTag->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272", nullptr));
#if QT_CONFIG(shortcut)
    actionExportTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+B", nullptr));
#endif // QT_CONFIG(shortcut)
    actionImportTag->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245", nullptr));
#if QT_CONFIG(shortcut)
    actionImportTag->setShortcut(QCoreApplication::translate("MainWindow", "Alt+E", nullptr));
#endif // QT_CONFIG(shortcut)
    actionDeviceNew->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272", nullptr));
#if QT_CONFIG(shortcut)
    actionDeviceNew->setShortcut(QCoreApplication::translate("MainWindow", "Alt+N", nullptr));
#endif // QT_CONFIG(shortcut)
    actionDeviceModify->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271", nullptr));
#if QT_CONFIG(shortcut)
    actionDeviceModify->setShortcut(QCoreApplication::translate("MainWindow", "Alt+M", nullptr));
#endif // QT_CONFIG(shortcut)
    actionDeviceDelete->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
#if QT_CONFIG(shortcut)
    actionDeviceDelete->setShortcut(QCoreApplication::translate("MainWindow", "Alt+T", nullptr));
#endif // QT_CONFIG(shortcut)
    actionHelp->setText(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
#if QT_CONFIG(shortcut)
    actionHelp->setShortcut(QCoreApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_CONFIG(shortcut)
    actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
#if QT_CONFIG(shortcut)
    actionAbout->setShortcut(QCoreApplication::translate("MainWindow", "F2", nullptr));
#endif // QT_CONFIG(shortcut)
    menuProject->setTitle(QCoreApplication::translate("MainWindow", "\345\267\245\347\250\213(&P)", nullptr));
    menuView->setTitle(QCoreApplication::translate("MainWindow", "\346\237\245\347\234\213(&V)", nullptr));
    menu_T->setTitle(QCoreApplication::translate("MainWindow", "\345\267\245\345\205\267(T)", nullptr));
    menu_D->setTitle(QCoreApplication::translate("MainWindow", "\346\223\215\344\275\234(D)", nullptr));
    menu->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251(H)", nullptr));
    ViewToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    RunToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    TagOperateToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    DeviceOperateToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_2", nullptr));
    toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
}


MainWindow::~MainWindow()
{
    delete pTreeViewProjectModel;
    pTreeViewProjectModel = Q_NULLPTR;
}


/**
 * @brief MainWindow::createActions
 * @details 创建动作
 */
void MainWindow::createActions()
{



    //-----------------------------<画面编辑器>----------------------------------

    // 窗口.图形元素
    m_pActionShowGraphObj = new QAction(tr("图形元素"), this);
    m_pActionShowGraphObj->setCheckable(true);
    m_pActionShowGraphObj->setChecked(true);
    connect(m_pActionShowGraphObj, SIGNAL(triggered(bool)), SLOT(onSlotShowGraphObj(bool)));

    // 窗口.属性编辑器
    m_pActionShowPropEditorObj = new QAction(tr("属性编辑器"), this);
    m_pActionShowPropEditorObj->setCheckable(true);
    m_pActionShowPropEditorObj->setChecked(true);
    connect(m_pActionShowPropEditorObj, SIGNAL(triggered(bool)), SLOT(onSlotShowPropEditor(bool)));

    // 画面.新建
    m_pActionNewGraphPageObj = new QAction(QIcon(":/DrawAppImages/filenew.png"), tr("新建"), this);
    m_pActionNewGraphPageObj->setShortcut(QString("Ctrl+N"));
    connect(m_pActionNewGraphPageObj, SIGNAL(triggered()), SLOT(onSlotNewGraphPage()));

    // 画面.打开
    m_pActionOpenObj = new QAction(QIcon(":/DrawAppImages/fileopen.png"), tr("打开"), this);
    m_pActionOpenObj->setShortcut(QString("Ctrl+O"));
    connect(m_pActionOpenObj, SIGNAL(triggered()), SLOT(onSlotEditOpen()));

    // 画面.保存
    m_pActionSaveGraphPageObj = new QAction(QIcon(":/DrawAppImages/saveproject.png"), tr("保存"), this);
    m_pActionSaveGraphPageObj->setShortcut(QKeySequence::Save);
    connect(m_pActionSaveGraphPageObj, SIGNAL(triggered()), SLOT(onSlotSaveGraphPage()));

    actionCloseGraphPage_ = new QAction(tr("关闭"), this);
    connect(actionCloseGraphPage_, SIGNAL(triggered()), SLOT(slotCloseGraphPage()));

    actionCloseAll_ = new QAction(tr("关闭所有"), this);
    connect(actionCloseAll_, SIGNAL(triggered()), SLOT(slotCloseAll()));

    // 显示栅格
    m_pActionShowGridObj = new QAction(QIcon(":/DrawAppImages/showgrid.png"), tr("显示栅格"), this);
    m_pActionShowGridObj->setCheckable(true);
    m_pActionShowGridObj->setChecked(gridVisible_);
    connect(m_pActionShowGridObj, SIGNAL(triggered(bool)), SLOT(onSlotShowGrid(bool)));

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


/**
 * @brief MainWindow::createMenus
 * @details 创建菜单
 */
void MainWindow::createMenus()
{

    //-----------------------------<画面编辑器>----------------------------------
    QMenu *filemenu = new QMenu(tr("画面"), this);
#if 0  // for test we need this
    filemenu->addAction(m_pActionNewGraphPageObj);
    filemenu->addAction(m_pActionOpenObj);
#endif
    filemenu->addAction(m_pActionSaveGraphPageObj);
    filemenu->addSeparator();
    filemenu->addAction(actionCloseGraphPage_);
    filemenu->addAction(actionCloseAll_);
    filemenu->addSeparator();


    QMenu *windowMenu = new QMenu(tr("窗口"), this);
    windowMenu->addAction(m_pActionShowGraphObj);
    windowMenu->addAction(m_pActionShowPropEditorObj);

    QMainWindow::menuBar()->addMenu(filemenu);
    QMainWindow::menuBar()->addMenu(windowMenu);
}


/**
 * @brief MainWindow::createToolbars
 * @details 创建工具条
 */
void MainWindow::createToolbars()
{

    //-----------------------------<画面编辑器>----------------------------------
    this->toolBar->addAction(m_pActionSaveGraphPageObj);
    this->toolBar->addSeparator();
    this->toolBar->addAction(m_pActionShowGridObj); // 显示栅格
    //toolBar->addAction(actionShowLinear);
    this->toolBar->addAction(actionZoomOut_);
    this->toolBar->addAction(actionZoomIn_);
    this->toolBar->addSeparator();
    this->toolBar->addAction(actionUndo_);
    this->toolBar->addAction(actionRedo_);
    this->toolBar->addSeparator();
    this->toolBar->addAction(actionCopy_); // 拷贝
    this->toolBar->addAction(actionPaste_); // 粘贴
    this->toolBar->addAction(actionDelete_); // 删除
    this->toolBar->addSeparator();
    this->toolBar->addAction(alignTopAction_); // 顶部对齐
    this->toolBar->addAction(alignDownAction_); // 底部对齐
    this->toolBar->addAction(alignLeftAction_); // 左对齐
    this->toolBar->addAction(alignRightAction_); // 右对齐
    this->toolBar->addAction(hUniformDistributeAction_); // 水平均匀分布
    this->toolBar->addAction(vUniformDistributeAction_); // 垂直均匀分布
    this->toolBar->addAction(setTheSameSizeAction_); // 设置选中控件大小一致
    this->toolBar->addAction(upLayerAction_); // 上移一层
    this->toolBar->addAction(downLayerAction_); // 下移一层
    this->toolBar->addSeparator();
}


// 工程管理器ui初始化
void MainWindow::setUpProjectTreeView()
{
    treeViewProject->setHeaderHidden(true);

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
    this->treeViewProject->setModel(pTreeViewProjectModel);
    this->treeViewProject->expandAll();
}

ChildForm *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = this->mdiArea->activeSubWindow()) {
        return qobject_cast<ChildForm *>(activeSubWindow->widget());
    }
    return nullptr;
}

void MainWindow::setActiveSubWindow(ChildForm *window)
{
    if (!window) return;
    window->showMaximized();
    m_CurItem = window->windowTitle();
    this->mdiArea->setActiveSubWindow(Q_NULLPTR);  // Activates the subwindow window. If
    // window is 0, any current active window
    // is deactivated.
    this->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

ChildForm *MainWindow::getActiveSubWindow()
{
    return qobject_cast<ChildForm *>(this->mdiArea->activeSubWindow()->widget());
}

ChildForm *MainWindow::findMdiChild(const QString &windowTitle)
{
    foreach (QMdiSubWindow *window, this->mdiArea->subWindowList()) {
        ChildForm *pChildWin = qobject_cast<ChildForm *>(window->widget());
        if (pChildWin->windowTitle() == windowTitle) return pChildWin;
    }
    return nullptr;
}

QMdiSubWindow *MainWindow::findMdiSubWindow(const QString &windowTitle)
{
    foreach (QMdiSubWindow *window, this->mdiArea->subWindowList()) {
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
    QModelIndex index = this->treeViewProject->currentIndex();
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
    QModelIndex ModelIndex = this->treeViewProject->selectionModel()->currentIndex();
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
    QModelIndex ModelIndex = this->treeViewProject->selectionModel()->currentIndex();
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

    QModelIndex index = this->treeViewProject->currentIndex();
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
    this->mdiArea->closeAllSubWindows();
    writeSettings();
    if (this->mdiArea->currentSubWindow()) {
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
    setCentralWidget(this->mdiArea);
    //    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("HmiFuncDesigner跨平台自动化软件"));

    // 当多文档区域的内容超出可视区域后，出现滚动条
    this->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//    this->mdiArea->setLineWidth(3);
//    this->mdiArea->setFrameShape(QFrame::Panel);
//    this->mdiArea->setFrameShadow(QFrame::Sunken);
//    this->mdiArea->setViewMode(QMdiArea::TabbedView);

    this->statusBar->showMessage(tr("欢迎使用HmiFuncDesigner组态系统"));
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

void MainWindow::on_actionWorkSpace_triggered(bool checked)
{
    this->m_pDockProjectMgrObj->setVisible(checked);
}

/*
* 工具条使能
*/
void MainWindow::enableToolBar(QString text) {
    bool bTagIOOrTmp = (text == tr("中间变量")) | (text.startsWith(tr("设备变量")));
    this->TagOperateToolBar->setEnabled(bTagIOOrTmp);

    bool bdevice = (text == tr("串口设备")) | (text == tr("网络设备")) |
            (text == tr("总线设备")) | (text == tr("OPC设备"));
    this->DeviceOperateToolBar->setEnabled(bdevice);
}

void MainWindow::onTreeViewProjectClicked(const QString &szItemText)
{
    QString winTittle = szItemText;

    ChildForm *findForm = findMdiChild(m_strProjectName);
    if(findForm == Q_NULLPTR) {
        findForm = new ChildForm(this, m_strProjectName);
        findForm->setWindowTitle(m_strProjectName);
        this->mdiArea->addSubWindow(findForm);
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
        this->actionRun->setEnabled(true);
    } else {
        m_strProjectName = "";
        m_strProjectPath = "";
        this->actionRun->setEnabled(false);
        pTreeViewProjectModel->clear();
        this->treeViewProject->reset();
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
    foreach (QMdiSubWindow* window, this->mdiArea->subWindowList()) {
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
    foreach (QMdiSubWindow* window, this->mdiArea->subWindowList()) {
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
            this->menuProject->insertAction(this->actionRecentProjectList, pAct);
        }
        this->menuProject->removeAction(this->actionRecentProjectList);
        return;
    }

    QList<QAction *> listActRemove;
    QList<QAction *> listAct = this->menuProject->actions();
    for (int i = 0; i<listAct.size(); ++i) {
        QAction *pAct = listAct.at(i);
        if(pAct->isSeparator())
            listActRemove.append(pAct);
        if(pAct->text() == tr("最近文件列表"))
            listActRemove.append(pAct);
    }
    for (int j = 0; j<listActRemove.size(); ++j) {
        this->menuProject->removeAction(listActRemove.at(j));
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
        QList<QAction *> listAct = this->menuProject->actions();
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
            this->menuProject->removeAction(listActRemove.at(j));
        }

        /////////////////////////////////////////////////////

        bStart = bEnd = false;
        QAction *pActPos = Q_NULLPTR;
        listAct.clear();
        listAct = this->menuProject->actions();
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
            this->menuProject->insertAction(pActPos, pAct);
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
    this->actionBigIcon->setChecked(true);
    this->actionSmallIcon->setChecked(false);
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
    this->actionBigIcon->setChecked(false);
    this->actionSmallIcon->setChecked(true);
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
    this->scrollArea->setWidget(graphPageTabWidget_);

    elementWidget_ = new ElementLibraryWidget();
    this->ElemetsLayout->addWidget(elementWidget_);

    variantEditorFactory_ = new VariantFactory(this);

    //propertyEditor_ = new QtTreePropertyBrowser(dockProperty);
    propertyEditor_ = new QtTreePropertyBrowser(this);
    propertyEditor_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    propertyEditor_->setHeaderLabels(QStringList() << tr("属性") << tr("值"));
    //propertyEditor_->setColumnWidth(0, 60);
    //propertyEditor_->setColumnWidth(1, 200);


    this->PropertyLayout->addWidget(propertyEditor_);

    VariantManager *pVariantManager  = new VariantManager(this);
    variantPropertyManager_ = pVariantManager;
    pVariantManager->setPropertyEditor(propertyEditor_);
    propertyEditor_->setFactoryForManager(variantPropertyManager_, variantEditorFactory_);

    connect(variantPropertyManager_, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));
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
        this->listWidgetGraphPages->addItem(szPageId);
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

    QList<QListWidgetItem*> listWidgetItem = this->listWidgetGraphPages->findItems(szPageName, Qt::MatchCaseSensitive);
    if ( listWidgetItem.size() > 0 ) {
        this->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
    }

}


/**
 * @brief MainWindow::onSlotNewGraphPage
 * @details [画面.新建] 动作响应函数
 */
void MainWindow::onSlotNewGraphPage()
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
    m_pActionShowGridObj->setEnabled(graphPageTabWidget_->count() ? true : false);

    if (!currentGraphPage_) {
        return;
    }

    undoGroup_->setActiveStack(currentGraphPage_->undoStack());

    if (!currentGraphPage_->undoStack()->isClean() || currentGraphPage_->getUnsavedFlag()) {
        m_pActionSaveGraphPageObj->setEnabled(true);
    } else {
        m_pActionSaveGraphPageObj->setEnabled(false);
    }
}

void MainWindow::slotChangeGraphPage(int GraphPageNum)
{
    if (GraphPageNum == -1) {
        return;
    }

    this->listWidgetGraphPages->setCurrentRow(GraphPageNum);

    for (int i = 0; i < graphPageTabWidget_->count(); i++) {
        QGraphicsView *view = dynamic_cast<QGraphicsView *>(graphPageTabWidget_->widget(i));
        dynamic_cast<GraphPage *>(view->scene())->setActive(false);
    }

    currentView_ = dynamic_cast<QGraphicsView *>(graphPageTabWidget_->widget(GraphPageNum));
    currentGraphPage_ = dynamic_cast<GraphPage *>(currentView_->scene());
    currentGraphPage_->setActive(true);
    //currentGraphPage_->fillGraphPagePropertyModel();
    currentGraphPageIndex_ = GraphPageNum;
    slotUpdateActions();
}

void MainWindow::slotChangeGraphPageName()
{
    graphPageTabWidget_->setTabText(currentGraphPageIndex_, currentGraphPage_->getGraphPageId());
    //int index = GraphPageManager::getInstance()->getIndexByGraphPage(currentGraphPage_);
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

void MainWindow::propertyValueChanged(QtProperty *property, const QVariant &value)
{
    Q_UNUSED(property)
    Q_UNUSED(value)
}

void MainWindow::slotNewElementAdded()
{

}

void MainWindow::slotElementsDeleted()
{

}


/**
 * @brief MainWindow::onSlotShowGrid
 * @details 显示栅格
 * @param on true-显示，false-隐藏
 */
void MainWindow::onSlotShowGrid(bool on)
{
    QListIterator <GraphPage*> iter(GraphPageManager::getInstance()->getGraphPageList());

    while (iter.hasNext()) {
        iter.next()->setGridVisible(on);
    }

    gridVisible_ = on;
}


/**
 * @brief MainWindow::onSlotShowGraphObj
 * @details [窗口.图形元素] 动作响应函数; 图形元素停靠控件的显示或隐藏
 * @param on true-显示, false-隐藏
 */
void MainWindow::onSlotShowGraphObj(bool on)
{
    this->m_pDockElemetsObj->setVisible(on);
}


/**
 * @brief MainWindow::slotShowPropEditor
 * @details [窗口.属性编辑器] 动作响应函数; 属性编辑器停靠控件的显示或隐藏
 * @param on true-显示, false-隐藏
 */
void MainWindow::onSlotShowPropEditor(bool on)
{
    this->m_pDockPropertyObj->setVisible(on);
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
            onSlotSaveGraphPage();
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


/**
 * @brief MainWindow::onSlotEditOpen
 * @details [画面.打开] 动作响应函数
 */
void MainWindow::onSlotEditOpen()
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

/**
 * @brief MainWindow::onSlotEditOpen
 * @details [画面.保存] 动作响应函数
 */
void MainWindow::onSlotSaveGraphPage()
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
    graphPageTabWidget_->setCurrentIndex(this->listWidgetGraphPages->currentRow());
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

        this->listWidgetGraphPages->addItem(szGraphPageName);
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

        QList<QListWidgetItem*> listWidgetItem = this->listWidgetGraphPages->findItems(szGraphPageName, Qt::MatchCaseSensitive);
        if ( listWidgetItem.size() > 0 ) {
            this->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
            graphPageTabWidget_->setCurrentIndex(this->listWidgetGraphPages->currentRow());
        }
    }
}


/**
 * @brief MainWindow::onRenameGraphPage
 * @details 修改画面名称
 */
void MainWindow::onRenameGraphPage()
{
    QString szOldGraphPageName = this->listWidgetGraphPages->currentItem()->text();

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
                this->listWidgetGraphPages->currentItem()->setText(szNewGraphPageName);
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
    QString szGraphPageName = this->listWidgetGraphPages->currentItem()->text();

    for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
        if ( szGraphPageName == DrawListUtils::drawList_.at(i) ) {
            DrawListUtils::drawList_.removeAt(i);

            QString fileName = szProjPath_ + "/" + szGraphPageName + ".drw";
            QFile file(fileName);
            if (file.exists()) {
                file.remove();
            }

            graphPageTabWidget_->removeTab(this->listWidgetGraphPages->currentRow());

            GraphPage *pGraphPageObj = GraphPageManager::getInstance()->getGraphPageById(szGraphPageName);
            if ( pGraphPageObj != Q_NULLPTR ) {
                GraphPageManager::getInstance()->removeGraphPage(pGraphPageObj);
                delete pGraphPageObj;
                pGraphPageObj = Q_NULLPTR;
            }

            DrawListUtils::saveDrawList(szProjPath_);

            this->listWidgetGraphPages->clear();
            foreach(QString szPageId, DrawListUtils::drawList_) {
                this->listWidgetGraphPages->addItem(szPageId);
            }

            if (this->listWidgetGraphPages->count() > 0) {
                this->listWidgetGraphPages->setCurrentRow(0);
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
    szCopyGraphPageFileName_ = this->listWidgetGraphPages->currentItem()->text();
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

    this->listWidgetGraphPages->addItem(strDrawPageName);
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

    QList<QListWidgetItem*> listWidgetItem = this->listWidgetGraphPages->findItems(strDrawPageName, Qt::MatchCaseSensitive);
    if ( listWidgetItem.size() > 0 ) {
        this->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
        graphPageTabWidget_->setCurrentIndex(this->listWidgetGraphPages->currentRow());
    }

    currentGraphPage_->setUnsavedFlag(true);
    slotUpdateActions();
}
