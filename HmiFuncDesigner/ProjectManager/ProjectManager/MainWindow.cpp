#include "MainWindow.h"
#include <QApplication>
#include <QCloseEvent>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QScreen>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
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
#include "ProjectDownloadDialog.h"
#include "ProjectUploadDialog.h"
#include "MainWindow.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "qsoftcore.h"
#include "VerifyPasswordDialog.h"
#include "../../libs/core/manhattanstyle.h"
#include "../../libs/core/newprojectdialog.h"
#include "../../libs/shared/pluginloader.h"
#include "../../libs/core/qabstractpage.h"
#include "../../libs/core/qsoftcore.h"
#include "../../libs/shared/qprojectcore.h"
#include "../../libs/shared/host/qabstracthost.h"
#include "../Public/userevent.h"
#include "devicepluginloader.h"
#include "../../../Devices/IDevicePlugin/IDevicePlugin.h"
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
    qApp->installEventFilter(this);

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
    QRect rect = QGuiApplication::primaryScreen()->geometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    this->resize(screenWidth * 3 / 4, screenHeight * 3 / 4);
    Helper::WidgetMoveCenter(this);
    setWindowTitle(tr("HmiFuncDesigner组态软件"));
    this->m_pStatusBarObj->showMessage(tr("欢迎使用HmiFuncDesigner组态软件"));
    connect(m_pTabProjectMgrObj, SIGNAL(currentChanged(int)), SLOT(onSlotTabProjectMgrCurChanged(int)));
    onSlotTabProjectMgrCurChanged(0);
}

MainWindow::~MainWindow()
{
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
    QAction *pActObj = NULL;

    // 新建工程
    pActObj = new QAction(QIcon(":/images/newproject.png"), tr("新建"), NULL);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+N"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onNewPoject);
        QSoftCore::getCore()->insertAction("Project.New", pActObj);
    }

    //  打开工程
    pActObj = new QAction(QIcon(":/images/openproject.png"), tr("打开"), NULL);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+O"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onOpenProject);
        QSoftCore::getCore()->insertAction("Project.Open", pActObj);
    }

    // 关闭工程
    pActObj = new QAction(QIcon(":/images/projectexit.png"), tr("关闭"), NULL);
    if(pActObj) {
        connect(pActObj, &QAction::triggered, this, &MainWindow::onCloseProject);
        QSoftCore::getCore()->insertAction("Project.Close", pActObj);
    }

    // 保存工程
    pActObj = new QAction(QIcon(":/images/saveproject.png"), tr("保存"), NULL);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+S"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onSaveProject);
        QSoftCore::getCore()->insertAction("Project.Save", pActObj);
    }

    // 设置打开工程的密码
    pActObj = new QAction(tr("设置打开工程的密码"), NULL);
    if(pActObj) {
        connect(pActObj, &QAction::triggered, this, &MainWindow::onSetOpenProjPassword);
        QSoftCore::getCore()->insertAction("Project.OpenPassword", pActObj);
        pActObj->setEnabled(false);
    }

    // 最近打开的工程
    pActObj = new QAction(tr("最近打开的工程"), NULL);
    if(pActObj) {
        QSoftCore::getCore()->insertAction("Project.LastOpen", pActObj);
        QJsonObject datJson;
        datJson.insert("which", QJsonValue("LastOpen"));
        QVariant datUser = QVariant(datJson);
        pActObj->setData(datUser);
    }

    // 退出
    pActObj = new QAction(QIcon(":/images/programexit.png"), tr("退出"), NULL);
    if(pActObj) {
        pActObj->setShortcut(QString("Ctrl+Q"));
        connect(pActObj, &QAction::triggered, this, &MainWindow::onExit);
        QSoftCore::getCore()->insertAction("Project.Exit", pActObj);
    }

    //-----------------------------<视图>---------------------------------------

    // 视图工具栏
    pActObj = new QAction(tr("视图"), NULL);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.Toolbar", pActObj);
    }

    // 状态栏
    pActObj = new QAction(tr("状态栏"), NULL);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.StatusBar", pActObj);
    }

    // 工作区
    pActObj = new QAction(tr("工作区"), NULL);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.WorkSpace", pActObj);
    }

    // 显示区
    pActObj = new QAction(tr("显示区"), NULL);
    if(pActObj) {
        pActObj->setCheckable(true);
        QSoftCore::getCore()->insertAction("Widget.DisplayArea", pActObj);
    }

    //-----------------------------<工具菜单>----------------------------------
    // 模拟仿真
    pActObj = new QAction(QIcon(":/images/offline.png"), tr("模拟仿真"));
    if(pActObj) {
        pActObj->setEnabled(false);
        pActObj->setVisible(false);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotSimulate()));
        QSoftCore::getCore()->insertAction("Tools.Simulate", pActObj);
    }

    // 运行工程
    pActObj = new QAction(QIcon(":/images/online.png"), tr("运行"));
    if(pActObj) {
        pActObj->setEnabled(true);
        pActObj->setVisible(false);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotRunProject()));
        QSoftCore::getCore()->insertAction("Tools.Run", pActObj);
    }

    // 停止运行工程
    pActObj = new QAction(QIcon(":/images/offline.png"), tr("停止"));
    if(pActObj) {
        pActObj->setEnabled(false);
        pActObj->setVisible(false);
        connect(pActObj, SIGNAL(triggered()), SLOT(onSlotStopRunProject()));
        QSoftCore::getCore()->insertAction("Tools.StopRun", pActObj);
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

    //-----------------------------<工具菜单>----------------------------------
    m_pMenuToolsObj = this->menuBar()->addMenu(tr("工具"));
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.Simulate")); // 模拟仿真
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.Run")); // 运行工程
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.StopRun")); // 停止运行工程
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.Download")); // 下载工程
    m_pMenuToolsObj->addAction(QSoftCore::getCore()->getAction("Tools.UpLoad")); // 上传工程

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

    pToolBarObj->addSeparator();

    //-----------------------------<工具>----------------------------------
    pToolBarObj = new QToolBar(this);
    QSoftCore::getCore()->insertToolBar("ToolBar.Tools", pToolBarObj);
    pToolBarObj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.Simulate")); // 模拟仿真
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.Run")); // 运行工程
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.StopRun")); // 停止运行工程
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.Download")); // 下载工程
    pToolBarObj->addAction(QSoftCore::getCore()->getAction("Tools.UpLoad")); // 上传工程

    //--------------------------------------------------------------------------

    this->addToolBar(Qt::TopToolBarArea, QSoftCore::getCore()->getToolBar("ToolBar.Project"));
    this->addToolBar(Qt::TopToolBarArea, QSoftCore::getCore()->getToolBar("ToolBar.Tools"));
    //addToolBarBreak();
}


