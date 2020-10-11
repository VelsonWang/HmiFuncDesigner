#ifndef COMMUNICATIONDEVICECHILD_H
#define COMMUNICATIONDEVICECHILD_H

#include "ChildInterface.h"
#include "ListViewEx.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QVBoxLayout>

class CommunicationDeviceChild : public QWidget, public ChildInterface
{
    Q_OBJECT
    Q_INTERFACES(ChildInterface)
public:
    explicit CommunicationDeviceChild(QWidget *parent = Q_NULLPTR);
    ~CommunicationDeviceChild();

public:
    void buildUserInterface(QMainWindow* pMainWin);
    void removeUserInterface(QMainWindow* pMainWin);
    QString wndTitle() const;

private:
    void listViewUISetting();
    void listViewUpdate();
    void listViewCommunicationDeviceUpdate();
    void listViewCOMDeviceUpdate();
    void listViewNetDeviceUpdate();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

public slots:
    void onSlotListViewProjectDoubleClicked(const QModelIndex &index);
    void onSlotNewDevice();
    void onSlotModifyDevice();
    void onSlotDeleteDevice();

private:
    QStandardItemModel *m_pCommDevModelObj = Q_NULLPTR;
    ListViewEx *m_pListViewCommDevObj = Q_NULLPTR;

};

#endif
