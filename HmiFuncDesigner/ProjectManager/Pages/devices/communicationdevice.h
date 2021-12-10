#ifndef COMMUNICATIONDEVICE_H
#define COMMUNICATIONDEVICE_H

#include "listviewex.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QVBoxLayout>

class CommunicationDevice : public QWidget
{
    Q_OBJECT
public:
    explicit CommunicationDevice(QWidget *parent = NULL);
    ~CommunicationDevice();

private:
    void listViewUISetting();
    void listViewUpdate();
    void listViewCommunicationDeviceUpdate();
    void listViewCOMDeviceUpdate();
    void listViewNetDeviceUpdate();

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    bool event(QEvent *ev);

public slots:
    void onSlotListViewProjectDoubleClicked(const QModelIndex &index);
    void onSlotNewDevice();
    void onSlotModifyDevice();
    void onSlotDeleteDevice();

private:
    QStandardItemModel *m_pCommDevModelObj = NULL;
    ListViewEx *m_pListViewCommDevObj = NULL;
    QString m_szItemName = "";
};

#endif
