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
    void on_actionSimulate_triggered();
    void on_actionRun_triggered();
    void on_actionUpLoad_triggered();
    void on_actionUDisk_triggered();
    void on_actionDownload_triggered();
    void on_actionAddTag_triggered();
    void on_actionAppendTag_triggered();
    void on_actionRowCopyTag_triggered();
    void on_actionColumnCopyTag_triggered();
    void on_actionModifyTag_triggered();
    void on_actionDeleteTag_triggered();
    void on_actionExportTag_triggered();
    void on_actionImportTag_triggered();
    void on_actionDeviceNew_triggered();
    void on_actionDeviceModify_triggered();
    void on_actionDeviceDelete_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionBigIcon_triggered();
    void on_actionSmallIcon_triggered();






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
    void openGraphPage(const QString &szProjPath,
                       const QString &szProjName,
                       const QString &szPageName);

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

    bool createDocument(GraphPage *graphPage,
                        QGraphicsView *view,
                        const QString &filename);
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



private:
public:

    QAction *actionToolBar;
    QAction *actionStatusBar;
    QAction *actionWorkSpace;
    QAction *actionDisplayArea_I;
    QAction *actionBigIcon;
    QAction *actionSmallIcon;
    QAction *actionEdit;
    QAction *actionSimulate;
    QAction *actionRun;
    QAction *actionDownload;
    QAction *actionUpLoad;
    QAction *actionUDisk;
    QAction *actionAddTag;
    QAction *actionAppendTag;
    QAction *actionRowCopyTag;
    QAction *actionColumnCopyTag;
    QAction *actionModifyTag;
    QAction *actionDeleteTag;
    QAction *actionExportTag;
    QAction *actionImportTag;
    QAction *actionDeviceNew;
    QAction *actionDeviceModify;
    QAction *actionDeviceDelete;
    QAction *actionHelp;
    QAction *actionAbout;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_7;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;

    QMenu *menuView;
    QMenu *menu_T;
    QMenu *menu_D;
    QMenu *menu;

    QStatusBar *statusBar;
    QToolBar *ViewToolBar;

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
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *PropertyLayout;
    QToolBar *RunToolBar;
    QToolBar *TagOperateToolBar;
    QToolBar *DeviceOperateToolBar;
    QToolBar *toolBar;

    void setupUi();
    void retranslateUi();


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


private:
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

    QDockWidget *m_pDockProjectMgrObj; // 工程管理器停靠控件
    QDockWidget *m_pDockPropertyObj; // 属性停靠控件
    QDockWidget *m_pDockElemetsObj; // 图形元素停靠控件
    QTabWidget *m_pTabProjectMgrObj; // 工程管理器TabWidget控件

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






};

#endif // MAINWINDOW_H
