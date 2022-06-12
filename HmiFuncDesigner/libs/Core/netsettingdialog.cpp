#include "netsettingdialog.h"
#include "ui_netsettingdialog.h"
#include "qsoftcore.h"
#include "../shared/qprojectcore.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>

NetSettingDialog::NetSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetSettingDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
}

NetSettingDialog::~NetSettingDialog()
{
    delete ui;
}

void NetSettingDialog::on_btnHelp_clicked()
{
    // this time we do nothing!
}

bool NetSettingDialog::check_data()
{
    bool ret = true;

    bool ok = false;
    int iHeartbeatTime = ui->editHeartbeatTime->text().toInt(&ok);
    if(ui->editHeartbeatTime->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("心跳探测间隔设置错误！"));
        ret = false;
    }

    ok = false;
    int iDatabaseSyncTime = ui->editDatabaseSyncTime->text().toInt(&ok);
    if(ui->editDatabaseSyncTime->text().isEmpty() || !ok) {
        QMessageBox::information(this, tr("系统提示"), tr("数据库同步时间设置错误！"));
        ret = false;
    }

    return ret;
}

void NetSettingDialog::on_btnCheck_clicked()
{
    if(check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NetSettingDialog::on_btnOk_clicked()
{
    if(check_data()) {
        QDialog::accept();
    }
}

void NetSettingDialog::on_btnExit_clicked()
{
    QDialog::reject();
}

void NetSettingDialog::load()
{
    NetSetting &netSetting = QSoftCore::getCore()->getProjectCore()->m_netSetting;
    ui->checkHotStandbyMode->setChecked(netSetting.isHotStandbyMode());
    ui->checkClientMode->setChecked(netSetting.isClientMode());
    ui->rtbnServerStation->setChecked(netSetting.isServerStation());
    ui->rtbnClientStation->setChecked(netSetting.isClientStation());
    ui->editClientAddress->setIPString(netSetting.getClientAddress());
    ui->editServerAddress->setIPString(netSetting.getServerAddress());
    ui->editHeartbeatTime->setText(QString::number(netSetting.getHeartbeatTime()));
    ui->editDatabaseSyncTime->setText(QString::number(netSetting.getDatabaseSyncTime()));
}

void NetSettingDialog::save()
{
    NetSetting &netSetting = QSoftCore::getCore()->getProjectCore()->m_netSetting;
    netSetting.setHotStandbyMode(ui->checkHotStandbyMode->isChecked());
    netSetting.setClientMode(ui->checkClientMode->isChecked());
    netSetting.setServerStation(ui->rtbnServerStation->isChecked());
    netSetting.setClientStation(ui->rtbnClientStation->isChecked());
    netSetting.setClientAddress(ui->editClientAddress->getIPString());
    netSetting.setServerAddress(ui->editServerAddress->getIPString());
    netSetting.setHeartbeatTime(ui->editHeartbeatTime->text().toInt());
    netSetting.setDatabaseSyncTime(ui->editDatabaseSyncTime->text().toInt());
}



