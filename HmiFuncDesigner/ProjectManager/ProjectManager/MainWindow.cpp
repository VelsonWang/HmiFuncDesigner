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
      m_szProjPath(""),
      m_szCurItem(""),
      m_szCurTreeViewItem("")
{
    setupUi();

    this->setWindowIcon(QIcon(":/images/appicon.png"));

    // 创建状态栏
    createStatusBar();
    // 创建动作
    createActions();
    // 创建菜单
    createMenus();
    // 创建工具条
    createToolbars();



    m_pCurrentGraphPageObj = Q_NULLPTR;
    m_pCurrentViewObj = Q_NULLPTR;
    m_bGraphPageGridVisible = true;
    m_iCurrentGraphPageIndex = 0;

    enableToolBar(""); // 工具条使能
    setContextMenuPolicy(Qt::DefaultContextMenu); // 右键菜单生效
    readSettings();  // 初始窗口时读取窗口设置信息
    initWindow(); // 初始化窗口
    setUpProjectTreeView();
    loadRecentProjectList();
    onBigIcon();  // 大图标显示

    //--------------------------------------------------------------------------

    m_pUndoGroupObj = new QUndoGroup(this);
    createActions();

    createMenus();
    createToolbars();
    initView();
    slotUpdateActions();

    setWindowIcon(QIcon(":/DrawAppImages/application.png"));

    connect(m_pGraphPageTabWidgetObj, SIGNAL(currentChanged(int)), SLOT(slotChangeGraphPage(int)));

    QDesktopWidget * pDesktopWidget = QApplication::desktop();
    QRect rect = pDesktopWidget->screenGeometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    this->resize(screenWidth*3/4, screenHeight*3/4);

    Helper::WidgetMoveCenter(this);

    DrawListUtils::setProjectPath(m_szProjPath);

    listWidgetGraphPages->setContextMenuPolicy(Qt::DefaultContextMenu);
}


/**
 * @brief MainWindow::createStatusBar
 * @details 创建状态栏
 */
void MainWindow::createStatusBar()
{
    m_pStatusBarObj = new QStatusBar(this);
    m_pStatusBarObj->setObjectName(QString::fromUtf8("statusBar"));
    this->setStatusBar(m_pStatusBarObj);
}

