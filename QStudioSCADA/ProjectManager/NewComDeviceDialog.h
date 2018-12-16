#ifndef NEWCOMDEVICEDIALOG_H
#define NEWCOMDEVICEDIALOG_H

#include "../Public/Public.h"
#include "./DeviceBase.h"
#include <QDialog>
#include <QJsonObject>

namespace Ui {
class NewComDeviceDialog;
}

class ComDevice : public DeviceBase
{
public:
    QString m_sPortNumber;
    int m_iBaudrate;
    int m_iDatabit;
    float m_fStopbit;
    QString m_sVerifybit;
    int m_iTimeout;
};

class NewComDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewComDeviceDialog(QWidget *parent = 0, QString ProjectPath = "");
    ~NewComDeviceDialog();

public:
    bool loadFromFile(SaveFormat saveFormat, QString file);
    bool saveToFile(SaveFormat saveFormat);
    ComDevice* GetComDevice();
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
    Ui::NewComDeviceDialog *ui;
    QString m_ProjectPath;
    ComDevice m_dev;
    QStringList m_ListDeviceName;
    QString m_sOldDeviceName;
};

#endif // NEWCOMDEVICEDIALOG_H
