#include "CommentsDialog.h"
#include "ui_CommentsDialog.h"

CommentsDialog::CommentsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommentsDialog)
{
    ui->setupUi(this);
}

CommentsDialog::~CommentsDialog()
{
    delete ui;
}


void CommentsDialog::setCommentsText(QString s)
{
    ui->textComments->setPlainText(s);
}

QString CommentsDialog::getCommentsText()
{
    return ui->textComments->toPlainText();
}


void CommentsDialog::on_btnOk_clicked()
{
    accept();
}

void CommentsDialog::on_btnExit_clicked()
{
    reject();
}
