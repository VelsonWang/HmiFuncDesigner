#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QMap>
#include "SystemParametersWin.h"
#include "ChildBase.h"
#include "ChildForm.h"
#include "DBVarGroup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void treeItemClicked(const QString &itemText);

private slots:   
    void setActiveSubWindow(ChildForm *window);
    ChildForm* getActiveSubWindow();
    void on_actionNewPoject_triggered();
    void on_actionWorkSpace_triggered(bool checked);
    void on_treeViewProject_clicked(const QModelIndex &index);
    void on_actionOpenProject_triggered();
    void on_actionSaveProject_triggered();
    void on_treeViewProject_activated(const QModelIndex &index);
    void VariableGroupAdd(); // 增加组
    void VariableGroupRename(); // 重命名组
    void VariableDeleteGroup(); // 删除组
    void VariableGroupCopy(); // 复制组
    void VariableModifyGroupID(); // 修改ID
    void on_actionSimulate_triggered();
    void on_actionRun_triggered();
    void on_actionUpLoad_triggered();
    void on_actionUDisk_triggered();
    void on_actionDownload_triggered();
    void on_actionCloseProject_triggered();
    void on_actionExit_triggered();
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

private:
    Ui::MainWindow *ui;
    QString m_strProjectPath;
    QString m_strProjectName;
    QString m_CurItem;
    QString m_CurTreeViewItem;
};

#endif // MAINWINDOW_H
