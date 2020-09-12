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

#include "SystemParametersWin.h"
#include "ChildBase.h"
#include "ChildForm.h"

#include <QVariant>
#include <QIcon>
#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QHeaderView>
#include <QListWidget>
#include <QMainWindow>
#include <QMdiArea>
#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

signals:
    void treeItemClicked(const QString &itemText);

private slots:   
    void setActiveSubWindow(ChildForm *window);
    ChildForm* getActiveSubWindow();
    void on_actionWorkSpace_triggered(bool checked);
    void on_treeViewProject_clicked(const QModelIndex &index);
    void on_treeViewProject_activated(const QModelIndex &index);
    void tagIOGroupAdd(); // 增加组
    void tagIOGroupRename(); // 重命名组
    void tagIODeleteGroup(); // 删除组
    void tagIOGroupCopy(); // 复制组

private:
    QStandardItemModel *pTreeViewProjectModel;

    QStandardItem *pProjectItem;
    QStandardItem *pSystemParameters;
    QStandardItem *pCommunicationDevice;
    QStandardItem *pComDevice;
    QStandardItem *pNetDevice;
    QStandardItem *pBusDevice;
    QStandardItem *pOPCDevice;
    QStandardItem *pDataBaseConfig;
    QStandardItem *pDevVariable;
    QList<QStandardItem *> pDevVariableTabList;
    QStandardItem *pTmpVariable;
    QStandardItem *pSysVariable;
    QStandardItem *pDataBaseManager;
    QStandardItem *pRealTimeDatabase;
    QStandardItem *pHistoryDatabase;
    QStandardItem *pDrawPage;
    QStandardItem *pLogicProgram;
    QStandardItem *pScriptEditor;
    QStandardItem *pSystemTool;


private:
    ChildForm* activeMdiChild();
    void CreateItemWindows();
    ChildForm* findMdiChild(const QString &windowTitle);
    QMdiSubWindow* findMdiSubWindow(const QString &windowTitle);
    void readSettings();  // 读取窗口设置
    void writeSettings(); // 写入窗口设置
    void initWindow(); // 初始化窗口
    void setUpProjectTreeView();
    void UpdateProjectName(QString name);
    void UpdateDeviceVariableTableGroup();
    void enableToolBar(QString text);
    void loadRecentProjectList();
    void updateRecentProjectList(QString newProj);
    void doOpenProject(QString proj);
    void CreateDefaultIOTagGroup();
    void onTreeViewProjectClicked(const QString &szItemText);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);  // 关闭事件


public:
    bool isGridVisible() const;
    // 打开画面
    void openGraphPage(const QString &szProjPath, const QString &szProjName, const QString &szPageName);

private:
    void initView();
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

public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_7;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QMdiArea *mdiArea;

    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTreeView *treeViewProject;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QListWidget *listWidgetGraphPages;
    QWidget *dockWidgetContents_8;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *ElemetsLayout;

    QWidget *dockPropertyLayout;
    QVBoxLayout *PropertyLayout;

    void setupUi();


    //-------------------------------<以下为整理过的代码>------------------------------------------


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

    // 显示大图标
    void onBigIcon();
    // 显示小图标
    void onSmallIcon();
    // 模拟仿真
    void onSlotSimulate();
    // 运行工程
    void onSlotRunProject();
    // 下载工程
    void onSlotDownloadProject();
    // 上载工程
    void onSlotUpLoadProject();
    // U盘
    void onSlotUDisk();

    // 添加变量
    void onSlotAddTag();
    // 追加变量
    void onSlotAppendTag();
    // 拷贝变量
    void onSlotRowCopyTag();
    // 修改变量
    void onSlotModifyTag();
    // 删除变量
    void onSlotDeleteTag();
    // 导出变量
    void onSlotExportTag();
    // 导入变量
    void onSlotImportTag();

    // 帮助
    void onSlotHelp();
    // 关于
    void onSlotAbout();

    // 新建设备
    void onSlotNewDevice();
    // 修改设备
    void onSlotModifyDevice();
    // 删除设备
    void onSlotDeleteDevice();

private:
    // 创建状态栏
    void createStatusBar();
    // 创建动作
    void createActions();
    // 创建菜单
    void createMenus();
    // 创建工具条
    void createToolbars();

