#include "EDncryptToolDialog.h"
#include "ui_EDncryptToolDialog.h"
#include <QMessageBox>

EDncryptToolDialog::EDncryptToolDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EDncryptToolDialog)
{
    ui->setupUi(this);
    ui->lineEditKeyCode->clear();
    ui->lineEditInput->clear();
    ui->lineEditOutput->clear();
}

EDncryptToolDialog::~EDncryptToolDialog()
{
    delete ui;
}

/**
 * @brief EDncryptToolDialog::getEDncryptType
 * @details 获取用户选中的加密解密类型
 * @return 加密解密类型 AES, D3DES
 */
int EDncryptToolDialog::getEDncryptType()
{
    int retType = AES;

    QString strType = ui->cboType->currentText();
    if(strType == "AES") {
        retType = AES;
    } else if(strType == "D3DES") {
        retType = D3DES;
    }
    return retType;
}

/**
 * @brief EDncryptToolDialog::on_btnEncrypt_clicked
 * @details 加密lineEditInput文本，并在lineEditOutput显示
 */
void EDncryptToolDialog::on_btnEncrypt_clicked()
{
    QString strInput = ui->lineEditInput->text();
    if(strInput == "") {
        QMessageBox::information(this, tr("提示"), tr("输入不能为空！"));
        return;
    }

    QString strKeyCode = ui->lineEditKeyCode->text();
    if(strKeyCode == "") {
        QMessageBox::information(this, tr("提示"), tr("秘钥不能为空！"));
        return;
    }

    QString strOutput = EDncrypt::Encrypt(strInput, getEDncryptType(), strKeyCode);
    ui->lineEditOutput->setText(strOutput);
}


/**
 * @brief EDncryptToolDialog::on_btnDncrypt_clicked
 * @details 解密lineEditInput文本，并在lineEditOutput显示
 */
void EDncryptToolDialog::on_btnDncrypt_clicked()
{
    QString strInput = ui->lineEditInput->text();
    if(strInput == "") {
        QMessageBox::information(this, tr("提示"), tr("输入不能为空！"));
        return;
    }

    QString strKeyCode = ui->lineEditKeyCode->text();
    if(strKeyCode == "") {
        QMessageBox::information(this, tr("提示"), tr("秘钥不能为空！"));
        return;
    }

    QString strOutput = EDncrypt::Dncrypt(strInput, getEDncryptType(), strKeyCode);
    ui->lineEditOutput->setText(strOutput);
}
