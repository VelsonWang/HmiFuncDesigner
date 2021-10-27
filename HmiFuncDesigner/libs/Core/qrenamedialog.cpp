#include "qrenamedialog.h"
#include "ui_qrenamedialog.h"

#include "qnamevalidator.h"

QRenameDialog::QRenameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRenameDialog),
    m_nameEdit(new QNameEdit("[_a-zA-Z][_a-zA-Z0-9]{,64}")),
    m_ret(0)
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(1, m_nameEdit);
    setProperty("no-ManhattanStyle", true);
}

QRenameDialog::~QRenameDialog()
{
    delete ui;
}

void QRenameDialog::set_name_list(const QStringList &list)
{
    m_nameEdit->setIntermediateList(list);
}

QString QRenameDialog::get_new_name()
{
    return m_nameEdit->value();
}

void QRenameDialog::set_old_name(const QString &name)
{
    m_nameEdit->setText(name);
    m_nameEdit->selectAll();
}

void QRenameDialog::on_okbtn_clicked()
{
    if(m_nameEdit->value() == "") {
        return;
    }
    m_ret = 1;
    close();
}

void QRenameDialog::on_cancelbtn_clicked()
{
    m_ret = 2;
    close();
}

int QRenameDialog::get_ret()
{
    return m_ret;
}
