#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "corelibglobal.h"
#include <QDialog>
#include <QMap>

namespace Ui
{
class NewProjectDialog;
}

class CORELIB_EXPORT NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = NULL);
    ~NewProjectDialog();

    bool load();
    bool save();

private:
    bool check_data();

private slots:
    void on_btnFileDialog_clicked();
    void on_btnHelp_clicked();
    void on_btnCheck_clicked();
    void on_btnOk_clicked();
    void on_btnExit_clicked();

private:
    Ui::NewProjectDialog *ui;
    QMap<QString, int> m_device;
};

#endif // NEWPROJECTDIALOG_H
