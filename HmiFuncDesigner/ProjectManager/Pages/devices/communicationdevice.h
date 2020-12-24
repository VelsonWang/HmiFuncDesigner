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
    explicit CommunicationDevice(QWidget *parent = Q_NULLPTR);
    ~CommunicationDevice();

private:
    void listViewUISetting();
    void listViewUpdate();
    void listViewCommunicationDeviceUpdate();
    void listViewCOMDeviceUpdate();
    void listViewNetDeviceUpdate();

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);

public slots:
    void onSlotListViewProjectDoubleClicked(const QModelIndex &index);
    void onSlotNewDevice();
    void onSlotModifyDevice();
    void onSlotDeleteDevice();

private:
    QStandardItemModel *m_pCommDevModelObj = Q_NULLPTR;
    ListViewEx *m_pListViewCommDevObj = Q_NULLPTR;
    QString m_szItemName = "";
};

#endif
