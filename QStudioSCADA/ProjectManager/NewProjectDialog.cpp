#include "NewProjectDialog.h"
#include "ui_NewProjectDialog.h"
#include "configutils.h"
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);

    ui->editProjectName->setText("demo1");
    ui->editProjectDescription->setText("demo1 project");
    ui->editStationNumber->setText("0");
    ui->editStationAddress->setText("192.168.1.10");
    ui->editStartPage->setText("draw1.drw");
    ui->editPageScanPeriod->setText("500");
    ui->editDataScanPeriod->setText("500");

    /////////////////////////////////////////////////////

    QString iniDevFileName = QCoreApplication::applicationDirPath() + "/Config/device.ini";
    QFile fileDevCfg(iniDevFileName);
    if(!fileDevCfg.exists())
        QMessageBox::critical(this, tr("提示"), tr("设备选型配置文件不存在，您不能创建新工程！"));

    QSettings settingsDev(iniDevFileName, QSettings::IniFormat);
    QStringList slistDev;
    ConfigUtils::getCfgList(iniDevFileName, "DeviceSupportIndex", "dev", slistDev);

    settingsDev.beginGroup("DeviceID");
    for (int i=0; i< slistDev.count(); i++)
    {
        int value = settingsDev.value(slistDev.at(i)).toInt();
        m_DeviceMap.insert(slistDev.at(i),value);
        ui->cboDevType->insertItem(i, slistDev.at(i));
    }
    settingsDev.endGroup();
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::on_btnFileDialog_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择工程目录"), "C:/", QFileDialog::ShowDirsOnly);
    ui->editProjectPath->setText(dir);
}

void NewProjectDialog::on_btnHelp_clicked()
{
    // this time we do nothing!
}

bool NewProjectDialog::check_data()
{
    bool ret = true;

    QDir dir(ui->editProjectPath->text());
    if(!dir.exists() || ui->editProjectPath->text().isEmpty())
    {
        QMessageBox::information(this, tr("系统提示"), tr("输入路径错误！"));
        ret = false;
    }

    if(ui->editProjectName->text().isEmpty())
    {
        QMessageBox::information(this, tr("系统提示"), tr("工程名称输入错误！"));
        ret = false;
    }

    if(ui->editStationNumber->text().isEmpty())
    {
        QMessageBox::information(this, tr("系统提示"), tr("本站站号不能为空！"));
        ret = false;
    }

    if(ui->editStationAddress->text().isEmpty())
    {
        QMessageBox::information(this, tr("系统提示"), tr("本站地址不能为空！"));
        ret = false;
    }

    if(ui->editPageScanPeriod->text().isEmpty())
    {
        QMessageBox::information(this, tr("系统提示"), tr("画面刷新率不能为空！"));
        ret = false;
    }

    if(ui->editDataScanPeriod->text().isEmpty())
    {
        QMessageBox::information(this, tr("系统提示"), tr("数据刷新率不能为空！"));
        ret = false;
    }

    return ret;
}

void NewProjectDialog::on_btnCheck_clicked()
{
    if(check_data())
    {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewProjectDialog::on_btnOk_clicked()
{
    if(check_data())
    {
        saveToFile(DATA_SAVE_FORMAT);
        QDialog::accept();
    }
}

void NewProjectDialog::on_btnExit_clicked()
{    
    QDialog::reject();
}

void NewProjectDialog::load(const QJsonObject &json)
{
    ui->editProjectName->setText(json["sProjectName"].toString());
    ui->editProjectDescription->setPlainText(json["sProjectDescription"].toString());
    ui->editProjectPath->setText(json["sProjectPath"].toString());
    ui->cboDevType->setCurrentText(json["sDeviceType"].toString());
    ui->editStationNumber->setText(json["sStationNumber"].toString());
    ui->editStartPage->setText(json["sStartPage"].toString());
    ui->editStationAddress->setText(json["sStationAddress"].toString());
    ui->chkProjectEncrypt->setChecked(json["bProjectEncrypt"].toBool());
    ui->editPageScanPeriod->setText(json["sPageScanPeriod"].toString());
    ui->editDataScanPeriod->setText(json["sDataScanPeriod"].toString());
}

void NewProjectDialog::save(QJsonObject &json)
{
    json["sProjectName"] = ui->editProjectName->text();
    json["sProjectDescription"] = ui->editProjectDescription->toPlainText();
    json["sProjectPath"] = ui->editProjectPath->text();
    json["sDeviceType"] = ui->cboDevType->currentText();
    json["sStationNumber"] = ui->editStationNumber->text();
    json["sStartPage"] = ui->editStartPage->text();
    json["sStationAddress"] = ui->editStationAddress->text();
    json["bProjectEncrypt"] = ui->chkProjectEncrypt->isChecked();
    json["sPageScanPeriod"] = ui->editPageScanPeriod->text();
    json["sDataScanPeriod"] = ui->editDataScanPeriod->text();
}

bool NewProjectDialog::loadFromFile(SaveFormat saveFormat, QString file)
{
    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return false;
    }
    m_strProjectName = file;
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    load(loadDoc.object());
    loadFile.close();
    return true;
}

bool NewProjectDialog::saveToFile(SaveFormat saveFormat)
{
    QString strDir =  ui->editProjectPath->text();
    QDir dir(strDir);
    dir.mkpath(strDir + "/" + ui->editProjectName->text());
    QString file = strDir + "/" + ui->editProjectName->text() + "/" + ui->editProjectName->text() + ".proj";
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    m_strProjectName = file;
    QJsonObject obj;
    save(obj);
    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
    return true;
}

QString NewProjectDialog::GetProjectName()
{
    return m_strProjectName;
}
