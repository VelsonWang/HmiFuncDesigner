#include "qtexteditdialog.h"
#include "ui_qtexteditdialog.h"

QTextEditDialog::QTextEditDialog(const QString &text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTextEditDialog),
    m_ret(0)
{
    ui->setupUi(this);
    ui->text_edit->setPlainText(text);
    ui->text_edit->moveCursor(QTextCursor::End);
}

QTextEditDialog::~QTextEditDialog()
{
    delete ui;
}

int QTextEditDialog::getRet()
{
    return m_ret;
}

QString QTextEditDialog::getText()
{
    return ui->text_edit->toPlainText();
}

void QTextEditDialog::on_okbtn_clicked()
{
    m_ret = 1;
    close();
}

void QTextEditDialog::on_cancelbtn_clicked()
{
    close();
}