/**
    * @brief MainWindow::closeEvent  关闭事件
    * @param event
    */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QString strFile = Helper::AppDir() + "/lastpath.ini";
    if (this->m_pProjectTreeViewObj->getProjectName() != tr("未创建工程")) {
        ConfigUtils::setCfgStr(strFile, "PathInfo", "Path", QSoftCore::getCore()->getProjectCore()->m_szProjPath);
    }

    writeSettings();
    event->accept();
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
        if(xml.load(szTags, NULL)) {
            QSoftCore::getCore()->getProjectCore()->m_tagMgr.openFromXml(&xml);
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

    updateRecentProjectList(proj);

    QAction *pActObj = QSoftCore::getCore()->getAction("Project.OpenPassword");
    if(pActObj) {
        pActObj->setEnabled(true);
    }

    if(m_pTabProjectMgrObj) {
        m_pTabProjectMgrObj->setCurrentIndex(0);
    }
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
    if(fileName != NULL) {
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
    if(QSoftCore::getCore()->getProjectCore()->m_szProjFile == "") {
        return;
    }

    QStringList szListUserData = szItemText.split("|");
    //qDebug() <<__FILE__ << __LINE__ <<__FUNCTION__ << "szListUserData:" << szListUserData;
    if(szListUserData.size() < 3) {
        return;
    }

    QAbstractPage* pPageObj = m_mapNameToPage.value(szListUserData.at(0).toUpper());
    if(pPageObj) {
        QWidget *pCurWidgetObj = m_pCentralWidgetObj->currentWidget();
        if(pCurWidgetObj) {
            UserEvent evHide(UserEvent::EVT_USER_HIDE_UPDATE, szListUserData.at(2).toUpper());
            QCoreApplication::sendEvent(pCurWidgetObj, &evHide);
        }
        QWidget *pWidgetObj = dynamic_cast<QWidget *>(pPageObj->getWidget());
        if(pWidgetObj) {
            UserEvent evShow(UserEvent::EVT_USER_SHOW_UPDATE, szListUserData.at(2).toUpper());
            QCoreApplication::sendEvent(pWidgetObj, &evShow);
            m_pCentralWidgetObj->setCurrentWidget(pWidgetObj);
        }
    }
}


/**
 * @brief MainWindow::UpdateProjectName
 * @details 更新工程名称
 * @param szName 工程名称
 */
void MainWindow::UpdateProjectName(const QString &szName)
{
    QAction *pActObj = NULL;
    pActObj = QSoftCore::getCore()->getAction("Tools.Run");
    if(!szName.isEmpty()) {
        QString szNameTmp = szName.mid(szName.lastIndexOf("/") + 1, szName.indexOf(".") - szName.lastIndexOf("/") - 1);
        this->m_pProjectTreeViewObj->setProjectName(szNameTmp);
        if(pActObj) {
            pActObj->setEnabled(true);
        }
    } else {
        QSoftCore::getCore()->getProjectCore()->m_szProjFile = "";
        QSoftCore::getCore()->getProjectCore()->m_szProjPath = "";
        if(pActObj) {
            pActObj->setEnabled(false);
        }
        this->m_pProjectTreeViewObj->updateUI();
    }
}


/**
    * @brief MainWindow::onSaveProject
    * @details 保存工程
    */
void MainWindow::onSaveProject()
{
    buildBlockReadTags();
    QSoftCore::getCore()->getProjectCore()->saveToXml(QSoftCore::getCore()->getProjectCore()->m_szProjFile);
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
            QSoftCore::getCore()->getProjectCore()->m_headerObj.byOpenVerifyPassword = 0;
            memset(QSoftCore::getCore()->getProjectCore()->m_headerObj.szPassword, 0, 32);
        } else {
            memset(QSoftCore::getCore()->getProjectCore()->m_headerObj.szPassword, 0, 32);
            QCryptographicHash crypt(QCryptographicHash::Md5);
            crypt.reset();
            crypt.addData(szPwd.toStdString().c_str(), szPwd.toStdString().length());
            QByteArray baPwd = crypt.result();
            QSoftCore::getCore()->getProjectCore()->m_headerObj.byOpenVerifyPassword = baPwd.length();
            memcpy_s(QSoftCore::getCore()->getProjectCore()->m_headerObj.szPassword, 32, baPwd.data(), baPwd.length());
        }
    }
}

