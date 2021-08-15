#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMap>
#include <QUndoView>
#include <QUndoGroup>
#include <QUndoStack>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "ProjectTreeView.h"
#include <QVariant>
#include <QIcon>
#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QHeaderView>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

class QAbstractPage;
class QStackedWidget;
class QRunningManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public slots:
    void on_actionWorkSpace_triggered(bool checked);
    // 工程树节点被单击
    void onSlotTreeProjectViewClicked(const QString &szItemText);


private:
    void CreateItemWindows();
    void readSettings();  // 读取窗口设置
    void writeSettings(); // 写入窗口设置
    void UpdateProjectName(const QString &szName);
    void loadRecentProjectList();
    void updateRecentProjectList(QString newProj);
    void doOpenProject(QString proj);

protected:
    void closeEvent(QCloseEvent *event) override;  // 关闭事件

public:
    bool isGridVisible() const;

private:
    void createUndoView();

private slots:
    // 新建工程
    void onNewPoject();
    // 打开工程
    void onOpenProject();
    // 关闭工程
    void onCloseProject();
    // 保存工程
    void onSaveProject();
    // 设置打开工程的密码
    void onSetOpenProjPassword();
    // 退出
    void onExit();

    // 模拟仿真
    void onSlotSimulate();

    // 运行工程
    void onSlotRunProject();
    void onSlotStart();
    void onSlotStop();

    // 停止运行工程
    void onSlotStopRunProject();
    // 下载工程
    void onSlotDownloadProject();
    // 上载工程
    void onSlotUpLoadProject();

    // 帮助
    void onSlotHelp();
    // 关于
    void onSlotAbout();

    // 工程管理器标签改变
    void onSlotTabProjectMgrCurChanged(int index);

private:
    // 创建状态栏
    void createStatusBar();
    // 创建动作
    void createActions();
    // 创建菜单
    void createMenus();
    // 创建工具条
    void createToolbars();
    // 拷贝系统变量
    void copySystemTags();

private:
    QString m_szCurItem;
    QString m_szCurTreeViewItem;
    QWidget *m_pDesignerWidgetObj = Q_NULLPTR;
    bool m_bGraphPageGridVisible;
    int m_iCurrentGraphPageIndex;
    QString m_szCopyGraphPageFileName;
    QStackedWidget *m_pCentralWidgetObj = Q_NULLPTR;
    ProjectTreeView *m_pProjectTreeViewObj = Q_NULLPTR;
    QStatusBar *m_pStatusBarObj = Q_NULLPTR; // 状态栏
    QDockWidget *m_pDockProjectMgrObj = Q_NULLPTR; // 工程管理器停靠控件
    QTabWidget *m_pTabProjectMgrObj = Q_NULLPTR; // 工程管理器TabWidget控件
    QMenu *m_pMenuProjectObj = Q_NULLPTR; // 工程菜单
    QMenu *m_pMenuViewObj = Q_NULLPTR; // 视图
    QMenu *m_pMenuToolsObj = Q_NULLPTR; // 工具菜单
    QMenu *m_pMenuHelpObj = Q_NULLPTR; // 帮助菜单
    QMap<QString, QAbstractPage*> m_mapNameToPage;
};

#endif // MAINWINDOW_H
