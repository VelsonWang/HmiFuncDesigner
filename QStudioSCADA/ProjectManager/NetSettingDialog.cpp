#include "NetSettingDialog.h"
#include "ui_NetSettingDialog.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

NetSettingDialog::NetSettingDialog(QWidget *parent, QString ProjectPath) :
    QDialog(parent),
    ui(new Ui::NetSettingDialog),
    m_ProjectPath(ProjectPath)
{
    ui->setupUi(this);
    loadFromFile(DATA_SAVE_FORMAT);

    QRegExp rx("((2[0-4]//d|25[0-5]|[01]?//d//d?)//.){3}(2[0-4]//d|25[0-5]|[01]?//d//d?)");
    QRegExpValidator v(rx, 0);
    ui->editClientAddress->setValidator(&v);
    ui->editClientAddress->setInputMask("000.000.000.000;0");
    ui->editServerAddress->setValidator(&v);
    ui->editServerAddress->setInputMask("000.000.000.000;0");
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
    if(ui->editHeartbeatTime->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("心跳探测间隔设置错误！"));
        ret = false;
    }

    ok = false;
    int iDatabaseSyncTime = ui->editDatabaseSyncTime->text().toInt(&ok);
    if(ui->editDatabaseSyncTime->text().isEmpty() || !ok)
    {
        QMessageBox::information(this, tr("系统提示"), tr("数据库同步时间设置错误！"));
        ret = false;
    }

    return ret;
}

void NetSettingDialog::on_btnCheck_clicked()
{
    if(check_data())
    {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NetSettingDialog::on_btnOk_clicked()
{
    if(check_data())
    {
        saveToFile(DATA_SAVE_FORMAT);
        QDialog::accept();
    }
}

void NetSettingDialog::on_btnExit_clicked()
{
    QDialog::reject();
}

void NetSettingDialog::load(const QJsonObject &json)
{
    ui->checkHotStandbyMode->setChecked(json["bHotStandbyMode"].toBool());
    ui->checkClientMode->setChecked(json["bClientMode"].toBool());
    ui->rtbnServerStation->setChecked(json["bServerStation"].toBool());
    ui->rtbnClientStation->setChecked(json["bClientStation"].toBool());
    ui->editClientAddress->setText(json["sClientAddress"].toString());
    ui->editServerAddress->setText(json["sServerAddress"].toString());
    ui->editHeartbeatTime->setText(json["sHeartbeatTime"].toString());
    ui->editDatabaseSyncTime->setText(json["sDatabaseSyncTime"].toString());
}

void NetSettingDialog::save(QJsonObject &json)
{
    json["bHotStandbyMode"] = ui->checkHotStandbyMode->isChecked();
    json["bClientMode"] = ui->checkClientMode->isChecked();
    json["bServerStation"] = ui->rtbnServerStation->isChecked();
    json["bClientStation"] = ui->rtbnClientStation->isChecked();
    json["sClientAddress"] = ui->editClientAddress->text();
    json["sServerAddress"] = ui->editServerAddress->text();
    json["sHeartbeatTime"] = ui->editHeartbeatTime->text();
    json["sDatabaseSyncTime"] = ui->editDatabaseSyncTime->text();
}

bool NetSettingDialog::loadFromFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/net.odb";
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

bool NetSettingDialog::saveToFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/net.odb";
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













