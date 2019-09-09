#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "../Public/Public.h"
#include <QDialog>
#include <QMap>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = 0, QString projPath = "");
    ~NewProjectDialog();

    QString GetProjectName();
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
    QMap<QString, int> deviceMap_;
    QString projectName_;
    QString projectPath_;
};

#endif // NEWPROJECTDIALOG_H
