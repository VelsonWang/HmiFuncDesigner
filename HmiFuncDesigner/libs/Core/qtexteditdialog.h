#ifndef QTEXTEDITDIALOG_H
#define QTEXTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class QTextEditDialog;
}

class QTextEditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QTextEditDialog(const QString &text,QWidget *parent = 0);
    ~QTextEditDialog();
    
    QString getText();
    int     getRet();
private slots:
    void on_okbtn_clicked();

    void on_cancelbtn_clicked();

private:
    Ui::QTextEditDialog *ui;
    int     m_ret;
};

#endif // QTEXTEDITDIALOG_H
