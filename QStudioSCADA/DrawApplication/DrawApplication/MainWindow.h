#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QMap>
#include "ChildGraphWin.h"
#include "PublicDefine.h"
#include "IDrawApplicationPlugin.h"
#include "IElement.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString projpath, QString drawPageName, QWidget *parent = 0);
    void setStatusBarMessage(const QString &msg);
    ~MainWindow();

private:
    ChildGraphWin* activeMdiChild();
    ChildGraphWin* createMdiChild(QString filePathName, QString page);
    ChildGraphWin* findMdiChild(const QString &windowTitle);
    void doOpenDraw(QString file); // 打开画面
    void readSettings();  // 读取窗口设置
    void writeSettings(); // 写入窗口设置
    void initWindow(); // 初始化窗口
    void LoadDrawNameList();
    void SaveDrawNameList();
    void TreeViewGraphUISetting(); // 图形列表UI设置
    void TreeViewGraphUpdate(); // 图形列表更新
    void ToolBoxUISetting(); // 工具箱UI设置

private slots:
    void setActiveSubWindow(ChildGraphWin *window);
    ChildGraphWin* getActiveSubWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_treeViewGraph_doubleClicked(const QModelIndex &index);
    void ToolButtonClick(QString s);

signals:
    void sigToolButtonClick(StdElement *e);

private:
    Ui::MainWindow *ui;
    ChildGraphWin *m_ChildGraphWin;
    QString m_ProjectPath;
    QStringList m_GraphPageNameList;
    QStandardItemModel *pTreeViewGraphModel;


protected:
    void closeEvent(QCloseEvent *event);  // 关闭事件

///////////////////////////////////////////////////

public:
    void LoadPlugin();
    IDrawApplicationPlugin* GetPlugin(const QString &class_type,const QString &element_name);
    IDrawApplicationPlugin* GetPluginByElementName(const QString &element_name);
    QMap<QString, IDrawApplicationPlugin*> GetPluginByType(const QString &class_type);
    void ReleasePlugin();

private:
    //------type---------name-------plugin
    QMap<QString, QMap<QString, IDrawApplicationPlugin*> > m_toolPlugins;


//////////////////////////////////////////////////////



};

#endif // MAINWINDOW_H