private:
    QString m_szProjPath;
    QString m_szProjName;
    QString m_szCurItem;
    QString m_szCurTreeViewItem;
    GraphPage *m_pCurrentGraphPageObj;
    QGraphicsView *m_pCurrentViewObj;
    QTabWidget *m_pGraphPageTabWidgetObj;
    ElementLibraryWidget *m_pElementWidgetObj;
    bool m_bGraphPageGridVisible;
    int m_iCurrentGraphPageIndex;
    QString m_szCopyGraphPageFileName;

    QtVariantPropertyManager *m_pVariantPropertyMgrObj;
    QtTreePropertyBrowser *m_pPropertyEditorObj;
    QtVariantEditorFactory *m_pVariantEditorFactoryObj;
    QMap<QtProperty *, QString> m_mapPropertyObjToId;
    QMap<QString, QtVariantProperty *> m_mapIdToPropertyObj;
    QMap<QString, bool> m_mapIdToExpanded;

    QStatusBar *m_pStatusBarObj; // 状态栏
    QDockWidget *m_pDockProjectMgrObj; // 工程管理器停靠控件
    QDockWidget *m_pDockPropertyObj; // 属性停靠控件
    QDockWidget *m_pDockElemetsObj; // 图形元素停靠控件
    QTabWidget *m_pTabProjectMgrObj; // 工程管理器TabWidget控件

    QToolBar *m_pToolBarGraphPageEditObj; // 画面编辑工具条
    QAction *m_pActionShowGraphObj; // 窗口.图形元素
    QAction *m_pActionShowPropEditorObj; // 窗口.属性编辑器
    QAction *m_pActionNewGraphPageObj; // 画面.新建
    QAction *m_pActionOpenObj; // 画面.打开
    QAction *m_pActionSaveGraphPageObj; // 画面.保存
    QAction *m_pActionShowGridObj; // 显示栅格
    QAction *m_pActionZoomInObj; // 画面放大
    QAction *m_pActionZoomOutObj; // 画面缩小
    QUndoGroup *m_pUndoGroupObj;
    QAction *m_pActionUndoObj; // 撤销
    QAction *m_pActionRedoObj; // 重做
    QAction *m_pActionCloseGraphPageObj; // 关闭画面
    QAction *m_pActionCloseAllObj; // 关闭所有画面
    QAction *m_pActionCopyObj; // 拷贝画面
    QAction *m_pActionPasteObj; // 粘贴画面
    QAction *m_pActionDeleteObj; // 删除画面
    QAction *m_pActionAlignTopObj; // 顶部对齐
    QAction *m_pActionAlignDownObj; // 底部对齐
    QAction *m_pActionAlignRightObj; // 右对齐
    QAction *m_pActionAalignLeftObj; // 左对齐
    QAction *m_pActionHUniformDistributeObj; // 水平均匀分布
    QAction *m_pActionVUniformDistributeObj; // 垂直均匀分布
    QAction *m_pActionSetTheSameSizeObj; // 设置选中控件大小一致
    QAction *m_pActionUpLayerObj; // 上移一层
    QAction *m_pActionDownLayerObj; // 下移一层

    QMenu *m_pMenuProjectObj; // 工程菜单
    QToolBar *m_pToolBarProjectObj; // 工程工具条
    QAction *m_pActionNewProjObj; // 新建工程
    QAction *m_pActionOpenProjObj; // 打开工程
    QAction *m_pActionCloseProjObj; // 关闭工程
    QAction *m_pActionSaveProjObj; // 保存工程
    QAction *m_pActionRecentProjListObj; // 最近打开工程
    QAction *m_pActionExitObj; // 退出

    QMenu *m_pMenuViewObj; // 视图
    QToolBar *m_pToolBarView; // 视图工具栏
    QAction *m_pActionToolBarObj;
    QAction *m_pActionStatusBarObj;
    QAction *m_pActionWorkSpaceObj;
    QAction *m_pActionDisplayAreaObj;
    QAction *m_pActionBigIconObj; // 显示大图标
    QAction *m_pActionSmallIconObj; // 显示小图标
    QAction *m_pActionEditObj; // 编辑

    QMenu *m_pMenuToolsObj; // 工具菜单
    QToolBar *m_pToolBarToolsObj;
    QAction *m_pActionSimulateObj; // 模拟仿真
    QAction *m_pActionRunObj; // 运行工程
    QAction *m_pActionDownloadObj; // 下载工程
    QAction *m_pActionUpLoadObj; // 上传工程
    QAction *m_pActionUDiskObj; // U盘

    QMenu *m_pMenuTagEditObj; // 变量编辑菜单
    QToolBar *m_pToolBarTagEditObj;
    QAction *m_pActionAddTagObj; // 添加变量
    QAction *m_pActionAppendTagObj; // 追加变量
    QAction *m_pActionRowCopyTagObj; // 拷贝变量
    QAction *m_pActionModifyTagObj; // 修改变量
    QAction *m_pActionDeleteTagObj; // 删除变量
    QAction *m_pActionExportTagObj; // 导出变量
    QAction *m_pActionImportTagObj; // 导入变量

    QMenu *m_pMenuHelpObj; // 帮助菜单
    QAction *m_pActionHelpObj; // 帮助
    QAction *m_pActionAboutObj; // 关于

    QToolBar *m_pToolBarDeviceEditObj; // 设备编辑工具条
    QAction *m_pActionNewDeviceObj; // 新建设备
    QAction *m_pActionModifyDeviceObj; // 修改设备
    QAction *m_pActionDeleteDeviceObj; // 删除设备


};

#endif // MAINWINDOW_H
