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
    ProjectTreeView(QWidget *parent = Q_NULLPTR);
    ~ProjectTreeView();

    // 更新树形控件
    void updateUI();
    // 获取设备变量组的个数
    int getDevTagGroupCount();
    // 获取工程名称
    QString getProjectName();
    // 设置工程名称
    void setProjectName(const QString &szName);
    // 更新设备变量组
    void updateDeviceTagGroup();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

public slots:
    // 增加组
    void onSlotTagIOGroupAdd();
    // 重命名组
    void onSlotTagIOGroupRename();
    // 删除组
    void onSlotTagIODeleteGroup();
    // 复制组
    void onSlotTagIOGroupCopy();


private slots:
    // 叶节点被单击
    void onSlotClicked(const QModelIndex &index);
    // 叶节点被双击击
    void onSlotDoubleClicked(const QModelIndex &index);

signals:
    // 设置窗口标题
    void sigNotifySetWindowSetTitle(const QString &szTitle);

private:
    QStandardItemModel *m_pItemModelObj = Q_NULLPTR;
    QStandardItem *m_pProjectItemObj = Q_NULLPTR;
    QStandardItem *m_pSystemParametersObj = Q_NULLPTR;
    QStandardItem *m_pCommunicationDeviceObj = Q_NULLPTR;
    QStandardItem *m_pComDeviceObj = Q_NULLPTR;
    QStandardItem *m_pNetDeviceObj = Q_NULLPTR;
    QStandardItem *m_pBusDeviceObj = Q_NULLPTR;
    QStandardItem *m_pOPCDeviceObj = Q_NULLPTR;
    QStandardItem *m_pDataBaseConfigObj = Q_NULLPTR;
    QStandardItem *m_pDevTagObj = Q_NULLPTR;
    QList<QStandardItem *> m_pDevVariableTabListObj;
    QStandardItem *m_pTmpVariableObj = Q_NULLPTR;
    QStandardItem *m_pSysVariableObj = Q_NULLPTR;
    QStandardItem *m_pDataBaseManagerObj = Q_NULLPTR;
    QStandardItem *m_pRealTimeDatabaseObj = Q_NULLPTR;
    QStandardItem *m_pHistoryDatabaseObj = Q_NULLPTR;
    QStandardItem *m_pLogicProgramObj = Q_NULLPTR;
    QStandardItem *m_pScriptEditorObj = Q_NULLPTR;
    QStandardItem *m_pSystemToolObj = Q_NULLPTR;



};

#endif // PROJECTTREEVIEW_H
