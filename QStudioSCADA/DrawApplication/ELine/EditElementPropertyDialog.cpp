#include "EditElementPropertyDialog.h"
#include "ui_EditElementPropertyDialog.h"

EditElementPropertyDialog::EditElementPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditElementPropertyDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("常用属性"));
}

EditElementPropertyDialog::~EditElementPropertyDialog()
{
    delete ui;
}

void EditElementPropertyDialog::on_btnOk_clicked()
{
    this->accept();
}

void EditElementPropertyDialog::on_btnCancel_clicked()
{
    this->reject();
}
