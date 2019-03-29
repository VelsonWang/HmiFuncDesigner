#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUndoView>
#include <QUndoGroup>
#include <QUndoStack>
#include <QMainWindow>
#include <QAction>
#include "GraphPage.h"
#include "propertymodel.h"
#include "propertytableview.h"
#include "GraphPageManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(const QString &projpath,
                        const QString &graphPageName,
                        QWidget *parent = 0);
    ~MainWindow();

    // 打开画面
    void openGraphPage(const QString &pagePath, const QString &pageName);

private:
    void updateGraphPageViewInfo(const QString &);
    bool isGraphPageOpen(const QString &filename);

    bool createDocument(GraphPage *graphPage,
                        QGraphicsView *view,
                        const QString &filename);

    void moveCenter();

private slots:
    QGraphicsView *createTabView();
    void slotCloseGraphPage();
    void slotCloseAll();

private:
    QString projpath_;
    QString graphPageName_;

    GraphPage *currentGraphPage_;
    QGraphicsView *currentView_;
    int currentGraphPageIndex_;
};

#endif // MAINWINDOW_H
