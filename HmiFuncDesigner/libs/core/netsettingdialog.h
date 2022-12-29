#ifndef NETSETTINGDIALOG_H
#define NETSETTINGDIALOG_H

#include "corelibglobal.h"
#include <QDialog>

namespace Ui
{
class NetSettingDialog;
}

class CORELIB_EXPORT NetSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSettingDialog(QWidget *parent = NULL);
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
};

#endif // NETSETTINGDIALOG_H
