#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "VersionInfo.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->lblVersion->setText(QString("工程管理器 V%1").arg(VER_FILE));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_btnOk_clicked()
{
    this->accept();
}
