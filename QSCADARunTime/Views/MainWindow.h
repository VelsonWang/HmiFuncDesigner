#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    bool createDocument(GraphPage *graphPage, const QString &filename);
    void moveCenter();

private:
    QString projpath_;
    QString graphPageName_;
    GraphPage *currentGraphPage_;
};

#endif // MAINWINDOW_H
