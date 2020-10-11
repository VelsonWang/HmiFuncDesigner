#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QMap>
#include <QUndoView>
#include <QUndoGroup>
#include <QUndoStack>
#include "GraphPageTreeView.h"
#include "ElementLibraryWidget.h"
#include "GraphPage.h"
#include "GraphPageManager.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "ProjectTreeView.h"
#include "GraphPageListWidget.h"
#include "ChildInterface.h"
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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();
    void UpdateDeviceVariableTableGroup();

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
    virtual bool eventFilter(QObject*, QEvent* e);
    void closeEvent(QCloseEvent *event);  // 关闭事件


public:
    bool isGridVisible() const;
    // 打开画面
    void openGraphPage(const QString &szProjPath, const QString &szProjName, const QString &szPageName);

private:
    void createUndoView();
    void addNewGraphPage();
    QString fixedWindowTitle(const QGraphicsView *viewGraphPage) const;
    int exitResponse();
    QString getFileName();
    void updateGraphPageViewInfo(const QString &);
    void connectGraphPage(GraphPage *graphPage);
    void disconnectGraphPage(GraphPage *graphPage);
    void removeGraphPage(QGraphicsView *view);
    bool isGraphPageOpen(const QString &filename);

    bool createDocument(GraphPage *graphPage, QGraphicsView *view, const QString &filename);
    // 创建空的画面页
    void createEmptyGraphpage(const QString &projPath,
                              const QString &graphPageName,
                              int width,
                              int height);
    // 初始化画面列表控件
    void initGraphPageListWidget();
    // 清空画面列表控件
    void clearGraphPageListWidget();

public slots:
    void slotNewElementAdded();
    void slotElementsDeleted();
    void slotElementIdChanged();
    void slotElementPropertyChanged();
    void slotGraphPagePropertyChanged();
    void propertyValueChanged(QtProperty *property, const QVariant &value);

private slots:
    QGraphicsView *createTabView();
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
    // 退出
    void onExit();

    // 窗口.图形元素 动作响应函数
    void onSlotShowGraphObj(bool);
    // 窗口.属性编辑器 动作响应函数
    void onSlotShowPropEditor(bool);
    // 画面.新建
    void onSlotNewGraphPage();
    // 画面.打开
    void onSlotEditOpen();
    // 画面.保存
    void onSlotSaveGraphPage();
    // 显示栅格
    void onSlotShowGrid(bool);
    // 画面放大
    void onSlotZoomIn();
    // 画面缩小
    void onSlotZoomOut();
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
    // 对齐操作
    void onSlotAlignElements();
    // 水平均匀分布
    void onSlotHUniformDistributeElements();
    // 垂直均匀分布
    void onSlotVUniformDistributeElements();
    // 设置选中控件大小一致
    void onSlotSetTheSameSizeElements();
    // 上移一层
    void onSlotUpLayerElements();
    // 下移一层
    void onSlotDownLayerElements();
    // 画面名称被单击
    void onSlotListWidgetGraphPagesCurrentTextChanged(const QString &szText);
    // 新建画面
    void onNewGraphPage();
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

