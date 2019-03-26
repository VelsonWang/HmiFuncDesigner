#include "NewProjectDialog.h"
#include "ui_NewProjectDialog.h"
#include "configutils.h"
#include "Singleton.h"
#include "ProjectInfoManger.h"
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
        deviceMap_.insert(slistDev.at(i),value);
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
        ProjectInfoManger &projMgr = Singleton<ProjectInfoManger>::instance();
        projMgr.setProjectName(ui->editProjectName->text());
        projMgr.setProjectDescription(ui->editProjectDescription->toPlainText());
        projMgr.setProjectPath(ui->editProjectPath->text());
        projMgr.setDeviceType(ui->cboDevType->currentText());
        projMgr.setStationNumber(ui->editStationNumber->text());
        projMgr.setStartPage(ui->editStartPage->text());
        projMgr.setStationAddress(ui->editStationAddress->text());
        projMgr.setProjectEncrypt(ui->chkProjectEncrypt->isChecked());
        projMgr.setPageScanPeriod(ui->editPageScanPeriod->text());
        projMgr.setDataScanPeriod(ui->editDataScanPeriod->text());

        QString strProjPath =  ui->editProjectPath->text();
        QString strProjName = ui->editProjectName->text();
        QString file = strProjPath + "/" + strProjName + "/" + strProjName + ".proj";
        projectName_ = file;
        projMgr.saveToFile(strProjPath, strProjName, DATA_SAVE_FORMAT);
        QDialog::accept();
    }
}

void NewProjectDialog::on_btnExit_clicked()
{    
    QDialog::reject();
}


QString NewProjectDialog::GetProjectName()
{
    return projectName_;
}

bool NewProjectDialog::loadFromFile(SaveFormat saveFormat, const QString &file) {
    ProjectInfoManger &projMgr = Singleton<ProjectInfoManger>::instance();
    projMgr.loadFromFile(saveFormat, file);
    ui->editProjectName->setText(projMgr.getProjectName());
    ui->editProjectDescription->setPlainText(projMgr.getProjectDescription());
    ui->editProjectPath->setText(projMgr.getProjectPath());
    ui->cboDevType->setCurrentText(projMgr.getDeviceType());
    ui->editStationNumber->setText(projMgr.getStationNumber());
    ui->editStartPage->setText(projMgr.getStartPage());
    ui->editStationAddress->setText(projMgr.getStationAddress());
    ui->chkProjectEncrypt->setChecked(projMgr.getProjectEncrypt());
    ui->editPageScanPeriod->setText(projMgr.getPageScanPeriod());
    ui->editDataScanPeriod->setText(projMgr.getDataScanPeriod());
}


