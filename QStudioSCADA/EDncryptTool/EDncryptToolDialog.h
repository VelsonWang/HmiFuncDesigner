#ifndef EDNCRYPTTOOLDIALOG_H
#define EDNCRYPTTOOLDIALOG_H

#include <QDialog>
#include "edncrypt.h"

namespace Ui {
class EDncryptToolDialog;
}

class EDncryptToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EDncryptToolDialog(QWidget *parent = 0);
    ~EDncryptToolDialog();

private:
    int getEDncryptType();

private slots:
    void on_btnEncrypt_clicked();
    void on_btnDncrypt_clicked();

private:
    Ui::EDncryptToolDialog *ui;
};

#endif // EDNCRYPTTOOLDIALOG_H
