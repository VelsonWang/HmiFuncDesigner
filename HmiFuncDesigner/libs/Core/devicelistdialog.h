#ifndef DEVICELISTDIALOG_H
#define DEVICELISTDIALOG_H

#include "corelibglobal.h"
#include <QDialog>
#include <QStandardItemModel>

namespace Ui
{
class DeviceListDialog;
}

class CORELIB_EXPORT DeviceListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceListDialog(QString stype = "COM", QWidget *parent = NULL);
    ~DeviceListDialog();
    // 取得设备名称
    QString GetDeviceName() const;


private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    Ui::DeviceListDialog *ui;

private:
    void TreeViewInit();

private:
    QString m_deviceName;
    QStandardItemModel *m_treeViewItemModel;
    QStringList m_supportDevList;
    QString m_portType;
};

#endif // DEVICELISTDIALOG_H
