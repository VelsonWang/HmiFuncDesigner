#include "DatabaseSettingDialog.h"
#include "ui_DatabaseSettingDialog.h"
#include "ProjectData.h"
#include <QMessageBox>


DatabaseSettingDialog::DatabaseSettingDialog(QWidget *parent, QString ProjectPath) :
    QDialog(parent),
    ui(new Ui::DatabaseSettingDialog),
    m_ProjectPath(ProjectPath)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
}

DatabaseSettingDialog::~DatabaseSettingDialog()
{
    delete ui;
}

void DatabaseSettingDialog::on_btnHelp_clicked()
{
    // this time we do nothing!
}

void DatabaseSettingDialog::on_btnCheck_clicked()
{
    if(check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void DatabaseSettingDialog::on_btnOk_clicked()
{
    if(check_data()) {
        QDialog::accept();
    }
}

void DatabaseSettingDialog::on_btnExit_clicked()
{
    QDialog::reject();
}

bool DatabaseSettingDialog::check_data()
{
    bool ret = true;

    bool ok = false;
    int iAlarmSize = ui->editAlarmSize->text().toInt(&ok);
    iAlarmSize = iAlarmSize;  // avoid warning!
    if(ui->editAlarmSize->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("数据库警告设置错误！"));
        ret = false;
    }

    ok = false;
    int iDataKeepDays = ui->editDataKeepDays->text().toInt(&ok);
    iDataKeepDays = iDataKeepDays;
    if(ui->editDataKeepDays->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("数据保留设置错误！"));
        ret = false;
    }

    ok = false;
    int iSavePeriod = ui->editSavePeriod->text().toInt(&ok);
    iSavePeriod = iSavePeriod;
    if(ui->editSavePeriod->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("存盘周期设置错误！"));
        ret = false;
    }

    ok = false;
    int iSendPeriod = ui->editSendPeriod->text().toInt(&ok);
    iSendPeriod = iSendPeriod;
    if(ui->editSendPeriod->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("指令周期设置错误！"));
        ret = false;
    }

    ok = false;
    int iStartTime = ui->editStartTime->text().toInt(&ok);
    iStartTime = iStartTime;
    if(ui->editStartTime->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("存盘启动设置错误！"));
        ret = false;
    }

    ok = false;
    int iPort = ui->editPort->text().toInt(&ok);
    iPort = iPort;
    if(ui->editPort->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("端口设置错误！"));
        ret = false;
    }
    return ret;
}


void DatabaseSettingDialog::load()
{
    DatabaseSetting &dbSetting = ProjectData::getInstance()->dbSetting_;
    ui->editAlarmSize->setText(QString::number(dbSetting.getAlarmSize()));
    ui->checkSpecialDB->setChecked(dbSetting.isSpecialDB());
    ui->checkAutoDelete->setChecked(dbSetting.isAutoDelete());
    ui->cboDBType->setCurrentText(dbSetting.getDBType());
    ui->editDataKeepDays->setText(QString::number(dbSetting.getDataKeepDays()));
    ui->editIPAddress->setText(dbSetting.getIPAddress());
    ui->checkUseSD->setChecked(dbSetting.isUseSD());
    ui->editUser->setText(dbSetting.getUserName());
    ui->editSavePeriod->setText(QString::number(dbSetting.getSavePeriod()));
    ui->editPassword->setText(dbSetting.getPassword());
    ui->editSendPeriod->setText(QString::number(dbSetting.getSendPeriod()));
    ui->editDBName->setText(dbSetting.getDBName());
    ui->editStartTime->setText(QString::number(dbSetting.getStartTime()));
    ui->editPort->setText(QString::number(dbSetting.getPort()));
}

void DatabaseSettingDialog::save()
{
    DatabaseSetting &dbSetting = ProjectData::getInstance()->dbSetting_;
    dbSetting.setAlarmSize(ui->editAlarmSize->text().toInt());
    dbSetting.setSpecialDB(ui->checkSpecialDB->isChecked());
    dbSetting.setAutoDelete(ui->checkAutoDelete->isChecked());
    dbSetting.setDBType(ui->cboDBType->currentText());
    dbSetting.setDataKeepDays(ui->editDataKeepDays->text().toInt());
    dbSetting.setIPAddress(ui->editIPAddress->text());
    dbSetting.setUseSD(ui->checkUseSD->isChecked());
    dbSetting.setUserName(ui->editUser->text());
    dbSetting.setSavePeriod(ui->editSavePeriod->text().toInt());
    dbSetting.setPassword(ui->editPassword->text());
    dbSetting.setSendPeriod(ui->editSendPeriod->text().toInt());
    dbSetting.setDBName(ui->editDBName->text());
    dbSetting.setStartTime(ui->editStartTime->text().toInt());
    dbSetting.setPort(ui->editPort->text().toInt());
}














