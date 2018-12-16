#include "DatabaseSettingDialog.h"
#include "ui_DatabaseSettingDialog.h"
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>

DatabaseSettingDialog::DatabaseSettingDialog(QWidget *parent, QString ProjectPath) :
    QDialog(parent),
    ui(new Ui::DatabaseSettingDialog),
    m_ProjectPath(ProjectPath)
{
    ui->setupUi(this);
    loadFromFile(DATA_SAVE_FORMAT);
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
    if(check_data())
    {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void DatabaseSettingDialog::on_btnOk_clicked()
{
    if(check_data())
    {
        saveToFile(DATA_SAVE_FORMAT);
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
    if(ui->editAlarmSize->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("数据库警告设置错误！"));
        ret = false;
    }

    ok = false;
    int iDataKeepDays = ui->editDataKeepDays->text().toInt(&ok);
    iDataKeepDays = iDataKeepDays;
    if(ui->editDataKeepDays->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("数据保留设置错误！"));
        ret = false;
    }

    ok = false;
    int iSavePeriod = ui->editSavePeriod->text().toInt(&ok);
    iSavePeriod = iSavePeriod;
    if(ui->editSavePeriod->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("存盘周期设置错误！"));
        ret = false;
    }

    ok = false;
    int iSendPeriod = ui->editSendPeriod->text().toInt(&ok);
    iSendPeriod = iSendPeriod;
    if(ui->editSendPeriod->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("指令周期设置错误！"));
        ret = false;
    }

    ok = false;
    int iStartTime = ui->editStartTime->text().toInt(&ok);
    iStartTime = iStartTime;
    if(ui->editStartTime->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("存盘启动设置错误！"));
        ret = false;
    }

    ok = false;
    int iPort = ui->editPort->text().toInt(&ok);
    iPort = iPort;
    if(ui->editPort->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("端口设置错误！"));
        ret = false;
    }
    return ret;
}


void DatabaseSettingDialog::load(const QJsonObject &json)
{
    ui->editAlarmSize->setText(json["iAlarmSize"].toString());
    ui->checkSpecialDB->setChecked(json["bSpecialDB"].toBool());
    ui->checkAutoDelete->setChecked(json["bAutoDelete"].toBool());
    ui->cboDBType->setCurrentText(json["sDBType"].toString());
    ui->editDataKeepDays->setText(json["iDataKeepDays"].toString());
    ui->editIPAddress->setText(json["sIPAddress"].toString());
    ui->checkUseSD->setChecked(json["bUseSD"].toBool());
    ui->editUser->setText(json["sUser"].toString());
    ui->editSavePeriod->setText(json["iSavePeriod"].toString());
    ui->editPassword->setText(json["sPassword"].toString());
    ui->editSendPeriod->setText(json["iSendPeriod"].toString());
    ui->editDBName->setText(json["sDBName"].toString());
    ui->editStartTime->setText(json["iStartTime"].toString());
    ui->editPort->setText(json["iPort"].toString());
}

void DatabaseSettingDialog::save(QJsonObject &json)
{
    json["iAlarmSize"] = ui->editAlarmSize->text();
    json["bSpecialDB"] = ui->checkSpecialDB->isChecked();
    json["bAutoDelete"] = ui->checkAutoDelete->isChecked();
    json["sDBType"] = ui->cboDBType->currentText();
    json["iDataKeepDays"] = ui->editDataKeepDays->text();
    json["sIPAddress"] = ui->editIPAddress->text();
    json["bUseSD"] = ui->checkUseSD->isChecked();
    json["sUser"] = ui->editUser->text();
    json["iSavePeriod"] = ui->editSavePeriod->text();
    json["sPassword"] = ui->editPassword->text();
    json["iSendPeriod"] = ui->editSendPeriod->text();
    json["sDBName"] = ui->editDBName->text();
    json["iStartTime"] = ui->editStartTime->text();
    json["iPort"] = ui->editPort->text();
}

bool DatabaseSettingDialog::loadFromFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/db.odb";
    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    load(loadDoc.object());
    loadFile.close();
    return true;
}

bool DatabaseSettingDialog::saveToFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/bd.odb";
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject obj;
    save(obj);
    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
    return true;
}
















