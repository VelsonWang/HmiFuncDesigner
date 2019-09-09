#ifndef NEWCOMDEVICEDIALOG_H
#define NEWCOMDEVICEDIALOG_H

#include "../Public/Public.h"
#include "DeviceInfo.h"
#include <QDialog>

namespace Ui {
class NewComDeviceDialog;
}


class NewComDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewComDeviceDialog(QWidget *parent = 0);
    ~NewComDeviceDialog();

    void load(int id);
    void save(int id);

public:
    ComDevice* GetComDevice();
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
    Ui::NewComDeviceDialog *ui;
    ComDevice m_dev;
    QStringList m_ListDeviceName;
};

#endif // NEWCOMDEVICEDIALOG_H