private:
    QString m_szCurItem;
    QString m_szCurTreeViewItem;
    GraphPage *m_pCurrentGraphPageObj = Q_NULLPTR;
    QGraphicsView *m_pCurrentViewObj = Q_NULLPTR;
    QTabWidget *m_pGraphPageTabWidgetObj = Q_NULLPTR;
    ElementLibraryWidget *m_pElementWidgetObj = Q_NULLPTR;
    bool m_bGraphPageGridVisible;
    int m_iCurrentGraphPageIndex;
    QString m_szCopyGraphPageFileName;
    QWidget* m_childCurrent = Q_NULLPTR;
    QSignalMapper* m_windowMapper = Q_NULLPTR;

    QtVariantPropertyManager *m_pVariantPropertyMgrObj = Q_NULLPTR;
    QtTreePropertyBrowser *m_pPropertyEditorObj = Q_NULLPTR;
    QtVariantEditorFactory *m_pVariantEditorFactoryObj = Q_NULLPTR;
    QMap<QtProperty *, QString> m_mapPropertyObjToId;
    QMap<QString, QtVariantProperty *> m_mapIdToPropertyObj;
    QMap<QString, bool> m_mapIdToExpanded;
    QVBoxLayout *m_pElemetsLayoutObj = Q_NULLPTR;
    QVBoxLayout *m_pPropertyLayoutObj = Q_NULLPTR;
    QWidget *m_pCentralWidgetObj = Q_NULLPTR;
    MdiArea *m_pMdiAreaObj = Q_NULLPTR;
    ProjectTreeView *m_pProjectTreeViewObj = Q_NULLPTR;
    GraphPageListWidget *m_pListWidgetGraphPagesObj = Q_NULLPTR; // 画面名称列表


    QStatusBar *m_pStatusBarObj = Q_NULLPTR; // 状态栏
    QDockWidget *m_pDockProjectMgrObj = Q_NULLPTR; // 工程管理器停靠控件
    QDockWidget *m_pDockPropertyObj = Q_NULLPTR; // 属性停靠控件
    QDockWidget *m_pDockElemetsObj = Q_NULLPTR; // 图形元素停靠控件
    QTabWidget *m_pTabProjectMgrObj = Q_NULLPTR; // 工程管理器TabWidget控件

    QToolBar *m_pToolBarGraphPageEditObj = Q_NULLPTR; // 画面编辑工具条
    QAction *m_pActShowGraphObj = Q_NULLPTR; // 窗口.图形元素
    QAction *m_pActShowPropEditorObj = Q_NULLPTR; // 窗口.属性编辑器
    QAction *m_pActNewGraphPageObj = Q_NULLPTR; // 画面.新建
    QAction *m_pActOpenObj = Q_NULLPTR; // 画面.打开
    QAction *m_pActSaveGraphPageObj = Q_NULLPTR; // 画面.保存
    QAction *m_pActShowGridObj = Q_NULLPTR; // 显示栅格
    QAction *m_pActZoomInObj = Q_NULLPTR; // 画面放大
    QAction *m_pActZoomOutObj = Q_NULLPTR; // 画面缩小
    QUndoGroup *m_pUndoGroupObj = Q_NULLPTR;
    QAction *m_pActUndoObj = Q_NULLPTR; // 撤销
    QAction *m_pActRedoObj = Q_NULLPTR; // 重做
    QAction *m_pActCloseGraphPageObj = Q_NULLPTR; // 关闭画面
    QAction *m_pActCloseAllObj = Q_NULLPTR; // 关闭所有画面
    QAction *m_pActCopyObj = Q_NULLPTR; // 拷贝画面
    QAction *m_pActPasteObj = Q_NULLPTR; // 粘贴画面
    QAction *m_pActDeleteObj = Q_NULLPTR; // 删除画面
    QAction *m_pActAlignTopObj = Q_NULLPTR; // 顶部对齐
    QAction *m_pActAlignDownObj = Q_NULLPTR; // 底部对齐
    QAction *m_pActAlignRightObj = Q_NULLPTR; // 右对齐
    QAction *m_pActAalignLeftObj = Q_NULLPTR; // 左对齐
    QAction *m_pActHUniformDistributeObj = Q_NULLPTR; // 水平均匀分布
    QAction *m_pActVUniformDistributeObj = Q_NULLPTR; // 垂直均匀分布
    QAction *m_pActSetTheSameSizeObj = Q_NULLPTR; // 设置选中控件大小一致
    QAction *m_pActUpLayerObj = Q_NULLPTR; // 上移一层
    QAction *m_pActDownLayerObj = Q_NULLPTR; // 下移一层

    QMenu *m_pMenuProjectObj = Q_NULLPTR; // 工程菜单
    QToolBar *m_pToolBarProjectObj = Q_NULLPTR; // 工程工具条
    QAction *m_pActNewProjObj = Q_NULLPTR; // 新建工程
    QAction *m_pActOpenProjObj = Q_NULLPTR; // 打开工程
    QAction *m_pActCloseProjObj = Q_NULLPTR; // 关闭工程
    QAction *m_pActSaveProjObj = Q_NULLPTR; // 保存工程
    QAction *m_pActRecentProjListObj = Q_NULLPTR; // 最近打开工程
    QAction *m_pActExitObj = Q_NULLPTR; // 退出

    QMenu *m_pMenuViewObj = Q_NULLPTR; // 视图
    QAction *m_pActToolBarObj = Q_NULLPTR;
    QAction *m_pActStatusBarObj = Q_NULLPTR;
    QAction *m_pActWorkSpaceObj = Q_NULLPTR;
    QAction *m_pActDisplayAreaObj = Q_NULLPTR;

    QMenu *m_pMenuToolsObj = Q_NULLPTR; // 工具菜单
    QToolBar *m_pToolBarToolsObj = Q_NULLPTR;
    QAction *m_pActSimulateObj = Q_NULLPTR; // 模拟仿真
    QAction *m_pActRunObj = Q_NULLPTR; // 运行工程
    QAction *m_pActDownloadObj = Q_NULLPTR; // 下载工程
    QAction *m_pActUpLoadObj = Q_NULLPTR; // 上传工程

    QMenu *m_pActWindowMenuObj = Q_NULLPTR; // 窗口菜单
    QAction *m_pActCloseWndObj = Q_NULLPTR;
    QAction *m_pActCloseAllWndObj = Q_NULLPTR;
    QAction *m_pActTileWndObj = Q_NULLPTR; // 平铺
    QAction *m_pActCascadeWndObj = Q_NULLPTR; // 层叠
    QAction *m_pActNextWndObj = Q_NULLPTR;
    QAction *m_pActPreviousWndObj = Q_NULLPTR;
    QAction *m_pActWindowMenuSeparatorObj = Q_NULLPTR;

    QMenu *m_pMenuHelpObj = Q_NULLPTR; // 帮助菜单
    QAction *m_pActHelpObj = Q_NULLPTR; // 帮助
    QAction *m_pActAboutObj = Q_NULLPTR; // 关于

};

#endif // MAINWINDOW_H
