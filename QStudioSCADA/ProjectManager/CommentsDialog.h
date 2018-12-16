#ifndef COMMENTSDIALOG_H
#define COMMENTSDIALOG_H

#include <QDialog>

namespace Ui {
class CommentsDialog;
}

class CommentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommentsDialog(QWidget *parent = 0);
    ~CommentsDialog();
    void SetCommentsText(QString s);
    QString GetCommentsText();

private slots:
    void on_btnOk_clicked();

    void on_btnExit_clicked();

private:
    Ui::CommentsDialog *ui;
};

#endif // COMMENTSDIALOG_H
