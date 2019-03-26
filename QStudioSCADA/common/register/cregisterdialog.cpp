#include "cregisterdialog.h"
#include "ui_cregisterdialog.h"
#include <QMessageBox>

#include "cregister.h"

CRegisterDialog::CRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRegisterDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("注册工具");

    QString usercode = CRegister::GetCPUId().c_str();
    ui->edt_usercode2->setText(usercode);
    ui->btn_close->hide();
}

CRegisterDialog::~CRegisterDialog()
{
    delete ui;
}

int CRegisterDialog::GeneratorDialog()
{
    CRegisterDialog dlg;
    dlg.ui->content->setCurrentIndex(0);
    return dlg.exec();
}

int CRegisterDialog::RegisterDialog()
{
    CRegisterDialog dlg;
    dlg.ui->content->setCurrentIndex(1);
    return dlg.exec();
}

void CRegisterDialog::on_btn_generate_clicked()
{
    QString usercode = ui->edt_usercode1->text();
    QString validdt = ui->dt_validdate->dateTime().toString("yyyyMMddhhmmss");

    if (usercode.isEmpty())
    {
        ui->lb_tips1->setText("无效用户码!");
        return;
    }

    std::string code = CRegister::GengerateCode(usercode.toStdString(), validdt.toStdString());
    ui->edt_registercode1->setText(code.c_str());
    ui->lb_tips1->setText("注册码生成成功!");
}

void CRegisterDialog::on_btn_register_clicked()
{
    QString code = ui->edt_registercode2->text();

    bool ret = CRegister::AuthRegisterCode(code.toStdString());
    if (!ret)
    {
        ui->lb_tips2->setText("无效注册码!");
        return;
    }

    ui->lb_tips2->setText("注册授权成功!");
}

void CRegisterDialog::on_btn_close_clicked()
{
    accept();
}
