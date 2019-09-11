#include "NewVariableGroupDialog.h"
#include "ui_NewVariableGroupDialog.h"

NewVariableGroupDialog::NewVariableGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewVariableGroupDialog),
    m_GroupName("")
{
    ui->setupUi(this);
}

NewVariableGroupDialog::~NewVariableGroupDialog()
{
    delete ui;
}
QString NewVariableGroupDialog::GetGroupName()
{
    return m_GroupName;
}

void NewVariableGroupDialog::SetGroupName(QString s)
{
    m_GroupName = s;
    ui->editName->setText(m_GroupName);
}

void NewVariableGroupDialog::on_btnOk_clicked()
{
    m_GroupName = ui->editName->text();
    this->accept();
}

void NewVariableGroupDialog::on_btnCancel_clicked()
{
    this->reject();
}

void NewVariableGroupDialog::SetDialogName(QString s)
{
    this->setWindowTitle(s);
}

void NewVariableGroupDialog::SetLabelName(QString s)
{
    ui->lblName->setText(s);
}



