#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QMap>
#include <QUndoView>
#include <QUndoGroup>
#include <QUndoStack>
#include "ElementLibraryWidget.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "ProjectTreeView.h"
#include "ChildInterface.h"
#include "GraphPageListWidget.h"
#include <QVariant>
#include <QIcon>
#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QHeaderView>
#include <QListWidget>
#include <QMainWindow>
#include "MdiArea.h"
#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QSignalMapper>

class QAbstractPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;
    void UpdateDeviceVariableTableGroup();

public:
    // 获取工程所有控件的ID名称
    void getAllElementIDName(QStringList &szIDList);
    // 获取工程所有画面名称
    void getAllGraphPageName(QStringList &szList);

public slots:
    QWidget* getActiveSubWindow();
    void on_actionWorkSpace_triggered(bool checked);
    // 工程树节点被单击
    void onSlotTreeProjectViewClicked(const QString &szItemText);


private:
    QWidget* activeMdiChild();
    void CreateItemWindows();
    QMdiSubWindow* findMdiChild(const QString &szWndTitle);
    void readSettings();  // 读取窗口设置
    void writeSettings(); // 写入窗口设置
    void UpdateProjectName(const QString &szName);
    void loadRecentProjectList();
    void updateRecentProjectList(QString newProj);
    void doOpenProject(QString proj);
    void CreateDefaultIOTagGroup();


protected:
    bool eventFilter(QObject*, QEvent* e) override;
    void closeEvent(QCloseEvent *event) override;  // 关闭事件

public:
    bool isGridVisible() const;

private:
    void createUndoView();
    int exitResponse();
    void updateGraphPageViewInfo(const QString &);
    // 清空画面列表控件
    void clearGraphPageListWidget();

public slots:
    void slotElementIdChanged();
    void slotElementPropertyChanged();
    void slotGraphPagePropertyChanged();

private slots:
    void slotUpdateActions();
    void slotChangeGraphPage(int);
    void slotChangeGraphPageName();

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

    // 关闭画面
    void onSlotCloseGraphPage();
    // 关闭所有画面
    void onSlotCloseAll();
    // 拷贝画面
    void onSlotEditCopy();
    // 粘贴画面
    void onSlotEditPaste();
    // 删除画面
    void onSlotEditDelete();
    // 画面名称被单击
    void onSlotGraphPageNameClicked(QListWidgetItem *pItemObj);
    // 创建指定名称的画面
    void onSlotCreateGraphPageUseName(const QString &szName);
    // 重命名画面
    void onRenameGraphPage();
    // 删除画面
    void onDeleteGraphPage();
    // 复制画面
    void onCopyGraphPage();
    // 粘贴画面
    void onPasteGraphPage();

    // 模拟仿真
    void onSlotSimulate();
    // 运行工程
    void onSlotRunProject();
    // 下载工程
    void onSlotDownloadProject();
    // 上载工程
    void onSlotUpLoadProject();

    // 更新窗口菜单
    void onSlotUpdateWindowMenu();

    // 帮助
    void onSlotHelp();
    // 关于
    void onSlotAbout();


    // 设置窗口标题
    void onSlotSetWindowSetTitle(const QString &szTitle);

    // 工程管理器标签改变
    void onSlotTabProjectMgrCurChanged(int index);
    // 子窗口关闭请求
    void onSlotTabCloseRequested(int);
    // 更新菜单
    void onSlotUpdateMenus();
    //
    void onSlotSetActiveSubWindow(QWidget* window);

private:
    // 初始化UI
    void initUI();
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
    QWidget* m_childCurrent = Q_NULLPTR;
    QSignalMapper* m_windowMapper = Q_NULLPTR;
    QWidget *m_pCentralWidgetObj = Q_NULLPTR;
    MdiArea *m_pMdiAreaObj = Q_NULLPTR;
    ProjectTreeView *m_pProjectTreeViewObj = Q_NULLPTR;
    GraphPageListWidget *m_pListWidgetGraphPagesObj = Q_NULLPTR; // 画面名称列表
    QStatusBar *m_pStatusBarObj = Q_NULLPTR; // 状态栏
    QDockWidget *m_pDockProjectMgrObj = Q_NULLPTR; // 工程管理器停靠控件
    QTabWidget *m_pTabProjectMgrObj = Q_NULLPTR; // 工程管理器TabWidget控件
    QUndoGroup *m_pUndoGroupObj = Q_NULLPTR;
    QMenu *m_pMenuProjectObj = Q_NULLPTR; // 工程菜单
    QMenu *m_pMenuViewObj = Q_NULLPTR; // 视图
    QMenu *m_pMenuToolsObj = Q_NULLPTR; // 工具菜单
    QMenu *m_pActWindowMenuObj = Q_NULLPTR; // 窗口菜单
    QMenu *m_pMenuHelpObj = Q_NULLPTR; // 帮助菜单
    QMap<QString, QAbstractPage*> m_mapNameToPage;
};

#endif // MAINWINDOW_H
