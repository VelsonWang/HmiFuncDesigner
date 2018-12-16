#ifndef NewNetDeviceDialog_H
#define NewNetDeviceDialog_H

#include "../Public/Public.h"
#include "./DeviceBase.h"
#include <QDialog>
#include <QJsonObject>

namespace Ui {
class NewNetDeviceDialog;
}

class NetDevice : public DeviceBase
{
public:
    QString m_sIpAddress;
    int m_iPort;
    QString m_sIpAddress1;
    int m_iPort1;
};

class NewNetDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewNetDeviceDialog(QWidget *parent = 0, QString ProjectPath = "");
    ~NewNetDeviceDialog();

public:
    bool loadFromFile(SaveFormat saveFormat, QString file);
    bool saveToFile(SaveFormat saveFormat);
    NetDevice* GetNetDevice();
    QString GetDeviceName() const;
    // 设置已经建立的设备名称列表
    void SetListDeviceName(QStringList l);

private:
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
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
    QString m_ProjectPath;
    NetDevice m_dev;
    QStringList m_ListDeviceName;
    QString m_sOldDeviceName;
};

#endif // NewNetDeviceDialog_H
