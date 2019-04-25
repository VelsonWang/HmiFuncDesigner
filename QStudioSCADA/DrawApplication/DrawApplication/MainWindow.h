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
    explicit MainWindow(const QString &projpath,
                        const QString &graphPageName,
                        QWidget *parent = 0);
    ~MainWindow();

    bool isGridVisible() const;
    // 打开画面
    void openGraphPage(const QString &pagePath, const QString &pageName);

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

private:
    QString projpath_;
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