void MainWindow::setupUi()
{




    actionDeviceNew = new QAction(this);
    actionDeviceNew->setObjectName(QString::fromUtf8("actionDeviceNew"));
    QIcon icon22;
    icon22.addFile(QString::fromUtf8(":/images/icon_new.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDeviceNew->setIcon(icon22);
    actionDeviceModify = new QAction(this);
    actionDeviceModify->setObjectName(QString::fromUtf8("actionDeviceModify"));
    //actionDeviceModify->setIcon(icon18);
    actionDeviceDelete = new QAction(this);
    actionDeviceDelete->setObjectName(QString::fromUtf8("actionDeviceDelete"));
    //actionDeviceDelete->setIcon(icon19);
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
    connect(listWidgetGraphPages, SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(onSlotListWidgetGraphPagesCurrentTextChanged(const QString &)));

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




    DeviceOperateToolBar = new QToolBar(this);
    DeviceOperateToolBar->setObjectName(QString::fromUtf8("DeviceOperateToolBar"));
    DeviceOperateToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->addToolBar(Qt::TopToolBarArea, DeviceOperateToolBar);
    toolBar = new QToolBar(this);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    this->addToolBar(Qt::TopToolBarArea, toolBar);





    this->menuBar()->addAction(menu->menuAction());







    menu->addAction(actionHelp);
    menu->addAction(actionAbout);






    DeviceOperateToolBar->addAction(actionDeviceNew);
    DeviceOperateToolBar->addAction(actionDeviceModify);
    DeviceOperateToolBar->addAction(actionDeviceDelete);

    QMetaObject::connectSlotsByName(this);
} // setupUi


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
    // 新建工程
    m_pActionNewProjObj = new QAction(QIcon(":/images/newproject.png"), tr("新建工程"), this);
    m_pActionNewProjObj->setShortcut(QString("Ctrl+N"));
    connect(m_pActionNewProjObj, &QAction::triggered, this, &MainWindow::onNewPoject);

    //  打开工程
    m_pActionOpenProjObj = new QAction(QIcon(":/images/openproject.png"), tr("打开工程"), this);
    m_pActionOpenProjObj->setShortcut(QString("Ctrl+O"));
    connect(m_pActionOpenProjObj, &QAction::triggered, this, &MainWindow::onOpenProject);

    // 关闭工程
    m_pActionCloseProjObj = new QAction(QIcon(":/images/projectexit.png"), tr("关闭"), this);
    connect(m_pActionCloseProjObj, &QAction::triggered, this, &MainWindow::onCloseProject);

    // 保存工程
    m_pActionSaveProjObj = new QAction(QIcon(":/images/saveproject.png"), tr("保存"), this);
    m_pActionSaveProjObj->setShortcut(QString("Ctrl+S"));
    connect(m_pActionSaveProjObj, &QAction::triggered, this, &MainWindow::onSaveProject);

    // 最近打开的工程
    m_pActionRecentProjListObj = new QAction(tr("最近打开的工程"), this);

    m_pActionExitObj = new QAction(QIcon(":/images/programexit.png"), tr("退出"), this);
    m_pActionExitObj->setShortcut(QString("Ctrl+Q"));
    connect(m_pActionExitObj, &QAction::triggered, this, &MainWindow::onExit);

    //-----------------------------<视图>---------------------------------------

    // 视图工具栏
    m_pActionToolBarObj = new QAction(tr("视图工具栏"), this);
    m_pActionToolBarObj->setCheckable(true);

    // 状态栏
    m_pActionStatusBarObj = new QAction(tr("状态栏"), this);
    m_pActionStatusBarObj->setCheckable(true);

    // 工作区
    m_pActionWorkSpaceObj = new QAction(tr("工作区"), this);
    m_pActionWorkSpaceObj->setCheckable(true);

    // 显示区
    m_pActionDisplayAreaObj = new QAction(tr("显示区"), this);
    m_pActionDisplayAreaObj->setCheckable(true);

    // 大图标显示
    m_pActionBigIconObj = new QAction(QIcon(":/images/viewbig.png"), tr("大图标"), this);
    m_pActionExitObj->setShortcut(QString("Ctrl+B"));
    m_pActionBigIconObj->setCheckable(true);
    connect(m_pActionBigIconObj, &QAction::triggered, this, &MainWindow::onBigIcon);

    // 显示小图标
    m_pActionSmallIconObj = new QAction(QIcon(":/images/viewlittle.png"), tr("小图标"), this);
    m_pActionSmallIconObj->setShortcut(QString("Ctrl+L"));
    m_pActionSmallIconObj->setCheckable(true);
    connect(m_pActionSmallIconObj, &QAction::triggered, this, &MainWindow::onSmallIcon);

    // 编辑
    m_pActionEditObj = new QAction(QIcon(":/images/edit.png"), tr("编辑"), this);
    m_pActionEditObj->setShortcut(QString("Ctrl+E"));
    m_pActionEditObj->setCheckable(true);


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

    // 关闭画面
    m_pActionCloseGraphPageObj = new QAction(tr("关闭"), this);
    connect(m_pActionCloseGraphPageObj, SIGNAL(triggered()), SLOT(onSlotCloseGraphPage()));

    // 关闭所有画面
    m_pActionCloseAllObj = new QAction(tr("关闭所有"), this);
    connect(m_pActionCloseAllObj, SIGNAL(triggered()), SLOT(slotCloseAll()));

    // 显示栅格
    m_pActionShowGridObj = new QAction(QIcon(":/DrawAppImages/showgrid.png"), tr("显示栅格"), this);
    m_pActionShowGridObj->setCheckable(true);
    m_pActionShowGridObj->setChecked(m_bGraphPageGridVisible);
    connect(m_pActionShowGridObj, SIGNAL(triggered(bool)), SLOT(onSlotShowGrid(bool)));

    // 画面放大
    m_pActionZoomInObj = new QAction(QIcon(":/DrawAppImages/zoom-in.png"), tr("放大"), this);
    connect(m_pActionZoomInObj, SIGNAL(triggered()), SLOT(slotZoomIn()));

    // 画面缩小
    m_pActionZoomOutObj = new QAction(QIcon(":/DrawAppImages/zoom-out.png"), tr("缩小"), this);
    connect(m_pActionZoomOutObj, SIGNAL(triggered()), SLOT(slotZoomOut()));

    // 撤销
    m_pActionUndoObj = m_pUndoGroupObj->createUndoAction(this);
    m_pActionUndoObj->setIcon(QIcon(":/DrawAppImages/undo.png"));
    m_pActionUndoObj->setText(tr("撤销"));
    m_pActionUndoObj->setShortcut(QKeySequence::Undo);

    // 重做
    m_pActionRedoObj = m_pUndoGroupObj->createRedoAction(this);
    m_pActionRedoObj->setText(tr("重做"));
    m_pActionRedoObj->setIcon(QIcon(":/DrawAppImages/redo.png"));
    m_pActionRedoObj->setShortcut(QKeySequence::Redo);

    // 删除画面
    m_pActionDeleteObj = new QAction(QIcon(":/DrawAppImages/delete.png"), tr("删除"));
    m_pActionDeleteObj->setShortcut(QKeySequence::Delete);
    connect(m_pActionDeleteObj, SIGNAL(triggered()), SLOT(slotEditDelete()));

    // 拷贝画面
    m_pActionCopyObj = new QAction(QIcon(":/DrawAppImages/editcopy.png"),tr("拷贝"));
    m_pActionCopyObj->setShortcut(QKeySequence::Copy);
    connect(m_pActionCopyObj, SIGNAL(triggered()), SLOT(onSlotEditCopy()));

    // 粘贴画面
    m_pActionPasteObj = new QAction(QIcon(":/DrawAppImages/editpaste.png"),tr("粘贴"));
    m_pActionPasteObj->setShortcut(QKeySequence::Paste);
    connect(m_pActionPasteObj, SIGNAL(triggered()), SLOT(slotEditPaste()));

    // 顶部对齐
    m_pActionAlignTopObj = new QAction(QIcon(":/DrawAppImages/align-top.png"), tr("顶部对齐"));
    m_pActionAlignTopObj->setData(Qt::AlignTop);
    connect(m_pActionAlignTopObj, SIGNAL(triggered()), SLOT(onSlotAlignElements()));

    // 底部对齐
    m_pActionAlignDownObj = new QAction(QIcon(":/DrawAppImages/align-bottom.png"), tr("底部对齐"));
    m_pActionAlignDownObj->setData(Qt::AlignBottom);
    connect(m_pActionAlignDownObj, SIGNAL(triggered()), SLOT(onSlotAlignElements()));

    // 右对齐
    m_pActionAlignRightObj = new QAction(QIcon(":/DrawAppImages/align-right.png"), tr("右对齐"));
    m_pActionAlignRightObj->setData(Qt::AlignRight);
    connect(m_pActionAlignRightObj, SIGNAL(triggered()), SLOT(onSlotAlignElements()));

    // 左对齐
    m_pActionAalignLeftObj = new QAction(QIcon(":/DrawAppImages/align-left.png"), tr("左对齐"));
    m_pActionAalignLeftObj->setData(Qt::AlignLeft);
    connect(m_pActionAalignLeftObj, SIGNAL(triggered()), SLOT(onSlotAlignElements()));

    // 水平均匀分布
    m_pActionHUniformDistributeObj = new QAction(QIcon(":/DrawAppImages/align_hsame.png"), tr("水平均匀分布"));
    connect(m_pActionHUniformDistributeObj, SIGNAL(triggered()), SLOT(onSlotHUniformDistributeElements()));

    // 垂直均匀分布
    m_pActionVUniformDistributeObj = new QAction(QIcon(":/DrawAppImages/align_vsame.png"), tr("垂直均匀分布"));
    connect(m_pActionVUniformDistributeObj, SIGNAL(triggered()), SLOT(onSlotVUniformDistributeElements()));

    // 设置选中控件大小一致
    m_pActionSetTheSameSizeObj = new QAction(QIcon(":/DrawAppImages/the-same-size.png"), tr("大小一致"));
    connect(m_pActionSetTheSameSizeObj, SIGNAL(triggered()), SLOT(onSlotSetTheSameSizeElements()));

    // 上移一层
    m_pActionUpLayerObj = new QAction(QIcon(":/DrawAppImages/posfront.png"), tr("上移一层"));
    connect(m_pActionUpLayerObj, SIGNAL(triggered()), SLOT(onSlotUpLayerElements()));

    // 下移一层
    m_pActionDownLayerObj = new QAction(QIcon(":/DrawAppImages/posback.png"), tr("下移一层"));
    connect(m_pActionDownLayerObj, SIGNAL(triggered()), SLOT(onSlotDownLayerElements()));

    //-----------------------------<工具菜单>----------------------------------
    // 模拟仿真
    m_pActionSimulateObj = new QAction(QIcon(":/images/offline.png"), tr("模拟仿真"));
    m_pActionSimulateObj->setEnabled(false);
    connect(m_pActionSimulateObj, SIGNAL(triggered()), SLOT(onSlotSimulate()));

    // 运行工程
    m_pActionRunObj = new QAction(QIcon(":/images/online.png"), tr("运行"));
    m_pActionRunObj->setObjectName(QString::fromUtf8("actionRun"));
    m_pActionRunObj->setEnabled(true);
    connect(m_pActionSimulateObj, SIGNAL(triggered()), SLOT(onSlotRunProject()));

    // 下载工程
    m_pActionDownloadObj = new QAction(QIcon(":/images/download.png"), tr("下载"));
    m_pActionDownloadObj->setObjectName(QString::fromUtf8("actionDownload"));
    connect(m_pActionDownloadObj, SIGNAL(triggered()), SLOT(onSlotDownloadProject()));

    // 上载工程
    m_pActionUpLoadObj = new QAction(QIcon(":/images/upload.png"), tr("上载"));
    m_pActionUpLoadObj->setObjectName(QString::fromUtf8("actionUpLoad"));
    connect(m_pActionUpLoadObj, SIGNAL(triggered()), SLOT(onSlotUpLoadProject()));

    // U盘
    m_pActionUDiskObj = new QAction(QIcon(":/images/downusb.png"), tr("U盘"));
    m_pActionUDiskObj->setObjectName(QString::fromUtf8("actionUDisk"));
    connect(m_pActionUDiskObj, SIGNAL(triggered()), SLOT(onSlotUDisk()));

    //-----------------------------<变量编辑菜单>----------------------------------
    // 添加变量
    m_pActionAddTagObj = new QAction(QIcon(":/images/data_add.png"), tr("添加变量"));
    m_pActionAddTagObj->setObjectName(QString::fromUtf8("actionAddTag"));
    connect(m_pActionAddTagObj, SIGNAL(triggered()), SLOT(onSlotAddTag()));

    // 追加变量
    m_pActionAppendTagObj = new QAction(QIcon(":/images/data_supadd.png"), tr("追加变量"));
    m_pActionAppendTagObj->setObjectName(QString::fromUtf8("actionAppendTag"));
    connect(m_pActionAppendTagObj, SIGNAL(triggered()), SLOT(onSlotAppendTag()));

    // 拷贝变量
    m_pActionRowCopyTagObj = new QAction(QIcon(":/images/data_rowcopy.png"), tr("拷贝变量"));
    m_pActionRowCopyTagObj->setObjectName(QString::fromUtf8("actionRowCopyTag"));
    connect(m_pActionRowCopyTagObj, SIGNAL(triggered()), SLOT(onSlotRowCopyTag()));

    // 修改变量
    m_pActionModifyTagObj = new QAction(QIcon(":/images/icon_modify.png"), tr("修改变量"));
    m_pActionModifyTagObj->setObjectName(QString::fromUtf8("actionModifyTag"));
    connect(m_pActionModifyTagObj, SIGNAL(triggered()), SLOT(onSlotModifyTag()));

    // 删除变量
    m_pActionDeleteTagObj = new QAction(QIcon(":/images/icon_delete.png"), tr("删除变量"));
    m_pActionDeleteTagObj->setObjectName(QString::fromUtf8("actionDeleteTag"));
    connect(m_pActionDeleteTagObj, SIGNAL(triggered()), SLOT(onSlotDeleteTag()));

    // 导出变量
    m_pActionExportTagObj = new QAction(QIcon(":/images/data_export.png"), tr("导出变量"));
    m_pActionExportTagObj->setObjectName(QString::fromUtf8("actionExportTag"));
    connect(m_pActionExportTagObj, SIGNAL(triggered()), SLOT(onSlotExportTag()));

    // 导入变量
    m_pActionImportTagObj = new QAction(QIcon(":/images/data_import.png"), tr("导入变量"));
    m_pActionImportTagObj->setObjectName(QString::fromUtf8("actionImportTag"));
    connect(m_pActionImportTagObj, SIGNAL(triggered()), SLOT(onSlotImportTag()));
}


/**
 * @brief MainWindow::createMenus
 * @details 创建菜单
 */
void MainWindow::createMenus()
{

    menu = new QMenu(this->menuBar());
    menu->setObjectName(QString::fromUtf8("menu"));
    this->setMenuBar(this->menuBar());

    // 工程菜单
    m_pMenuProjectObj = this->menuBar()->addMenu(tr("工程"));
    m_pMenuProjectObj->addAction(m_pActionNewProjObj);
    m_pMenuProjectObj->addAction(m_pActionOpenProjObj);
    m_pMenuProjectObj->addAction(m_pActionCloseProjObj);
    m_pMenuProjectObj->addAction(m_pActionSaveProjObj);
    m_pMenuProjectObj->addSeparator();
    m_pMenuProjectObj->addAction(m_pActionRecentProjListObj);
    m_pMenuProjectObj->addSeparator();
    m_pMenuProjectObj->addAction(m_pActionExitObj);

    // 视图菜单
    m_pMenuViewObj = this->menuBar()->addMenu(tr("视图"));
    m_pMenuViewObj->addAction(m_pActionToolBarObj);
    m_pMenuViewObj->addAction(m_pActionStatusBarObj);
    m_pMenuViewObj->addAction(m_pActionWorkSpaceObj);
    m_pMenuViewObj->addAction(m_pActionDisplayAreaObj);
    m_pMenuViewObj->addAction(m_pActionBigIconObj);
    m_pMenuViewObj->addAction(m_pActionSmallIconObj);
    m_pMenuViewObj->addAction(m_pActionEditObj);



    //-----------------------------<画面编辑器>----------------------------------
    QMenu *filemenu = this->menuBar()->addMenu(tr("画面"));
#if 0  // for test we need this
    filemenu->addAction(m_pActionNewGraphPageObj);
    filemenu->addAction(m_pActionOpenObj);
#endif
    filemenu->addAction(m_pActionSaveGraphPageObj);
    filemenu->addSeparator();
    filemenu->addAction(m_pActionCloseGraphPageObj); // 画面.关闭
    filemenu->addAction(m_pActionCloseAllObj); // 画面.关闭所有
    filemenu->addSeparator();


    QMenu *windowMenu = this->menuBar()->addMenu(tr("窗口"));
    windowMenu->addAction(m_pActionShowGraphObj);
    windowMenu->addAction(m_pActionShowPropEditorObj);

    //-----------------------------<工具菜单>----------------------------------
    m_pMenuToolsObj = this->menuBar()->addMenu(tr("工具"));
    m_pMenuToolsObj->addAction(m_pActionSimulateObj); // 模拟仿真
    m_pMenuToolsObj->addAction(m_pActionRunObj); // 运行工程
    m_pMenuToolsObj->addAction(m_pActionDownloadObj); // 下载工程
    m_pMenuToolsObj->addAction(m_pActionUpLoadObj); // 上传工程
    m_pMenuToolsObj->addAction(m_pActionUDiskObj); // U盘

    //-----------------------------<变量编辑菜单>----------------------------------
    m_pMenuTagEditObj = this->menuBar()->addMenu(tr("变量编辑"));
    m_pMenuTagEditObj->addAction(m_pActionAddTagObj); // 添加变量
    m_pMenuTagEditObj->addAction(m_pActionAppendTagObj); // 追加变量
    m_pMenuTagEditObj->addAction(m_pActionRowCopyTagObj); // 拷贝变量
    m_pMenuTagEditObj->addAction(m_pActionModifyTagObj); // 修改变量
    m_pMenuTagEditObj->addAction(m_pActionDeleteTagObj); // 删除变量
    m_pMenuTagEditObj->addAction(m_pActionExportTagObj); // 导出变量
    m_pMenuTagEditObj->addAction(m_pActionImportTagObj); // 导入变量


}


/**
 * @brief MainWindow::createToolbars
 * @details 创建工具条
 */
void MainWindow::createToolbars()
{
    //-----------------------------<工程工具栏>-----------------------------------
    m_pToolBarProjectObj = new QToolBar(this);
    m_pToolBarProjectObj->setObjectName(QString::fromUtf8("ProjectToolBar"));
    m_pToolBarProjectObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_pToolBarProjectObj->addAction(m_pActionNewProjObj);
    m_pToolBarProjectObj->addAction(m_pActionOpenProjObj);
    m_pToolBarProjectObj->addAction(m_pActionCloseProjObj);
    m_pToolBarProjectObj->addAction(m_pActionSaveProjObj);
    m_pToolBarProjectObj->addAction(m_pActionExitObj);

    //-----------------------------<视图工具栏>----------------------------------

    m_pToolBarView = new QToolBar(this);
    m_pToolBarView->setObjectName(QString::fromUtf8("ViewToolBar"));
    m_pToolBarView->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_pToolBarView->addAction(m_pActionBigIconObj);
    m_pToolBarView->addAction(m_pActionSmallIconObj);
    m_pToolBarView->addAction(m_pActionEditObj);

    //-----------------------------<画面编辑器>----------------------------------
    this->toolBar->addAction(m_pActionSaveGraphPageObj);
    this->toolBar->addSeparator();
    this->toolBar->addAction(m_pActionShowGridObj); // 显示栅格
    this->toolBar->addAction(m_pActionZoomOutObj); //画面缩小
    this->toolBar->addAction(m_pActionZoomInObj); // 画面放大
    this->toolBar->addSeparator();
    this->toolBar->addAction(m_pActionUndoObj); // 撤销
    this->toolBar->addAction(m_pActionRedoObj); // 重做
    this->toolBar->addSeparator();
    this->toolBar->addAction(m_pActionCopyObj); // 拷贝画面
    this->toolBar->addAction(m_pActionPasteObj); // 粘贴画面
    this->toolBar->addAction(m_pActionDeleteObj); // 删除画面
    this->toolBar->addSeparator();
    this->toolBar->addAction(m_pActionAlignTopObj); // 顶部对齐
    this->toolBar->addAction(m_pActionAlignDownObj); // 底部对齐
    this->toolBar->addAction(m_pActionAalignLeftObj); // 左对齐
    this->toolBar->addAction(m_pActionAlignRightObj); // 右对齐
    this->toolBar->addAction(m_pActionHUniformDistributeObj); // 水平均匀分布
    this->toolBar->addAction(m_pActionVUniformDistributeObj); // 垂直均匀分布
    this->toolBar->addAction(m_pActionSetTheSameSizeObj); // 设置选中控件大小一致
    this->toolBar->addAction(m_pActionUpLayerObj); // 上移一层
    this->toolBar->addAction(m_pActionDownLayerObj); // 下移一层
    this->toolBar->addSeparator();

    //-----------------------------<工具>----------------------------------

    m_pToolBarToolsObj = new QToolBar(this);
    m_pToolBarToolsObj->setObjectName(QString::fromUtf8("ToolBarTools"));
    m_pToolBarToolsObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_pToolBarToolsObj->addAction(m_pActionSimulateObj); // 模拟仿真
    m_pToolBarToolsObj->addAction(m_pActionRunObj); // 运行工程
    m_pToolBarToolsObj->addAction(m_pActionDownloadObj); // 下载工程
    m_pToolBarToolsObj->addAction(m_pActionUpLoadObj); // 上传工程
    m_pToolBarToolsObj->addAction(m_pActionUDiskObj); // U盘

    //-----------------------------<变量编辑>----------------------------------
    m_pToolBarTagEditObj = new QToolBar(this);
    m_pToolBarTagEditObj->setObjectName(QString::fromUtf8("TagOperateToolBar"));
    m_pToolBarTagEditObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_pToolBarTagEditObj->addAction(m_pActionAddTagObj);  // 添加变量
    m_pToolBarTagEditObj->addAction(m_pActionAppendTagObj); // 追加变量
    m_pToolBarTagEditObj->addAction(m_pActionRowCopyTagObj); // 拷贝变量
    m_pToolBarTagEditObj->addAction(m_pActionModifyTagObj); // 修改变量
    m_pToolBarTagEditObj->addAction(m_pActionDeleteTagObj); // 删除变量
    m_pToolBarTagEditObj->addAction(m_pActionExportTagObj); // 导出变量
    m_pToolBarTagEditObj->addAction(m_pActionImportTagObj); // 导入变量


    this->addToolBar(Qt::TopToolBarArea, m_pToolBarProjectObj);
    this->addToolBar(Qt::TopToolBarArea, m_pToolBarView);
    this->addToolBar(Qt::TopToolBarArea, m_pToolBarToolsObj);
    this->addToolBar(Qt::TopToolBarArea, m_pToolBarTagEditObj);
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
    return Q_NULLPTR;
}

void MainWindow::setActiveSubWindow(ChildForm *window)
{
    if (!window) return;
    window->showMaximized();
    m_szCurItem = window->windowTitle();
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
    return Q_NULLPTR;
}

QMdiSubWindow *MainWindow::findMdiSubWindow(const QString &windowTitle)
{
    foreach (QMdiSubWindow *window, this->mdiArea->subWindowList()) {
        ChildBase *pChildWin = qobject_cast<ChildBase *>(window->widget());
        if (pChildWin->windowTitle() == windowTitle) return window;
    }
    return Q_NULLPTR;
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
    ChildForm *window = findMdiChild(this->m_szCurItem);
    if (pDlg->exec() == QDialog::Accepted) {
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
        pObj->m_id = tagIOGroup.getGroupCount(ProjectData::getInstance()->dbData_) + 1;
        pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
        pObj->m_szShowName = pDlg->GetGroupName();
        tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
        UpdateDeviceVariableTableGroup();
        if(window != Q_NULLPTR) {
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
    ChildForm *window = findMdiChild(this->m_szCurItem);

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
    m_szCurTreeViewItem = this->pTreeViewProjectModel->itemFromIndex(index)->text();

    if (m_szCurTreeViewItem == "设备变量")
        found = true;

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    tagIOGroup.load(ProjectData::getInstance()->dbData_);

    foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
        if (m_szCurTreeViewItem == pObj->m_szShowName)
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
        ConfigUtils::setCfgStr(strFile, "PathInfo", "Path", m_szProjPath);
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

    this->m_pStatusBarObj->showMessage(tr("欢迎使用HmiFuncDesigner组态系统"));
}

/**
 * @brief MainWindow::onNewPoject
 * @details 新建工程
 */
void MainWindow::onNewPoject()
{
    if (pProjectItem->text() != tr("未创建工程")) {
        QMessageBox::information(
                    this, tr("系统提示"),
                    tr("工程文件已建立，请手动关闭当前工程文件后重新建立！"));
        return;
    }

    NewProjectDialog *pNewProjectDlg = new NewProjectDialog(this);
    if (pNewProjectDlg->exec() == QDialog::Accepted) {
        UpdateProjectName(pNewProjectDlg->GetProjectName());

        QString szPath = ProjectMgrUtils::getProjectPath(m_szProjName);
        QString szName = ProjectMgrUtils::getProjectNameWithOutSuffix(m_szProjName);
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
    ConfigUtils::setCfgStr(strFile, "PathInfo", "Path", m_szProjPath);
    QString szPath = ProjectMgrUtils::getProjectPath(m_szProjName);
    QString szName = ProjectMgrUtils::getProjectNameWithOutSuffix(m_szProjName);
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

/**
 * @brief MainWindow::onOpenProject
 * @details 打开工程
 */
void MainWindow::onOpenProject()
{
    QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
    QString path = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "C:/");
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("选择工程文件"), path, tr("project file (*.pdt)"));
    if(fileName != Q_NULLPTR) {
        doOpenProject(fileName);
    }
}

void MainWindow::on_actionWorkSpace_triggered(bool checked)
{
    this->m_pDockProjectMgrObj->setVisible(checked);
}


/**
 * @brief MainWindow::enableToolBar
 * @param text
 * @details 工具条使能
 */
void MainWindow::enableToolBar(QString text)
{
    bool bTagIOOrTmp = (text == tr("中间变量")) | (text.startsWith(tr("设备变量")));
    this->m_pToolBarTagEditObj->setEnabled(bTagIOOrTmp);

    bool bdevice = (text == tr("串口设备")) | (text == tr("网络设备")) |
            (text == tr("总线设备")) | (text == tr("OPC设备"));
    this->DeviceOperateToolBar->setEnabled(bdevice);
}

void MainWindow::onTreeViewProjectClicked(const QString &szItemText)
{
    QString winTittle = szItemText;

    ChildForm *findForm = findMdiChild(m_szProjName);
    if(findForm == Q_NULLPTR) {
        findForm = new ChildForm(this, m_szProjName);
        findForm->setWindowTitle(m_szProjName);
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
    if(m_szProjName == "")
        return;

    QStandardItem *item = pTreeViewProjectModel->itemFromIndex(index);
    onTreeViewProjectClicked(item->text());
}

void MainWindow::UpdateProjectName(QString name)
{
    if(!name.isEmpty()) {
        m_szProjName = name;
        m_szProjPath = ProjectMgrUtils::getProjectPath(name);
        QString strName = name.mid(name.lastIndexOf("/") + 1, name.indexOf(".") - name.lastIndexOf("/") - 1);
        pProjectItem->setText(strName);
        this->m_pActionRunObj->setEnabled(true);
    } else {
        m_szProjName = "";
        m_szProjPath = "";
        this->m_pActionRunObj->setEnabled(false);
        pTreeViewProjectModel->clear();
        this->treeViewProject->reset();
        delete pTreeViewProjectModel;
        pTreeViewProjectModel = Q_NULLPTR;
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


/**
 * @brief MainWindow::onSaveProject
 * @details 保存工程
 */
void MainWindow::onSaveProject()
{
    foreach (QMdiSubWindow* window, this->mdiArea->subWindowList()) {
        ChildForm *findForm = qobject_cast<ChildForm *>(window->widget());
        if(findForm != Q_NULLPTR)
            findForm->save();
    }
}


/**
 * @brief MainWindow::onCloseProject
 * @details 关闭工程
 */
void MainWindow::onCloseProject()
{
    if(pProjectItem->text() == "未创建工程")
        return;
    foreach (QMdiSubWindow* window, this->mdiArea->subWindowList()) {
        ChildForm * findForm = qobject_cast<ChildForm *>(window->widget());
        if(findForm != Q_NULLPTR)
            findForm->save();
        window->close();
    }
    UpdateProjectName(QString());
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

void MainWindow::on_treeViewProject_activated(const QModelIndex & /*index*/) {}


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
        argv << m_szProjPath;
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

    ProjectUploadDialog *pDlg = new ProjectUploadDialog(this, m_szProjName);
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
 * @brief MainWindow::onSlotUDisk
 * @details U盘
 */
void MainWindow::onSlotUDisk()
{

}

/**
 * @brief MainWindow::onSlotDownloadProject
 * @details 下载工程
 */
void MainWindow::onSlotDownloadProject()
{
    if(m_szProjName == Q_NULLPTR)
        return;

    // 创建tmp目录
    QString tmpDir = QCoreApplication::applicationDirPath() + "/tmp";
    QDir dir(tmpDir);
    if(!dir.exists()) {
        dir.mkpath(tmpDir);
    }

    // 拷贝工程到tmp目录
    QString desDir = QCoreApplication::applicationDirPath() + "/tmp/RunProject";
    Helper::CopyRecursively(m_szProjPath, desDir);

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

    ProjectDownloadDialog *pDlg = new ProjectDownloadDialog(this, m_szProjName);
    pDlg->setProjFileName(QCoreApplication::applicationDirPath() + "/tmp/RunProject.tar");
    if (pDlg->exec() == QDialog::Accepted) {
    }
    delete pDlg;
}


/**
 * @brief MainWindow::onSlotAddTag
 * @details 添加变量
 */
void MainWindow::onSlotAddTag()
{
    ChildForm *window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->addVariableTag();
    }
}


/**
 * @brief MainWindow::onSlotAppendTag
 * @details 追加变量
 */
void MainWindow::onSlotAppendTag()
{
    ChildForm *window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->appendVariableTag();
    }
}


/**
 * @brief MainWindow::onSlotRowCopyTag
 * @details 拷贝变量
 */
void MainWindow::onSlotRowCopyTag()
{
    ChildForm *window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->rowCopyVariableTag();
    }
}



/**
 * @brief MainWindow::onSlotModifyTag
 * @details 修改变量
 */
void MainWindow::onSlotModifyTag()
{
    ChildForm *window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->modifyVariableTag();
    }
}

/**
 * @brief MainWindow::onSlotDeleteTag
 * @details 删除变量
 */
void MainWindow::onSlotDeleteTag()
{
    ChildForm *window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->deleteVariableTag();
    }
}


/**
 * @brief MainWindow::onSlotExportTag
 * @details 导出变量
 */
void MainWindow::onSlotExportTag()
{
    QString dirUploadProjects = QCoreApplication::applicationDirPath();
    QString strSaveCsvPath = QFileDialog::getExistingDirectory(
                this, tr("选择导出csv路径"), dirUploadProjects,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (strSaveCsvPath == "") return;

    ChildForm *window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->variableTagExportToCsv(strSaveCsvPath);
    }
}


/**
 * @brief MainWindow::onSlotImportTag
 * @details 导入变量
 */
void MainWindow::onSlotImportTag()
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
        ChildForm* pFindForm = findMdiChild(this->m_szCurItem);
        if(pFindForm != Q_NULLPTR) {
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

            ChildForm* pFindForm = findMdiChild(this->m_szCurItem);
            if(pFindForm != Q_NULLPTR) {
                pFindForm->variableTagImportFromCsv(strSaveCsvFile);
            }
            return;
        }

        ChildForm* pFindForm = findMdiChild(this->m_szCurItem);
        if(pFindForm != Q_NULLPTR) {
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
    ChildForm* window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->newDevice();
    }
}

/*
 * 插槽：修改设备
 */
void MainWindow::on_actionDeviceModify_triggered()
{
    ChildForm* window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
        window->modifyDevice();
    }
}

