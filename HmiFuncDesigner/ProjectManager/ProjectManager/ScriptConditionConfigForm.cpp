#include "ScriptConditionConfigForm.h"
#include "ui_ScriptConditionConfigForm.h"
#include "ScriptRunConditionEditorDlg.h"
#include <QMessageBox>


ScriptConditionConfigForm::ScriptConditionConfigForm(QString projectPath, QWidget *parent) :
    QDialog(parent),
    m_strProjectPath(projectPath),
    ui(new Ui::ScriptConditionConfigForm)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->btnOk->setFocus();
    ui->btnOk->setDefault(true);
}

ScriptConditionConfigForm::~ScriptConditionConfigForm()
{
    delete ui;
}

QString ScriptConditionConfigForm::GetName()
{
    return ui->editScriptName->text();
}

void ScriptConditionConfigForm::SetName(const QString &name)
{
    ui->editScriptName->setText(name);
}

bool ScriptConditionConfigForm::isInUse()
{
    return ui->checkBoxIsInUse->isChecked();
}

void ScriptConditionConfigForm::SetInUse(bool b)
{
    ui->checkBoxIsInUse->setChecked(b);
}

QString ScriptConditionConfigForm::GetDescription()
{
    return ui->textEditDescription->toPlainText();
}

void ScriptConditionConfigForm::SetDescription(const QString &des)
{
    ui->textEditDescription->setText(des);
}

QString ScriptConditionConfigForm::GetRunMode()
{
    if(ui->rbtnOnStart->isChecked())
    {
        return QString("RunOnStart");
    }
    else if(ui->rbtnOnPeriod->isChecked())
    {
        return QString("RunOnPeriod");
    }
    else if(ui->rbtnOnCondition->isChecked())
    {
        return QString("RunOnCondition");
    }
    else
    {
        return QString("");
    }
}

void ScriptConditionConfigForm::SetRunMode(const QString &run)
{
    ui->rbtnOnStart->setChecked(run == QString("RunOnStart"));
    ui->rbtnOnPeriod->setChecked(run == QString("RunOnPeriod"));
    ui->rbtnOnCondition->setChecked(run == QString("RunOnCondition"));
}

QString ScriptConditionConfigForm::GetRunModeArgs()
{
    if(ui->rbtnOnPeriod->isChecked())
    {
        return ui->editPeriod->text();
    }
    else if(ui->rbtnOnCondition->isChecked())
    {
        return ui->textEditCondition->toPlainText();
    }
    else
    {
        return QString("");
    }
}

void ScriptConditionConfigForm::SetRunModeArgs(const QString &args)
{
    if(ui->rbtnOnPeriod->isChecked())
    {
        ui->editPeriod->setText(args);
    }
    else if(ui->rbtnOnCondition->isChecked())
    {
        ui->textEditCondition->setText(args);
    }
    else
    {

    }
}


void ScriptConditionConfigForm::on_btnOk_clicked()
{
    if(ui->editScriptName->text() == tr("新建脚本"))
    {
        QMessageBox::critical(0, "错误", "脚本名称不能为：新建脚本");
        return;
    }
    this->accept();
}

void ScriptConditionConfigForm::on_btnCancel_clicked()
{
    this->reject();
}

void ScriptConditionConfigForm::on_btnEdit_clicked()
{
    ScriptRunConditionEditorDlg  *pDlg = new ScriptRunConditionEditorDlg(m_strProjectPath, this);
    pDlg->setWindowTitle(tr("条件运行属性编辑"));
    pDlg->setConditionString(ui->textEditCondition->toPlainText());
    if(pDlg->exec() == QDialog::Accepted)
    {
        ui->textEditCondition->setText(pDlg->getConditionString());
    }
    delete pDlg;
}
