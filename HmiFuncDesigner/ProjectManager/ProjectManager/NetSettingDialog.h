#ifndef NETSETTINGDIALOG_H
#define NETSETTINGDIALOG_H

#include "../../Public/Public.h"
#include <QDialog>
#include <QJsonObject>

namespace Ui {
class NetSettingDialog;
}

class NetSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSettingDialog(QWidget *parent = 0, QString ProjectPath = "");
    ~NetSettingDialog();

    void load();
    void save();

private:
    bool check_data();

private slots:
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();

private:
    Ui::NetSettingDialog *ui;
    QString m_ProjectPath;
};

#endif // NETSETTINGDIALOG_H
