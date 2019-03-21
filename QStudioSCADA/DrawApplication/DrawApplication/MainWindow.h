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
#include "SaveStrategyDialog.h"
#include "PreviewWindow.h"
#include "GraphPageManager.h"
#include "ui_MainWindow.h"


class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(const QString &projpath,
                        const QString &graphPageName,
                        QWidget *parent = 0);
    ~MainWindow();

    bool isGridVisible() const;
    // 创建和保存画面
    void addAndSaveGraphPage(const QString &pagePath,
                             const QString &pageName,
                             int width,
                             int height);

protected:
    void closeEvent(QCloseEvent *);

private:
    void initView();
    void createMenus();
    void createActions();
    void createToolbars();
    void createUndoView();
    void addNewGraphPage();
    void saveImage();
    QString fixedWindowTitle(const QGraphicsView *viewGraphPage) const;
    int exitResponse();
    QString getFileName();
    void updateGraphPageViewInfo(const QString &);
    void updateObjectTree();
    void connectGraphPage(GraphPage *graphPage);
    void disconnectGraphPage(GraphPage *graphPage);
    void saveProject(const QString &);
    void loadProject(const QString &);
    void removeGraphPage(QGraphicsView *view);
    bool isGraphPageOpen(const QString &filename);
    bool createDocument(GraphPage *graphPage,QGraphicsView *view,const QString &filename);

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
    void slotShowProjTree(bool);
    void slotEditNew();
    void slotEditOpen();
    void slotSaveProject();
    void slotSaveGraphPageAs();
    void slotSaveGraphPage();
    void slotExit();
    void slotShowGrid(bool);
    void slotShowLinear(bool);
    void slotZoomIn();
    void slotZoomOut();
    void slotPreview();
    void slotUpdateActions();
    void slotChangeGraphPage(int);
    void slotChangeGraphPageName();
    void slotCloseGraphPage();
    void slotCloseAll();

private:
    QString projpath_;
    QString graphPageName_;

    QAction *actionShowGraphObj;
    QAction *actionShowTreeObj;
    QAction *actionShowPropEditor;
    QAction *actionShowProjTree;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSaveProject;
    QAction *actionSaveGraphPageAs;
    QAction *actionSaveGraphPage;
    QAction *actionExit;
    QAction *actionShowGrid;
    QAction *actionShowLinear;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionPreview;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCloseGraphPage;
    QAction *actionCloseAll;

    GraphPage *currentGraphPage;
    QGraphicsView *currentView;

    QTabWidget *graphPageTabWidget_;
    GraphPageTreeView *grapePageTree_;
    ObjectsTreeView *objTree;
    ElementLibraryWidget *elementWidget;
    PropertyTableView *propertyView;
    PropertyModel *propertyModel;
    QUndoGroup *undoGroup;
    PreviewWindow *preview;

    bool gridVisible;
    int currentGraphPageIndex;


};

#endif // MAINWINDOW_H
