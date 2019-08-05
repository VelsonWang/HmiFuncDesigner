#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUndoView>
#include <QUndoGroup>
#include <QUndoStack>
#include "ObjectsTreeView.h"
#include "GraphPageTreeView.h"
#include "ElementLibraryWidget.h"
#include "GraphPage.h"
#include "propertymodel.h"
#include "propertytableview.h"
#include "GraphPageManager.h"
#include "ui_MainWindow.h"

class QToolBar;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(const QString &szProjPath,
                        const QString &szProjName,
                        const QString &szGraphPageName,
                        QWidget *parent = nullptr);
    ~MainWindow();

    bool isGridVisible() const;
    // 打开画面
    void openGraphPage(const QString &szProjPath,
                       const QString &szProjName,
                       const QString &szPageName);

protected:
    void closeEvent(QCloseEvent *);

private:
    void initView();
    void createMenus();
    void createActions();
    void createToolbars();
    void createUndoView();
    void addNewGraphPage();
    QString fixedWindowTitle(const QGraphicsView *viewGraphPage) const;
    int exitResponse();
    QString getFileName();
    void updateGraphPageViewInfo(const QString &);
    void updateObjectTree();
    void connectGraphPage(GraphPage *graphPage);
    void disconnectGraphPage(GraphPage *graphPage);
    void removeGraphPage(QGraphicsView *view);
    bool isGraphPageOpen(const QString &filename);

    bool createDocument(GraphPage *graphPage,
                        QGraphicsView *view,
                        const QString &filename);

public slots:
    void slotNewElementAdded();
    void slotElementsDeleted();
    void slotElementIdChanged();
    void slotElementPropertyChanged();
    void slotGraphPagePropertyChanged();

private slots:
    QGraphicsView *createTabView();
    void slotShowGraphObj(bool);
    void slotShowTreeObj(bool);
    void slotShowPropEditor(bool);
    void slotEditNew();
    void slotEditOpen();
    void slotSaveGraphPage();
    void slotExit();
    void slotShowGrid(bool);
    void slotShowLinear(bool);
    void slotZoomIn();
    void slotZoomOut();
    void slotUpdateActions();
    void slotChangeGraphPage(int);
    void slotChangeGraphPageName();
    void slotCloseGraphPage();
    void slotCloseAll();
    // 对齐操作
    void slotAlignElements();
    // 水平均匀分布
    void slotHUniformDistributeElements();
    // 垂直均匀分布
    void slotVUniformDistributeElements();
    // 设置选中控件大小一致
    void slotSetTheSameSizeElements();
    // 上移一层
    void slotUpLayerElements();
    // 下移一层
    void slotDownLayerElements();
    // 删除
    void slotEditDelete();
    // 拷贝
    void slotEditCopy();
    // 粘贴
    void slotEditPaste();

private:
    QString szProjPath_;
    QString szProjName_;
    QString graphPageName_;

    QAction *actionShowGraphObj_;
    QAction *actionShowTreeObj_;
    QAction *actionShowPropEditor_;
    QAction *actionNew_;
    QAction *actionOpen_;
    QAction *actionSaveGraphPage_;
    QAction *actionExit_;
    QAction *actionShowGrid_;
    QAction *actionShowLinear_;
    QAction *actionZoomIn_;
    QAction *actionZoomOut_;
    QAction *actionUndo_;
    QAction *actionRedo_;
    QAction *actionCloseGraphPage_;
    QAction *actionCloseAll_;

    QAction *actionCopy_; // 拷贝
    QAction *actionPaste_; // 粘贴
    QAction *actionDelete_; // 删除
    QAction *alignTopAction_; // 顶部对齐
    QAction *alignDownAction_; // 底部对齐
    QAction *alignRightAction_; // 右对齐
    QAction *alignLeftAction_; // 左对齐
    QAction *hUniformDistributeAction_; // 水平均匀分布
    QAction *vUniformDistributeAction_; // 垂直均匀分布
    QAction *setTheSameSizeAction_; // 设置选中控件大小一致
    QAction *upLayerAction_; // 上移一层
    QAction *downLayerAction_; // 下移一层

    GraphPage *currentGraphPage_;
    QGraphicsView *currentView_;

    QTabWidget *graphPageTabWidget_;
    ObjectsTreeView *objTree_;
    ElementLibraryWidget *elementWidget_;
    PropertyTableView *propertyView_;
    PropertyModel *propertyModel_;
    QUndoGroup *undoGroup_;

    bool gridVisible_;
    int currentGraphPageIndex_;
};

#endif // MAINWINDOW_H
