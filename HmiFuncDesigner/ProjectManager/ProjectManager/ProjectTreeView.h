#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>
#include <QContextMenuEvent>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>

class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    ProjectTreeView(QWidget *parent = NULL);
    ~ProjectTreeView();

    // 更新树形控件
    void updateUI();
    // 获取设备变量组的个数
    int getDevTagGroupCount();
    // 获取工程名称
    QString getProjectName();
    // 设置工程名称
    void setProjectName(const QString &szName);

private slots:
    // 叶节点被单击
    void onSlotClicked(const QModelIndex &index);
    // 叶节点被双击击
    void onSlotDoubleClicked(const QModelIndex &index);

signals:
    // 叶节点被单击
    void sigNotifyClicked(const QString &szItemText);

private:
    QStandardItemModel *m_pItemModelObj = NULL;
    QStandardItem *m_pProjectItemObj = NULL;
    QStandardItem *m_pSystemParametersObj = NULL;
    QStandardItem *m_pCommunicationDeviceObj = NULL;
    QStandardItem *m_pComDeviceObj = NULL;
    QStandardItem *m_pNetDeviceObj = NULL;
    QStandardItem *m_pTagMgrObj = NULL;
    QStandardItem *m_pDevTagObj = NULL;
    QList<QStandardItem *> m_pDevVariableTabListObj;
    QStandardItem *m_pTmpVariableObj = NULL;
    QStandardItem *m_pSysVariableObj = NULL;
    QStandardItem *m_pDataBaseManagerObj = NULL;
    QStandardItem *m_pRealTimeDatabaseObj = NULL;
    QStandardItem *m_pHistoryDatabaseObj = NULL;
    QStandardItem *m_pLogicProgramObj = NULL;
    QStandardItem *m_pScriptEditorObj = NULL;
    QStandardItem *m_pSystemToolObj = NULL;



};

#endif // PROJECTTREEVIEW_H
