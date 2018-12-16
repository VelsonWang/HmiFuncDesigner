#include "NewComDeviceDialog.h"
#include "ui_NewComDeviceDialog.h"
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
#include <QFile>
#include <QJsonDocument>
#include <QPluginLoader>
#include <QDebug>



NewComDeviceDialog::NewComDeviceDialog(QWidget *parent, QString ProjectPath) :
    QDialog(parent),
    ui(new Ui::NewComDeviceDialog),
    m_ProjectPath(ProjectPath)
{
    ui->setupUi(this);
    ui->editProtocol->setReadOnly(true);
    for(int idx=0; idx<32;idx++)
    {
        ui->cboPortNumber->addItem(QString("COM%1").arg(idx));
    }

    ui->cboBaudrate->addItem("115200");
    ui->cboBaudrate->addItem("57600");
    ui->cboBaudrate->addItem("38400");
    ui->cboBaudrate->addItem("19200");
    ui->cboBaudrate->addItem("9600");
    ui->cboBaudrate->addItem("4800");
    ui->cboBaudrate->addItem("2400");
    ui->cboBaudrate->addItem("1200");
    ui->cboBaudrate->addItem("600");
    ui->cboBaudrate->addItem("300");
    ui->cboBaudrate->setCurrentText("9600");

    ui->cboDatabit->addItem("8");
    ui->cboDatabit->addItem("7");
    ui->cboDatabit->addItem("6");
    ui->cboDatabit->addItem("5");
    ui->cboDatabit->setCurrentText("8");

    ui->cboStopbit->addItem("1");
    ui->cboStopbit->addItem("1.5");
    ui->cboStopbit->addItem("2");
    ui->cboStopbit->setCurrentText("1");

    ui->cboVerifybit->addItem("无校验");
    ui->cboVerifybit->addItem("奇校验");
    ui->cboVerifybit->addItem("偶校验");
    ui->cboVerifybit->setCurrentText("无校验");

    ui->editTimeout->setText("50");

    m_dev.m_sDeviceType = "COM";
}

NewComDeviceDialog::~NewComDeviceDialog()
{
    delete ui;
}

void NewComDeviceDialog::on_btnHelp_clicked()
{

}

/*
* 设备选择
*/
void NewComDeviceDialog::on_btnDeviceSelect_clicked()
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
void NewComDeviceDialog::on_btnProtocolSelect_clicked()
{
    QString pluginNmae = ui->editDeviceName->text();
    if(pluginNmae.indexOf("_") >= 0)
        pluginNmae = pluginNmae.left(pluginNmae.indexOf("_"));

    QDir pluginsDir(ConfigUtils::AppDir());
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
void NewComDeviceDialog::SetListDeviceName(QStringList l)
{
    this->m_ListDeviceName = l;
}

void NewComDeviceDialog::on_btnCheck_clicked()
{
    if(check_data())
    {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewComDeviceDialog::on_btnOk_clicked()
{
    if(check_data())
    {
        saveToFile(DATA_SAVE_FORMAT);
        QDialog::accept();
    }
}

void NewComDeviceDialog::on_btnExit_clicked()
{
    QDialog::reject();
}



bool NewComDeviceDialog::check_data()
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

void NewComDeviceDialog::load(const QJsonObject &json)
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
    ui->cboPortNumber->setCurrentText(json["sPortNumber"].toString());
    m_dev.m_sPortNumber = json["sPortNumber"].toString();
    ui->cboBaudrate->setCurrentText(json["iBaudrate"].toString());
    m_dev.m_iBaudrate = json["iBaudrate"].toString().toInt();
    ui->cboDatabit->setCurrentText(json["iDatabit"].toString());
    m_dev.m_iDatabit = json["iDatabit"].toString().toInt();
    ui->cboStopbit->setCurrentText(json["fStopbit"].toString());
    m_dev.m_fStopbit = json["iStopbit"].toString().toFloat();
    ui->cboVerifybit->setCurrentText(json["sVerifybit"].toString());
    m_dev.m_sVerifybit = json["sVerifybit"].toString();
    ui->editTimeout->setText(json["iTimeout"].toString());
    m_dev.m_iTimeout = json["iTimeout"].toString().toInt();
}

void NewComDeviceDialog::save(QJsonObject &json)
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
    json["sPortNumber"] = ui->cboPortNumber->currentText();
    m_dev.m_sPortNumber = ui->cboPortNumber->currentText();
    json["iBaudrate"] = ui->cboBaudrate->currentText();
    m_dev.m_iBaudrate = ui->cboBaudrate->currentText().toInt();
    json["iDatabit"] = ui->cboDatabit->currentText();
    m_dev.m_iDatabit = ui->cboDatabit->currentText().toInt();
    json["fStopbit"] = ui->cboStopbit->currentText();
    m_dev.m_fStopbit = ui->cboStopbit->currentText().toFloat();
    json["sVerifybit"] = ui->cboVerifybit->currentText();
    m_dev.m_sVerifybit = ui->cboVerifybit->currentText();
    json["iTimeout"] = ui->editTimeout->text();
    m_dev.m_iTimeout = ui->editTimeout->text().toInt();
}

bool NewComDeviceDialog::loadFromFile(SaveFormat saveFormat, QString file)
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

bool NewComDeviceDialog::saveToFile(SaveFormat saveFormat)
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

ComDevice* NewComDeviceDialog::GetComDevice()
{
    return &m_dev;
}

QString NewComDeviceDialog::GetDeviceName() const
{
    return ui->editDeviceName->text();
}



