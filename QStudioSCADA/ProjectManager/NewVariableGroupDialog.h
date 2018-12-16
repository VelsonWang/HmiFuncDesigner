#ifndef NEWVARIABLEGROUPDIALOG_H
#define NEWVARIABLEGROUPDIALOG_H

#include <QDialog>

namespace Ui {
class NewVariableGroupDialog;
}

class NewVariableGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewVariableGroupDialog(QWidget *parent = 0);
    ~NewVariableGroupDialog();
    QString GetGroupName();
    void SetGroupName(QString s);
    void SetDialogName(QString s);
    void SetLabelName(QString s);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    Ui::NewVariableGroupDialog *ui;
    QString m_GroupName;
};

#endif // NEWVARIABLEGROUPDIALOG_H
