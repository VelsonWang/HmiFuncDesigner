#include "NewNetDeviceDialog.h"
#include "ui_NewNetDeviceDialog.h"
#include "DeviceListDialog.h"
#include "SelectProtocolDialog.h"
#include "IDevicePlugin.h"
#include "configutils.h"
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include "Helper.h"
#include <QFile>
#include <QJsonDocument>
#include <QPluginLoader>
#include <QDebug>



NewNetDeviceDialog::NewNetDeviceDialog(QWidget *parent, QString ProjectPath) :
    QDialog(parent),
    ui(new Ui::NewNetDeviceDialog),
    m_ProjectPath(ProjectPath)
{
    ui->setupUi(this);
    ui->editProtocol->setReadOnly(true);

    ui->editIpAddress->setText(QString("localhost"));
    ui->editPort->setText(QString("0"));
    ui->editIpAddress1->setText(QString("0"));
    ui->editPort1->setText(QString("0"));

    m_dev.m_sDeviceType = "NET";
}

NewNetDeviceDialog::~NewNetDeviceDialog()
{
    delete ui;
}

void NewNetDeviceDialog::on_btnHelp_clicked()
{

}

/*
* 设备选择
*/
void NewNetDeviceDialog::on_btnDeviceSelect_clicked()
{
    DeviceListDialog *pDlg = new DeviceListDialog(m_dev.m_sDeviceType, this);
    if(pDlg->exec() == QDialog::Accepted)
    {
        QString fileName = m_ProjectPath + "/" + ui->editDeviceName->text().trimmed() + ".odb";
        QFile file(fileName);
        if(file.exists())
            file.remove();
        QString devName = pDlg->GetDeviceName();

        // 查找相同的设备名称
        int findCnt = 0;
continueFind:
        for(int i=0; i<this->m_ListDeviceName.count(); i++)
        {
            if(this->m_ListDeviceName.at(i) == devName)
            {
                findCnt++;
                devName = pDlg->GetDeviceName() + QString("_%1").arg(findCnt);
                goto continueFind;
            }
        }

        ui->editDeviceName->setText(devName);
    }
}

/*
* 协议选择
*/
void NewNetDeviceDialog::on_btnProtocolSelect_clicked()
{
    QString pluginNmae = ui->editDeviceName->text();
    if(pluginNmae.indexOf("_") >= 0)
        pluginNmae = pluginNmae.left(pluginNmae.indexOf("_"));

    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cdUp();
    pluginsDir.cd("deviceplugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        if(fileName.indexOf(pluginNmae) == -1)
            continue;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        qDebug()<<pluginsDir.absoluteFilePath(fileName);
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            IDevicePlugin *iDevPlugin = qobject_cast<IDevicePlugin *>(plugin);
            if (iDevPlugin)
            {
                SelectProtocolDialog *pDlg = new SelectProtocolDialog(this);
                pDlg->SetProtocolList(iDevPlugin->GetDeviceSupportProtocol());
                if(pDlg->exec() == QDialog::Accepted)
                {
                    ui->editProtocol->setText(pDlg->GetProtocolName());
                }
            }
            else
            {
                QMessageBox::critical(this, tr("系统错误"), tr("插件加载失败！\n") + fileName);
            }
        }
    }

}

/*
* 设置已经建立的设备名称列表
*/
void NewNetDeviceDialog::SetListDeviceName(QStringList l)
{
    this->m_ListDeviceName = l;
}

