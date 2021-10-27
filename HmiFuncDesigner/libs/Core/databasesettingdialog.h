#ifndef DATABASESETTINGDIALOG_H
#define DATABASESETTINGDIALOG_H

#include "corelibglobal.h"
#include <QDialog>

namespace Ui
{
class DatabaseSettingDialog;
}

class CORELIB_EXPORT DatabaseSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSettingDialog(QWidget *parent = NULL);
    ~DatabaseSettingDialog();

public:
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
    Ui::DatabaseSettingDialog *ui;
};

#endif // DATABASESETTINGDIALOG_H
