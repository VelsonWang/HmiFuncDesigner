#ifndef NewNetDeviceDialog_H
#define NewNetDeviceDialog_H

#include "../Public/Public.h"
#include "DeviceInfo.h"
#include <QDialog>


namespace Ui {
class NewNetDeviceDialog;
}


class NewNetDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewNetDeviceDialog(QWidget *parent = 0);
    ~NewNetDeviceDialog();

    void load(int id);
    void save(int id);

public:
    NetDevice* GetNetDevice();
    QString GetDeviceName() const;
    // 设置已经建立的设备名称列表
    void SetListDeviceName(QStringList l);

private:
    bool check_data();

private slots:
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();
    void on_btnDeviceSelect_clicked();
    void on_btnProtocolSelect_clicked();

private:
    Ui::NewNetDeviceDialog *ui;
    NetDevice m_dev;
    QStringList m_ListDeviceName;
};

#endif // NewNetDeviceDialog_H