void NewNetDeviceDialog::on_btnCheck_clicked()
{
    if(check_data())
    {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewNetDeviceDialog::on_btnOk_clicked()
{
    if(check_data())
    {
        saveToFile(DATA_SAVE_FORMAT);
        QDialog::accept();
    }
}

void NewNetDeviceDialog::on_btnExit_clicked()
{
    QDialog::reject();
}



bool NewNetDeviceDialog::check_data()
{
    bool ret = true;
#if 0
    if(ui->editProjectName->text().isEmpty())
    {
        QMessageBox::information(this, tr("系统提示"), tr(""));
        ret = false;
    }
#endif
    return ret;
}

void NewNetDeviceDialog::load(const QJsonObject &json)
{
    ui->editDeviceName->setText(json["sDeviceName"].toString());
    m_sOldDeviceName = ui->editDeviceName->text();
    m_dev.m_sDeviceName = json["sDeviceName"].toString();
    ui->editFrameLen->setText(json["iFrameLen"].toString());
    m_dev.m_iFrameLen = json["iFrameLen"].toString().toInt();
    ui->editProtocol->setText(json["sProtocol"].toString());
    m_dev.m_sProtocol = json["sProtocol"].toString();
    ui->cboLink->setCurrentText(json["sLink"].toString());
    m_dev.m_sLink = json["sLink"].toString();
    ui->editStateVar->setText(json["iStateVar"].toString());
    m_dev.m_iStateVar = json["iStateVar"].toString().toInt();
    ui->editFrameTimePeriod->setText(json["iFrameTimePeriod"].toString());
    m_dev.m_iFrameTimePeriod = json["iFrameTimePeriod"].toString().toInt();
    ui->editCtrlVar->setText(json["iCtrlVar"].toString());
    m_dev.m_iCtrlVar = json["iCtrlVar"].toString().toInt();
    ui->checkDynamicOptimization->setChecked(json["bDynamicOptimization"].toBool());
    m_dev.m_bDynamicOptimization = json["bDynamicOptimization"].toBool();
    ui->editRemotePort->setText(json["iRemotePort"].toString());
    m_dev.m_iRemotePort = json["iRemotePort"].toString().toInt();    
    ui->editIpAddress->setText(json["ipAddress"].toString());
    m_dev.m_sIpAddress = json["ipAddress"].toString();
    ui->editPort->setText(json["iPort"].toString());
    m_dev.m_iPort = json["iPort"].toString().toInt();
    ui->editIpAddress1->setText(json["ipAddress1"].toString());
    m_dev.m_sIpAddress1 = json["ipAddress1"].toString();
    ui->editPort1->setText(json["iPort1"].toString());
    m_dev.m_iPort1 = json["iPort1"].toString().toInt();
}

void NewNetDeviceDialog::save(QJsonObject &json)
{
    json["sDeviceName"] = ui->editDeviceName->text();
    m_dev.m_sDeviceName = ui->editDeviceName->text();
    json["iFrameLen"] = ui->editFrameLen->text();
    m_dev.m_iFrameLen = ui->editFrameLen->text().toInt();
    json["sProtocol"] = ui->editProtocol->text();
    m_dev.m_sProtocol = ui->editProtocol->text();
    json["sLink"] = ui->cboLink->currentText();
    m_dev.m_sLink = ui->cboLink->currentText();
    json["iStateVar"] = ui->editStateVar->text();
    m_dev.m_iStateVar = ui->editStateVar->text().toInt();
    json["iFrameTimePeriod"] = ui->editFrameTimePeriod->text();
    m_dev.m_iFrameTimePeriod = ui->editFrameTimePeriod->text().toInt();
    json["iCtrlVar"] = ui->editCtrlVar->text();
    m_dev.m_iCtrlVar = ui->editCtrlVar->text().toInt();
    json["bDynamicOptimization"] = ui->checkDynamicOptimization->isChecked();
    m_dev.m_bDynamicOptimization = ui->checkDynamicOptimization->isChecked();
    json["iRemotePort"] = ui->editRemotePort->text();
    m_dev.m_iRemotePort = ui->editRemotePort->text().toInt();

    json["ipAddress"] = ui->editIpAddress->text();
    m_dev.m_sIpAddress = ui->editIpAddress->text();
    json["iPort"] = ui->editPort->text();
    m_dev.m_iPort = ui->editPort->text().toInt();
    json["ipAddress1"] = ui->editIpAddress1->text();
    m_dev.m_sIpAddress1 = ui->editIpAddress1->text();
    json["iPort1"] = ui->editPort1->text();
    m_dev.m_iPort1 = ui->editPort1->text().toInt();
}

bool NewNetDeviceDialog::loadFromFile(SaveFormat saveFormat, QString file)
{
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

bool NewNetDeviceDialog::saveToFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/" + ui->editDeviceName->text().trimmed() + ".odb";
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

    if(m_sOldDeviceName != ui->editDeviceName->text())
    {
        QString oldfile = m_ProjectPath + "/" + m_sOldDeviceName.trimmed() + ".odb";
        QFile oldFile(oldfile);
        if(oldFile.exists())
            oldFile.remove();
    }
    return true;
}

NetDevice* NewNetDeviceDialog::GetNetDevice()
{
    return &m_dev;
}

QString NewNetDeviceDialog::GetDeviceName() const
{
    return ui->editDeviceName->text();
}