/*
 * 插槽：删除设备
 */
void MainWindow::on_actionDeviceDelete_triggered()
{
    ChildForm* window = findMdiChild(this->m_szCurItem);
    if(window != Q_NULLPTR) {
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
            this->m_pMenuProjectObj->insertAction(this->m_pActionRecentProjListObj, pAct);
        }
        this->m_pMenuProjectObj->removeAction(this->m_pActionRecentProjListObj);
        return;
    }

    QList<QAction *> listActRemove;
    QList<QAction *> listAct = this->m_pMenuProjectObj->actions();
    for (int i = 0; i<listAct.size(); ++i) {
        QAction *pAct = listAct.at(i);
        if(pAct->isSeparator())
            listActRemove.append(pAct);
        if(pAct->text() == tr("最近文件列表"))
            listActRemove.append(pAct);
    }
    for (int j = 0; j<listActRemove.size(); ++j) {
        this->m_pMenuProjectObj->removeAction(listActRemove.at(j));
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


/**
 * @brief MainWindow::onBigIcon
 * @details 显示大图标
 */
void MainWindow::onBigIcon()
{
    this->m_pActionBigIconObj->setChecked(true);
    this->m_pActionSmallIconObj->setChecked(false);
    ChildForm *findForm = findMdiChild(this->m_szCurItem);
    if(findForm != Q_NULLPTR) {
        findForm->showLargeIcon();
    }
}

/**
 * @brief MainWindow::onSmallIcon
 * @details 显示小图标
 */
void MainWindow::onSmallIcon()
{
    this->m_pActionBigIconObj->setChecked(false);
    this->m_pActionSmallIconObj->setChecked(true);
    ChildForm *findForm = findMdiChild(this->m_szCurItem);
    if(findForm != Q_NULLPTR) {
        findForm->showSmallIcon();
    }
}


//------------------------------------------------------------------------------

void MainWindow::initView()
{
    m_pGraphPageTabWidgetObj = new QTabWidget(this);
    m_pGraphPageTabWidgetObj->installEventFilter(this);
    this->scrollArea->setWidget(m_pGraphPageTabWidgetObj);

    m_pElementWidgetObj = new ElementLibraryWidget();
    this->ElemetsLayout->addWidget(m_pElementWidgetObj);

    m_pVariantEditorFactoryObj = new VariantFactory(this);

    //propertyEditor_ = new QtTreePropertyBrowser(dockProperty);
    m_pPropertyEditorObj = new QtTreePropertyBrowser(this);
    m_pPropertyEditorObj->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pPropertyEditorObj->setHeaderLabels(QStringList() << tr("属性") << tr("值"));
    //propertyEditor_->setColumnWidth(0, 60);
    //propertyEditor_->setColumnWidth(1, 200);


    this->PropertyLayout->addWidget(m_pPropertyEditorObj);

    VariantManager *pVariantManager  = new VariantManager(this);
    m_pVariantPropertyMgrObj = pVariantManager;
    pVariantManager->setPropertyEditor(m_pPropertyEditorObj);
    m_pPropertyEditorObj->setFactoryForManager(m_pVariantPropertyMgrObj, m_pVariantEditorFactoryObj);

    connect(m_pVariantPropertyMgrObj, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
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

            if (m_pGraphPageTabWidgetObj->indexOf(view) != -1) {
                delete view;
                return;
            }

            GraphPage *graphPage = new GraphPage(QRectF(), m_pVariantPropertyMgrObj, m_pPropertyEditorObj);
            if (!createDocument(graphPage, view, fileName)) {
                return;
            }

            if(szPageId == szPageName) {
                m_pCurrentGraphPageObj = graphPage;
                m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(view);
            }

            graphPage->setProjectPath(szProjPath);
            graphPage->setProjectName(szProjName);
            graphPage->setGridVisible(m_bGraphPageGridVisible);
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
    QString title = m_pCurrentGraphPageObj->getGraphPageId();

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

        for (int j = 0; j < m_pGraphPageTabWidgetObj->count(); ++j) {
            const QWidget *widget = m_pGraphPageTabWidgetObj->widget(j);

            if (widget == viewGraphPage) {
                continue;
            }

            if (result == m_pGraphPageTabWidgetObj->tabText(j)) {
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

    if (m_pGraphPageTabWidgetObj->indexOf(view) != -1) {
        delete view;
        return;
    }

    GraphPage *graphPage = new GraphPage(QRectF(), m_pVariantPropertyMgrObj, m_pPropertyEditorObj);
    graphPage->setProjectPath(m_szProjPath);
    graphPage->setProjectName(m_szProjName);
    graphPage->setGridVisible(m_bGraphPageGridVisible);
    m_pCurrentGraphPageObj = graphPage;
    view->setScene(graphPage);
    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(view);
    QString title = fixedWindowTitle(view);
    graphPage->setFileName(title + ".drw");
    graphPage->setGraphPageId(title);
    m_pGraphPageTabWidgetObj->addTab(m_pCurrentViewObj, title);
    m_pGraphPageTabWidgetObj->setCurrentWidget(m_pCurrentViewObj);
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    m_pUndoGroupObj->addStack(graphPage->undoStack());
    m_pUndoGroupObj->setActiveStack(graphPage->undoStack());

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

    for (int i = 0; i < m_pGraphPageTabWidgetObj->count(); i++) {
        QGraphicsView *view = dynamic_cast<QGraphicsView*>(m_pGraphPageTabWidgetObj->widget(i));
        if (!dynamic_cast<GraphPage *>(view->scene())->undoStack()->isClean() ||
            dynamic_cast<GraphPage *>(view->scene())->getUnsavedFlag()) {
            m_pGraphPageTabWidgetObj->setTabIcon(m_pGraphPageTabWidgetObj->indexOf(view), unsaved);
        } else {
            m_pGraphPageTabWidgetObj->setTabIcon(m_pGraphPageTabWidgetObj->indexOf(view), QIcon());
        }
    }

    m_pActionZoomInObj->setEnabled(m_pGraphPageTabWidgetObj->count() ? true : false);
    m_pActionZoomOutObj->setEnabled(m_pGraphPageTabWidgetObj->count() ? true : false);
    m_pActionShowGridObj->setEnabled(m_pGraphPageTabWidgetObj->count() ? true : false);

    if (!m_pCurrentGraphPageObj) {
        return;
    }

    m_pUndoGroupObj->setActiveStack(m_pCurrentGraphPageObj->undoStack());

    if (!m_pCurrentGraphPageObj->undoStack()->isClean() || m_pCurrentGraphPageObj->getUnsavedFlag()) {
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

    for (int i = 0; i < m_pGraphPageTabWidgetObj->count(); i++) {
        QGraphicsView *view = dynamic_cast<QGraphicsView *>(m_pGraphPageTabWidgetObj->widget(i));
        dynamic_cast<GraphPage *>(view->scene())->setActive(false);
    }

    m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(m_pGraphPageTabWidgetObj->widget(GraphPageNum));
    m_pCurrentGraphPageObj = dynamic_cast<GraphPage *>(m_pCurrentViewObj->scene());
    m_pCurrentGraphPageObj->setActive(true);
    //currentGraphPage_->fillGraphPagePropertyModel();
    m_iCurrentGraphPageIndex = GraphPageNum;
    slotUpdateActions();
}

void MainWindow::slotChangeGraphPageName()
{
    m_pGraphPageTabWidgetObj->setTabText(m_iCurrentGraphPageIndex, m_pCurrentGraphPageObj->getGraphPageId());
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

    m_bGraphPageGridVisible = on;
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


/**
 * @brief MainWindow::onSlotCloseAll
 * @details 关闭所有画面
 */
void MainWindow::onSlotCloseAll()
{
    while (m_pGraphPageTabWidgetObj->count()) {
        QGraphicsView *view = static_cast<QGraphicsView*>(m_pGraphPageTabWidgetObj->widget(m_pGraphPageTabWidgetObj->currentIndex()));
        removeGraphPage(view);
        delete view;
    }

    m_pCurrentViewObj = Q_NULLPTR;
    m_pCurrentGraphPageObj = Q_NULLPTR;
    slotUpdateActions();
}

void MainWindow::removeGraphPage(QGraphicsView *view)
{
    int index = m_pGraphPageTabWidgetObj->indexOf(view);
    GraphPage *graphPage = static_cast<GraphPage*>(view->scene());

    if (index == -1)
        return;

    if (!graphPage->undoStack()->isClean()) {
        int ret = exitResponse();

        if (ret == QMessageBox::Yes) {
            onSlotSaveGraphPage();
        }
    }

    m_pGraphPageTabWidgetObj->removeTab(index);
    m_pUndoGroupObj->removeStack(graphPage->undoStack());
    GraphPageManager::getInstance()->removeGraphPage(graphPage);
    disconnectGraphPage(graphPage);
    delete graphPage;
}


/**
 * @brief MainWindow::onSlotCloseGraphPage
 * @details 关闭画面
 */
void MainWindow::onSlotCloseGraphPage()
{
    QGraphicsView *view = m_pCurrentViewObj;
    removeGraphPage(view);
    delete view;

    if (m_pGraphPageTabWidgetObj->count() == 0) {
        m_pCurrentGraphPageObj = Q_NULLPTR;
        m_pCurrentViewObj = Q_NULLPTR;
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

        if (m_pGraphPageTabWidgetObj->indexOf(view) != -1) {
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

        if (m_pGraphPageTabWidgetObj->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF(), m_pVariantPropertyMgrObj, m_pPropertyEditorObj);
        if (!createDocument(graphPage, view, filename)) {
            return;
        }

        m_pCurrentGraphPageObj = graphPage;
        m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(view);
        graphPage->setProjectPath(m_szProjPath);
        graphPage->setProjectName(m_szProjName);
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

    graphPage->setGridVisible(m_bGraphPageGridVisible);
    view->setScene(graphPage);
    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    m_pGraphPageTabWidgetObj->addTab(view, graphPage->getGraphPageId());
    m_pGraphPageTabWidgetObj->setCurrentWidget(view);
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    m_pUndoGroupObj->addStack(graphPage->undoStack());
    m_pUndoGroupObj->setActiveStack(graphPage->undoStack());

    connectGraphPage(graphPage);

    graphPage->undoStack()->setClean();

    return true;
}


QString MainWindow::getFileName()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save as"),
                                                    QString("./%1").arg(m_pCurrentGraphPageObj->getGraphPageId()),
                                                    tr("GraphPage(*.drw)"));
    return filename;
}

void MainWindow::updateGraphPageViewInfo(const QString &fileName)
{
    int index = m_pGraphPageTabWidgetObj->indexOf(m_pCurrentViewObj);
    QFileInfo file(fileName);
    m_pCurrentGraphPageObj->setGraphPageId(file.baseName());
    m_pGraphPageTabWidgetObj->setTabText(index,file.baseName());
    slotChangeGraphPageName();
}

/**
 * @brief MainWindow::onSlotEditOpen
 * @details [画面.保存] 动作响应函数
 */
void MainWindow::onSlotSaveGraphPage()
{
    if (!m_pCurrentGraphPageObj) {
        return;
    }

    for (;;) {
        QString fileName = m_pCurrentGraphPageObj->getFileName();

        if (fileName.isEmpty())
            fileName = getFileName();

        if (fileName.isEmpty())
            break;

        m_pCurrentGraphPageObj->setFileName(fileName);
        updateGraphPageViewInfo(fileName);
        m_pCurrentGraphPageObj->saveAsXML(m_szProjPath + "/" + fileName);
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

/**
 * @brief MainWindow::onSlotZoomIn
 * @details 画面放大
 */
void MainWindow::onSlotZoomIn()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        int width = m_pCurrentGraphPageObj->getGraphPageWidth();
        int height = m_pCurrentGraphPageObj->getGraphPageHeight();
        m_pCurrentGraphPageObj->setGraphPageWidth(static_cast<int>(width * 1.25));
        m_pCurrentGraphPageObj->setGraphPageHeight(static_cast<int>(height * 1.25));
        m_pCurrentGraphPageObj->setGridVisible(m_pCurrentGraphPageObj->isGridVisible());
    }
    if (m_pCurrentViewObj != Q_NULLPTR) {
        m_pCurrentViewObj->scale(1.25, 1.25);
        m_pCurrentViewObj->setFixedSize(m_pCurrentGraphPageObj->getGraphPageWidth(), m_pCurrentGraphPageObj->getGraphPageHeight());
    }
}


/**
 * @brief MainWindow::onSlotZoomOut
 * @details 画面缩小
 */
void MainWindow::onSlotZoomOut()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        int width = m_pCurrentGraphPageObj->getGraphPageWidth();
        int height = m_pCurrentGraphPageObj->getGraphPageHeight();
        m_pCurrentGraphPageObj->setGraphPageWidth(static_cast<int>(width * 1/1.25));
        m_pCurrentGraphPageObj->setGraphPageHeight(static_cast<int>(height * 1/1.25));
        m_pCurrentGraphPageObj->setGridVisible(m_pCurrentGraphPageObj->isGridVisible());
    }
    if (m_pCurrentViewObj != Q_NULLPTR) {
        m_pCurrentViewObj->scale(1/1.25, 1/1.25);
        m_pCurrentViewObj->setFixedSize(m_pCurrentGraphPageObj->getGraphPageWidth(), m_pCurrentGraphPageObj->getGraphPageHeight());
    }
}


/**
 * @brief MainWindow::onSlotAlignElements
 * @details 顶部对齐, 底部对齐, 右对齐, 左对齐
 */
void MainWindow::onSlotAlignElements()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    Qt::Alignment alignment = static_cast<Qt::Alignment>(action->data().toInt());
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onAlignElements(alignment, items);
    }
}


/**
 * @brief MainWindow::onSlotHUniformDistributeElements
 * @details 水平均匀分布
 */
void MainWindow::onSlotHUniformDistributeElements()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onHUniformDistributeElements(items);
    }
}


