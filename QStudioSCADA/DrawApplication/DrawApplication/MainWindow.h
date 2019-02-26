#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUndoView>
#include <QUndoGroup>
#include <QUndoStack>
#include "objectstreeview.h"
#include "ProjectTreeView.h"
#include "ElementLibraryWidget.h"
#include "GraphPage.h"
#include "newcomponentdialog.h"
#include "propertyeditor/propertymodel.h"
#include "propertyeditor/propertytableview.h"
#include "deviceeditor/deviceeditordialog.h"
#include "indicationeditor/indicationeditordialog.h"
#include "savestrategydialog.h"
#include "previewwindow.h"
#include "GraphPageManager.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    bool isGridVisible() const;

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
    QAction *actionShowDevDialog;
    QAction *actionShowIndicationDialog;

    GraphPage *currentGraphPage;
    QGraphicsView *currentView;

    QTabWidget *GraphPageTabWidget;
    ProjectTreeView *projTree;
    ObjectsTreeView *objTree;
    ElementLibraryWidget *elementWidget;
    PropertyTableView *propertyView;
    PropertyModel *propertyModel;
    DeviceEditorDialog *deviceEditor;
    QUndoGroup *undoGroup;
    PreviewWindow *preview;

    bool gridVisible;
    int currentGraphPageIndex;

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
    void slotShowDevDialog();
    void slotShowIndicationDialog();
};

#endif // MAINWINDOW_H
