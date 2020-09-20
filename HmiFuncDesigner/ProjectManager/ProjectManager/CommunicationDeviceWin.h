#ifndef COMMUNICATIONDEVICEWIN_H
#define COMMUNICATIONDEVICEWIN_H

#include "ChildBase.h"
#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class CommunicationDeviceWin;
}

class CommunicationDeviceWin : public ChildBase
{
    Q_OBJECT

public:
    explicit CommunicationDeviceWin(QWidget *parent = 0,
                                    const QString &itemName = "",
                                    const QString &projName = "");
    ~CommunicationDeviceWin();

public:
    void ListViewUpdate(const QString &it);

    // 打开文件
    void open();
    // 保存文件
    void save();

private:
    void ListViewUISetting();

    void ListViewCommunicationDeviceUpdate();
    void ListViewCOMDeviceUpdate();
    void ListViewNetDeviceUpdate();
    void ListViewBusDeviceUpdate();
    void ListViewOpcDeviceUpdate();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

public slots:
    void on_listViewCommunicationDevice_doubleClicked(const QModelIndex &index);
    void NewDevice();
    void ModifyDevice();
    void DeleteDevice();

private:
    Ui::CommunicationDeviceWin *ui;
    QStandardItemModel *pCommDevModel_;

};

#endif // COMMUNICATIONDEVICEWIN_H