/**
 * @brief MainWindow::onSlotVUniformDistributeElements
 * @details 垂直均匀分布
 */
void MainWindow::onSlotVUniformDistributeElements()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onVUniformDistributeElements(items);
    }
}


/**
 * @brief MainWindow::onSlotSetTheSameSizeElements
 * @details 设置选中控件大小一致
 */
void MainWindow::onSlotSetTheSameSizeElements()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onSetTheSameSizeElements(items);
    }
}


/**
 * @brief MainWindow::onSlotUpLayerElements
 * @details 上移一层
 */
void MainWindow::onSlotUpLayerElements()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onUpLayerElements(items);
    }
}


/**
 * @brief MainWindow::onSlotDownLayerElements
 * @details 下移一层
 */
void MainWindow::onSlotDownLayerElements()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onDownLayerElements(items);
    }
}


/**
 * @brief MainWindow::slotEditDelete
 * @details 删除画面
 */
void MainWindow::onSlotEditDelete()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onEditDelete(items);
    }
}


/**
 * @brief MainWindow::slotEditCopy
 * @details 拷贝画面
 */
void MainWindow::onSlotEditCopy()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onEditCopy(items);
    }
}


/**
 * @brief MainWindow::slotEditPaste
 * @details 粘贴画面
 */
void MainWindow::onSlotEditPaste()
{
    if(m_pCurrentGraphPageObj != Q_NULLPTR) {
        QList<QGraphicsItem*> items = m_pCurrentGraphPageObj->selectedItems();
        m_pCurrentGraphPageObj->onEditPaste();
    }
}


