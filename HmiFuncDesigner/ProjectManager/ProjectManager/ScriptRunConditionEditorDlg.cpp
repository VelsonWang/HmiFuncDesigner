#include "ScriptRunConditionEditorDlg.h"
#include "ui_ScriptRunConditionEditorDlg.h"
#include "InsertTagDialog.h"
#include <QDebug>

ScriptRunConditionEditorDlg::ScriptRunConditionEditorDlg(QString projectPath, QWidget *parent) :
    QDialog(parent),
    m_strProjectPath(projectPath),
    m_strCondition(""),
    ui(new Ui::ScriptRunConditionEditorDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
}

ScriptRunConditionEditorDlg::~ScriptRunConditionEditorDlg()
{
    delete ui;
}

void ScriptRunConditionEditorDlg::on_btnVarConst_clicked()
{
    InsertTagDialog *pDlg = new InsertTagDialog(m_strProjectPath, this);
    if(pDlg->exec() == QDialog::Accepted)
    {
        ui->editVarConst->setText(pDlg->getSelectedTagName());
    }
    delete pDlg;
}

void ScriptRunConditionEditorDlg::on_btnVarLeft_clicked()
{
    InsertTagDialog *pDlg = new InsertTagDialog(m_strProjectPath, this);
    if(pDlg->exec() == QDialog::Accepted)
    {
        ui->editVarVarLeft->setText(pDlg->getSelectedTagName());
    }
    delete pDlg;
}

void ScriptRunConditionEditorDlg::on_btnVarRight_clicked()
{
    InsertTagDialog *pDlg = new InsertTagDialog(m_strProjectPath, this);
    if(pDlg->exec() == QDialog::Accepted)
    {
        ui->editVarVarRight->setText(pDlg->getSelectedTagName());
    }
    delete pDlg;
}

void ScriptRunConditionEditorDlg::on_btnOk_clicked()
{
    if(ui->radioBtnVarConst->isChecked())
        m_strCondition = ui->editVarConst->text() + ui->cboVarConst->currentText() + ui->editValConst->text();
    else
        m_strCondition = ui->editVarVarLeft->text() + ui->cboVarVar->currentText() + ui->editVarVarRight->text();

    this->accept();
}

void ScriptRunConditionEditorDlg::on_btnCancel_clicked()
{
    this->reject();
}


QString ScriptRunConditionEditorDlg::getConditionString() const
{
    return this->m_strCondition;
}

void ScriptRunConditionEditorDlg::setConditionString(QString condition)
{
    QString optStr = "";
    if(condition.indexOf("==") != -1)
    {
        optStr = "==";
        goto dosplit;
    }
    if(condition.indexOf(">=") != -1)
    {
        optStr = ">=";
        goto dosplit;
    }
    if(condition.indexOf(">=") == -1 && condition.indexOf(">") != -1)
    {
        optStr = ">";
        goto dosplit;
    }
    if(condition.indexOf("<=") != -1)
    {
        optStr = "<=";
        goto dosplit;
    }
    if(condition.indexOf("<=") == -1 && condition.indexOf("<") != -1)
    {
        optStr = "<";
        goto dosplit;
    }

dosplit:
    QStringList listOpt = condition.split(optStr);
//    for (int i = 0; i < listOpt.size(); ++i)
//        qDebug() << listOpt.at(i);
    if(listOpt.size() != 2)
        return;
    QString leftOpt = listOpt.at(0);
    QString rightOpt = listOpt.at(1);
    if(rightOpt.indexOf("变量") != -1)
    {
        ui->radioBtnVarVar->setChecked(true);
        ui->radioBtnVarConst->setChecked(false);
        ui->editVarVarLeft->setText(leftOpt);
        ui->cboVarVar->setCurrentText(optStr);
        ui->editVarVarRight->setText(rightOpt);
    }
    else
    {
        ui->radioBtnVarConst->setChecked(true);
        ui->radioBtnVarVar->setChecked(false);
        ui->editVarConst->setText(leftOpt);
        ui->cboVarConst->setCurrentText(optStr);
        ui->editValConst->setText(rightOpt);
    }
}

void ScriptRunConditionEditorDlg::on_radioBtnVarConst_clicked(bool checked)
{
    Q_UNUSED(checked)
    ui->radioBtnVarVar->setChecked(false);
}

void ScriptRunConditionEditorDlg::on_radioBtnVarVar_clicked(bool checked)
{
    Q_UNUSED(checked)
    ui->radioBtnVarConst->setChecked(false);
}
