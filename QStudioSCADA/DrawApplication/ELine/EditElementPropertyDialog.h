#ifndef EDITELEMENTPROPERTYDIALOG_H
#define EDITELEMENTPROPERTYDIALOG_H

#include <QDialog>

namespace Ui {
class EditElementPropertyDialog;
}

class EditElementPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditElementPropertyDialog(QWidget *parent = 0);
    ~EditElementPropertyDialog();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::EditElementPropertyDialog *ui;
};

#endif // EDITELEMENTPROPERTYDIALOG_H