/**
 * @brief MainWindow::onSlotListWidgetGraphPagesCurrentTextChanged
 * @details 画面名称被单击
 * @param item
 */
void MainWindow::onSlotListWidgetGraphPagesCurrentTextChanged(const QString &currentText)
{
    Q_UNUSED(currentText)
    m_pGraphPageTabWidgetObj->setCurrentIndex(this->listWidgetGraphPages->currentRow());
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

        createEmptyGraphpage(m_szProjPath, szGraphPageName, width, height);
        DrawListUtils::drawList_.append(szGraphPageName);
        DrawListUtils::saveDrawList(m_szProjPath);

        this->listWidgetGraphPages->addItem(szGraphPageName);
        QString fileName = m_szProjPath + "/" + szGraphPageName + ".drw";

        if (fileName.toLower().endsWith(".drw")) {
            QGraphicsView *view = createTabView();

            if (m_pGraphPageTabWidgetObj->indexOf(view) != -1) {
                delete view;
                return;
            }

            GraphPage *graphPage = new GraphPage(QRectF(), m_pVariantPropertyMgrObj, m_pPropertyEditorObj);
            if (!createDocument(graphPage, view, fileName)) {
                return;
            }

            m_pCurrentGraphPageObj = graphPage;
            m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(view);
            graphPage->setProjectPath(m_szProjPath);
            graphPage->setProjectName(m_szProjName);
            graphPage->loadAsXML(fileName);
            view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
            graphPage->setFileName(szGraphPageName + ".drw");
            graphPage->setGraphPageId(szGraphPageName);
        }

        QList<QListWidgetItem*> listWidgetItem = this->listWidgetGraphPages->findItems(szGraphPageName, Qt::MatchCaseSensitive);
        if ( listWidgetItem.size() > 0 ) {
            this->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
            m_pGraphPageTabWidgetObj->setCurrentIndex(this->listWidgetGraphPages->currentRow());
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
                QString szOldName = m_szProjPath + "/" + szOldGraphPageName + ".drw";
                QString szNewName = m_szProjPath + "/" + szNewGraphPageName + ".drw";
                QFile::rename(szOldName, szNewName);
                DrawListUtils::saveDrawList(m_szProjPath);
                this->listWidgetGraphPages->currentItem()->setText(szNewGraphPageName);
                GraphPage *pGraphPage = GraphPageManager::getInstance()->getGraphPageById(szOldGraphPageName);
                pGraphPage->setFileName(szNewGraphPageName + ".drw");
                pGraphPage->setGraphPageId(szNewGraphPageName);
                m_pGraphPageTabWidgetObj->setTabText(m_pGraphPageTabWidgetObj->currentIndex(), szNewGraphPageName);
                m_pCurrentGraphPageObj->setUnsavedFlag(true);
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

            QString fileName = m_szProjPath + "/" + szGraphPageName + ".drw";
            QFile file(fileName);
            if (file.exists()) {
                file.remove();
            }

            m_pGraphPageTabWidgetObj->removeTab(this->listWidgetGraphPages->currentRow());

            GraphPage *pGraphPageObj = GraphPageManager::getInstance()->getGraphPageById(szGraphPageName);
            if ( pGraphPageObj != Q_NULLPTR ) {
                GraphPageManager::getInstance()->removeGraphPage(pGraphPageObj);
                delete pGraphPageObj;
                pGraphPageObj = Q_NULLPTR;
            }

            DrawListUtils::saveDrawList(m_szProjPath);

            this->listWidgetGraphPages->clear();
            foreach(QString szPageId, DrawListUtils::drawList_) {
                this->listWidgetGraphPages->addItem(szPageId);
            }

            if (this->listWidgetGraphPages->count() > 0) {
                this->listWidgetGraphPages->setCurrentRow(0);
                m_pGraphPageTabWidgetObj->setCurrentIndex(0);
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
    m_szCopyGraphPageFileName = this->listWidgetGraphPages->currentItem()->text();
}


/**
 * @brief MainWindow::onPasteGraphPage
 * @details 粘贴画面
 */
void MainWindow::onPasteGraphPage()
{
    int iLast = 0;

reGetNum:
    iLast = DrawListUtils::getMaxDrawPageNum(m_szCopyGraphPageFileName);
    QString strDrawPageName = m_szCopyGraphPageFileName + QString("-%1").arg(iLast);
    if ( DrawListUtils::drawList_.contains(strDrawPageName )) {
        m_szCopyGraphPageFileName = strDrawPageName;
        goto reGetNum;
    }

    this->listWidgetGraphPages->addItem(strDrawPageName);
    DrawListUtils::drawList_.append(strDrawPageName);
    DrawListUtils::saveDrawList(m_szProjPath);
    QString szFileName = m_szProjPath + "/" + m_szCopyGraphPageFileName + ".drw";
    QFile file(szFileName);
    QString szPasteFileName = m_szProjPath + "/" + strDrawPageName + ".drw";
    file.copy(szPasteFileName);

    if (szPasteFileName.toLower().endsWith(".drw")) {
        QGraphicsView *view = createTabView();

        if (m_pGraphPageTabWidgetObj->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF(), m_pVariantPropertyMgrObj, m_pPropertyEditorObj);
        if (!createDocument(graphPage, view, szPasteFileName)) {
            return;
        }

        m_pCurrentGraphPageObj = graphPage;
        m_pCurrentViewObj = dynamic_cast<QGraphicsView *>(view);
        graphPage->setProjectPath(m_szProjPath);
        graphPage->setProjectName(m_szProjName);
        graphPage->loadAsXML(szPasteFileName);
        view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
        graphPage->setFileName(strDrawPageName + ".drw");
        graphPage->setGraphPageId(strDrawPageName);
    }

    QList<QListWidgetItem*> listWidgetItem = this->listWidgetGraphPages->findItems(strDrawPageName, Qt::MatchCaseSensitive);
    if ( listWidgetItem.size() > 0 ) {
        this->listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
        m_pGraphPageTabWidgetObj->setCurrentIndex(this->listWidgetGraphPages->currentRow());
    }

    m_pCurrentGraphPageObj->setUnsavedFlag(true);
    slotUpdateActions();
}