/**
    * @brief MainWindow::onCloseProject
    * @details 关闭工程
    */
void MainWindow::onCloseProject()
{
    if(this->m_pProjectTreeViewObj->getProjectName() == tr("未创建工程")) {
        return;
    }

    UpdateProjectName(QString());
    onSlotTabProjectMgrCurChanged(0);
    m_pTabProjectMgrObj->setCurrentIndex(0);

    QAction *pActObj = QSoftCore::getCore()->getAction("Project.OpenPassword");
    if(pActObj) {
        pActObj->setEnabled(false);
    }
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
    QString szFileRuntimeApp = QDir::cleanPath(Helper::AppDir() + "/../../HmiRunTimeBin/HmiRunTime");
#ifdef Q_OS_WIN
    szFileRuntimeApp += ".exe";
#endif
    QFile file(szFileRuntimeApp);
    if (file.exists()) {
        QProcess *process = new QProcess();
        process->setWorkingDirectory(Helper::AppDir() + "/");
        QStringList argv;
        argv << QSoftCore::getCore()->getProjectCore()->m_szProjPath;
        process->startDetached(szFileRuntimeApp, argv);
    }
}

void MainWindow::onSlotStart()
{
    QAction *pActObj = QSoftCore::getCore()->getAction("Tools.Run");
    if(pActObj) {
        pActObj->setEnabled(false);
    }

    pActObj = QSoftCore::getCore()->getAction("Tools.StopRun");
    if(pActObj) {
        pActObj->setEnabled(true);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.New");
    if(pActObj) {
        pActObj->setEnabled(false);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.Close");
    if(pActObj) {
        pActObj->setEnabled(false);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.Save");
    if(pActObj) {
        pActObj->setEnabled(false);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.Open");
    if(pActObj) {
        pActObj->setEnabled(false);
    }
}

void MainWindow::onSlotStop()
{
    QAction *pActObj = QSoftCore::getCore()->getAction("Tools.StopRun");
    if(pActObj) {
        pActObj->setEnabled(false);
    }

    pActObj = QSoftCore::getCore()->getAction("Tools.Run");
    if(pActObj) {
        pActObj->setEnabled(true);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.New");
    if(pActObj) {
        pActObj->setEnabled(true);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.Close");
    if(pActObj) {
        pActObj->setEnabled(true);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.Save");
    if(pActObj) {
        pActObj->setEnabled(true);
    }

    pActObj = QSoftCore::getCore()->getAction("Project.Open");
    if(pActObj) {
        pActObj->setEnabled(true);
    }
}

/**
 * @brief MainWindow::onSlotStopRunProject
 * @details 停止运行工程
 */
void MainWindow::onSlotStopRunProject()
{
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
    QString projFile = QSoftCore::getCore()->getProjectCore()->m_szProjFile.trimmed();
    if(projFile.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("未打开工程文件或未新建工程文件！"));
        return;
    }

    ProjectDownloadDialog dlg(this, projFile);
    if (dlg.exec() == QDialog::Accepted) {
    }
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

        for (int i = 0; i < slist.count(); i++) {
            QAction *pAct = new QAction(slist.at(i));
            pAct->setStatusTip(tr(""));
            connect(pAct, &QAction::triggered, this, [ = ] {
                this->doOpenProject(pAct->text());
            });
            this->m_pMenuProjectObj->insertAction(QSoftCore::getCore()->getAction("Project.LastOpen"), pAct);
        }
        this->m_pMenuProjectObj->removeAction(QSoftCore::getCore()->getAction("Project.LastOpen"));
        return;
    }

    QList<QAction *> listActRemove;
    QList<QAction *> listAct = this->m_pMenuProjectObj->actions();
    for (int i = 0; i < listAct.size(); ++i) {
        QAction *pActObj = listAct.at(i);
        if(pActObj->isSeparator()) {
            listActRemove.append(pActObj);
        }
        QVariant datUser = pActObj->data();
        if(datUser.isValid()) {
            QJsonObject datJson = datUser.toJsonObject();
            if(datJson["which"].toString() == "LastOpen") {
                listActRemove.append(pActObj);
            }
        }
    }
    for (int j = 0; j < listActRemove.size(); ++j) {
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

        if (slist.count() >= 5) {
            slist.removeLast();
        }

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
            if(pAct->text() == tr("最近文件列表")) {
                listActRemove.append(pAct);
            }
        }
        for (int j = 0; j < listActRemove.size(); ++j) {
            this->m_pMenuProjectObj->removeAction(listActRemove.at(j));
        }

        /////////////////////////////////////////////////////

        bStart = bEnd = false;
        QAction *pActPos = NULL;
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

        for (int i = 0; i < slist.count(); i++) {
            QAction *pAct = new QAction(slist.at(i), this);
            pAct->setStatusTip(tr(""));
            connect(pAct, &QAction::triggered, this, [ = ] {
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
    * @brief MainWindow::onSlotTabProjectMgrCurChanged
    * @details 工程管理器标签改变
    * @param index 0：工程标签页, 1：画面标签页
    */
void MainWindow::onSlotTabProjectMgrCurChanged(int index)
{
    QToolBar *pToolBarObj = QSoftCore::getCore()->getToolBar("ToolBar.GraphPage");
    if(QSoftCore::getCore()->getProjectCore()->m_szProjFile == "") {
        if(m_pDesignerWidgetObj) {
            m_pDesignerWidgetObj->setVisible(false);
            UserEvent evShow(UserEvent::EVT_USER_HIDE_UPDATE, QStringList());
            QCoreApplication::sendEvent(m_pDesignerWidgetObj, &evShow);
        }
        if(pToolBarObj) {
            pToolBarObj->setVisible(false);    // 画面编辑工具条
        }
    } else {
        if(pToolBarObj) {
            pToolBarObj->setVisible(index == 1);
        }
        switch (index) {
            case 0: {

            } break;
            case 1: {
                if(m_pDesignerWidgetObj) {
                    UserEvent evShow(UserEvent::EVT_USER_SHOW_UPDATE, QStringList());
                    QCoreApplication::sendEvent(m_pDesignerWidgetObj, &evShow);
                    m_pCentralWidgetObj->setCurrentWidget(m_pDesignerWidgetObj);
                    QAbstractPage* pPageObj = m_mapNameToPage.value("Designer");
                    if(pPageObj) {
                        QSoftCore::getCore()->setActivityStack(pPageObj->getUndoStack());
                    }
                }
            }
            break;
            default:
                break;
        }
    }
}

/**
 * @brief MainWindow::buildBlockReadTags 生成块读变量
 * @return
 */
bool MainWindow::buildBlockReadTags()
{
    QProjectCore* pCoreObj = QSoftCore::getCore()->getProjectCore();
    // Step-1 设备变量分类
    QMap<QString, QVector<Tag *> > mapDevBlockReadTags;
    foreach (Tag *pTagObj, pCoreObj->m_tagMgr.m_vecTags) {
        // 系统变量或内存变量
        if(pTagObj->m_addrType == "AutoAlloc") {
            continue;
        }
        QString dev = pTagObj->m_devType;
        if(mapDevBlockReadTags.count(dev) > 0) {
            QVector<Tag *> &vecTags = mapDevBlockReadTags[dev];
            vecTags.append(pTagObj);
        } else {
            QVector<Tag *> vecTags;
            vecTags.append(pTagObj);
            mapDevBlockReadTags[dev] = vecTags;
        }
    }

    foreach (QString dev, pCoreObj->m_tagMgr.m_mapDevBlockReadTags.keys()) {
        qDeleteAll(pCoreObj->m_tagMgr.m_mapDevBlockReadTags[dev]);
        pCoreObj->m_tagMgr.m_mapDevBlockReadTags[dev].clear();
    }
    pCoreObj->m_tagMgr.m_mapDevBlockReadTags.clear();

    // Step-2 生成<tags>节点
    QList<QString> devs = mapDevBlockReadTags.keys();
    foreach (QString dev, devs) {
        QVector<Tag *> &vecTags = mapDevBlockReadTags[dev];
        XMLObject tagsObj;
        tagsObj.setTagName("tags");
        for(int i = 0; i < vecTags.count(); i++) {
            Tag *pObj = vecTags.at(i);
            pObj->saveToXml(&tagsObj);
        }

        QString xmlDevTags = tagsObj.write();
        QString xmlDevBlockReadTags;
        QVector<QPair<QString, QString>> idToBlockId;

        IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(dev);
        if (pDevPluginObj) {
            DeviceInfo &deviceInfo = pCoreObj->m_deviceInfo;
            DeviceInfoObject *pObj = deviceInfo.getObjectByName(dev);
            if(pObj == NULL) {
                return false;
            }

            bool ret = pDevPluginObj->buildBlockReadTags(xmlDevTags, pObj->m_properties, xmlDevBlockReadTags, idToBlockId);
            if(ret) {
                XMLObject xmlObj;
                QString errorInfo;
                bool bRet = xmlObj.load(xmlDevBlockReadTags, &errorInfo);
                if(bRet) {
                    if(xmlObj.getChildren().size() > 0) {
                        QList<XMLObject* > listTagsObj = xmlObj.getChildren();
                        foreach(XMLObject* pTagObj, listTagsObj) {
                            Tag *pObj = new Tag();
                            pObj->openFromXml(pTagObj);
                            if(pCoreObj->m_tagMgr.m_mapDevBlockReadTags.count(pObj->m_devType) > 0) {
                                QVector<Tag *> &vecTags = pCoreObj->m_tagMgr.m_mapDevBlockReadTags[pObj->m_devType];
                                vecTags.append(pObj);
                            } else {
                                QVector<Tag *> vecTags;
                                vecTags.append(pObj);
                                pCoreObj->m_tagMgr.m_mapDevBlockReadTags[pObj->m_devType] = vecTags;
                            }
                        }
                    }
                } else {
                    qDebug() << "MainWindow::buildBlockReadTags -> parse error info: " << errorInfo;
                }

                // Step-3 变量关联块读变量
                for (int i = 0; i < idToBlockId.size(); ++i) {
                    int tagId = idToBlockId[i].first.toInt();
                    Tag *pObj = pCoreObj->m_tagMgr.getTag(tagId);
                    if(pObj) {
                        pObj->m_blockReadId = idToBlockId[i].second.toInt();
                    }
                }
            }
        }
    }

    return true;
}
