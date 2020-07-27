#include "GetWidthHeightDialog.h"
#include "ui_GetWidthHeightDialog.h"

GetWidthHeightDialog::GetWidthHeightDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetWidthHeightDialog),
    iWidth_(0),
    iHeight_(0)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->spinBoxWidth->setRange(0, 65536);
    ui->spinBoxHeight->setRange(0, 65536);
}

GetWidthHeightDialog::~GetWidthHeightDialog()
{
    delete ui;
}

void GetWidthHeightDialog::on_btnOk_clicked()
{
    iWidth_ = ui->spinBoxWidth->value();
    iHeight_ = ui->spinBoxHeight->value();
    this->accept();
}

void GetWidthHeightDialog::on_btnCancel_clicked()
{
    this->reject();
}


void GetWidthHeightDialog::setWidth(int iWidth)
{
    iWidth_ = iWidth;
    ui->spinBoxWidth->setValue(iWidth);
}

int GetWidthHeightDialog::getWidth()
{
    return iWidth_;
}

void GetWidthHeightDialog::setHeight(int iHeight)
{
    iHeight_ = iHeight;
    ui->spinBoxHeight->setValue(iHeight);
}

int GetWidthHeightDialog::getHeight()
{
    return iHeight_;
}
