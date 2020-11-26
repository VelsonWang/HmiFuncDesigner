#ifndef DEVICELISTDIALOG_H
#define DEVICELISTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
class DeviceListDialog;
}

class DeviceListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceListDialog(QString stype = "COM", QWidget *parent = Q_NULLPTR);
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
    QString m_DeviceName;
    QStandardItemModel *pTreeViewItemModel;
    QStringList m_SupportDevList;
    QString m_strPortType;
};

#endif // DEVICELISTDIALOG_H
