#ifndef CREGISTERDIALOG_H
#define CREGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class CRegisterDialog;
}

class CRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRegisterDialog(QWidget *parent = 0);
    ~CRegisterDialog();

    static int GeneratorDialog();
    static int RegisterDialog();

public slots:
    void on_btn_generate_clicked();
    void on_btn_register_clicked();
    void on_btn_close_clicked();

private:
    Ui::CRegisterDialog *ui;
};

#endif // CREGISTERDIALOG_H
