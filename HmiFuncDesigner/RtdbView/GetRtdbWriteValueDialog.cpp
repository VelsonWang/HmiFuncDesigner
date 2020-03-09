#include "GetRtdbWriteValueDialog.h"
#include "ui_GetRtdbWriteValueDialog.h"
#include <QMessageBox>


GetRtdbWriteValueDialog::GetRtdbWriteValueDialog(TagItem *tag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetRtdbWriteValueDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    m_sWriteValue = "";

    QString info = "变量信息:\n";
    info += QString("变量ID:%1\n").arg(tag->mId);
    info += QString("名称:%1\n").arg(tag->mName);
    info += QString("设备信息:%1\n").arg(tag->mDescription);
    ui->lblTagInfo->setText(info);
}

GetRtdbWriteValueDialog::~GetRtdbWriteValueDialog()
{
    delete ui;
}

void GetRtdbWriteValueDialog::on_btnOk_clicked()
{
    m_sWriteValue = ui->lineEditWriteValue->text();
    if(m_sWriteValue == "")
    {
        QMessageBox::critical(this, tr("错误"), tr("该值无效!\n请输入正确值！"));
        return;
    }
    this->accept();
}

void GetRtdbWriteValueDialog::on_btnCancel_clicked()
{
    this->reject();
}

QString GetRtdbWriteValueDialog::GetWriteValue()
{
    return m_sWriteValue;
}



