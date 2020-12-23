#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSettings>
#include "qsoftcore.h"
#include "../shared/qprojectcore.h"


/**
 * @brief getCfgList 获取配置项目
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKeyName 键名称
 * @param strings 读取配置的数组值
 * @return 读取的个数
 */
static int getCfgList(QString sFileName, QString sSecName, QString sKeyName, QStringList &strings)
{
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);

    int iCount = settings.value(sKeyName + "-count").toInt();
    QString strValue;
    int iRet = 0;

    for(int i=0; i<iCount; i++) {
        strValue = settings.value(sKeyName + "-" + QString::number(i + 1)).toString();
        strValue = strValue.trimmed();
        if((strValue.length() != 0) && (strings.indexOf(strValue) < 0)) {
            strings << strValue;
            iRet ++;
        }
    }

    settings.endGroup();

    return iRet;
}


NewProjectDialog::NewProjectDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->editProjectName->setText("demo1");
    ui->editProjectDescription->setText("demo1 project");
    ui->editStationNumber->setText("0");
    ui->editStationAddress->setText("192.168.1.10");
    ui->chkProjectEncrypt->setChecked(true);

    ui->cboStartPage->clear();
    QStringList szGraphPageNameList;
    QSoftCore::getCore()->getProjectCore()->getAllGraphPageName(szGraphPageNameList);
    for (int i = 0; i < szGraphPageNameList.count(); i++) {
        QString strPageName = szGraphPageNameList.at(i);
        ui->cboStartPage->addItem(strPageName);
    }
    ui->cboStartPage->addItem("None");

    ui->editPageScanPeriod->setText("500");
    ui->editDataScanPeriod->setText("500");

    /////////////////////////////////////////////////////

    QString iniDevFileName = QCoreApplication::applicationDirPath() + "/Config/device.ini";
    QFile fileDevCfg(iniDevFileName);
    if (!fileDevCfg.exists())
        QMessageBox::critical(this,
                              tr("提示"),
                              tr("设备选型配置文件不存在，您不能创建新工程！"));

    QSettings settingsDev(iniDevFileName, QSettings::IniFormat);
    QStringList slistDev;
    getCfgList(iniDevFileName, "DeviceSupportIndex", "dev", slistDev);

    settingsDev.beginGroup("DeviceID");
    for (int i = 0; i < slistDev.count(); i++) {
        int value = settingsDev.value(slistDev.at(i)).toInt();
        deviceMap_.insert(slistDev.at(i), value);
        ui->cboDevType->insertItem(i, slistDev.at(i));
    }
    settingsDev.endGroup();
}

NewProjectDialog::~NewProjectDialog() {
    delete ui;
}

void NewProjectDialog::on_btnFileDialog_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("选择工程目录"),
                                                    "C:/",
                                                    QFileDialog::ShowDirsOnly);
    ui->editProjectPath->setText(dir);
}

void NewProjectDialog::on_btnHelp_clicked() {
    // this time we do nothing!
}

bool NewProjectDialog::check_data() {
    bool ret = true;

    QDir dir(ui->editProjectPath->text());
    if (!dir.exists() || ui->editProjectPath->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("输入路径错误！"));
        ret = false;
    }

    if (ui->editProjectName->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("工程名称输入错误！"));
        ret = false;
    }

    if (ui->editStationNumber->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("本站站号不能为空！"));
        ret = false;
    }

    if (ui->editStationAddress->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("本站地址不能为空！"));
        ret = false;
    }

    if (ui->editPageScanPeriod->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("画面刷新率不能为空！"));
        ret = false;
    }

    if (ui->editDataScanPeriod->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("数据刷新率不能为空！"));
        ret = false;
    }

    return ret;
}

void NewProjectDialog::on_btnCheck_clicked() {
    if (check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewProjectDialog::on_btnOk_clicked() {
    if (check_data()) {
        QString szProjPath = ui->editProjectPath->text();
        QSoftCore::getCore()->getProjectCore()->m_szProjPath = szProjPath;
        QString szProjName = ui->editProjectName->text();
        QSoftCore::getCore()->getProjectCore()->m_szProjName = szProjName;
        QString szFile = szProjPath + "/" + szProjName + ".pdt";
        QSoftCore::getCore()->getProjectCore()->m_szProjFile = szFile;
        QDialog::accept();
    }
}

void NewProjectDialog::on_btnExit_clicked() {
    QDialog::reject();
}


bool NewProjectDialog::load() {
    ProjectInfoManager &projInfoMgr = QSoftCore::getCore()->getProjectCore()->projInfoMgr_;
    projInfoMgr.setProjectName(QSoftCore::getCore()->getProjectCore()->m_szProjName);
    ui->editProjectName->setText(projInfoMgr.getProjectName());
    ui->editProjectDescription->setPlainText(projInfoMgr.getProjectDescription());
    ui->cboDevType->setCurrentText(projInfoMgr.getDeviceType());
    ui->editStationNumber->setText(QString::number(projInfoMgr.getStationNumber()));
    ui->cboStartPage->setCurrentText(projInfoMgr.getStartPage());
    ui->editStationAddress->setText(projInfoMgr.getStationAddress());
    ui->chkProjectEncrypt->setChecked(projInfoMgr.getProjectEncrypt());
    ui->editPageScanPeriod->setText(QString::number(projInfoMgr.getPageScanPeriod()));
    ui->editDataScanPeriod->setText(QString::number(projInfoMgr.getDataScanPeriod()));
    projInfoMgr.setProjectPath(QSoftCore::getCore()->getProjectCore()->m_szProjPath);
    ui->editProjectPath->setText(projInfoMgr.getProjectPath());
    return true;
}

bool NewProjectDialog::save() {
    ProjectInfoManager &projInfoMgr = QSoftCore::getCore()->getProjectCore()->projInfoMgr_;
    projInfoMgr.setProjectName(ui->editProjectName->text());
    projInfoMgr.setProjectDescription(ui->editProjectDescription->toPlainText());
    projInfoMgr.setProjectPath(ui->editProjectPath->text());
    projInfoMgr.setDeviceType(ui->cboDevType->currentText());
    projInfoMgr.setStationNumber(ui->editStationNumber->text().toInt());
    projInfoMgr.setStartPage(ui->cboStartPage->currentText());
    projInfoMgr.setStationAddress(ui->editStationAddress->text());
    projInfoMgr.setProjectEncrypt(ui->chkProjectEncrypt->isChecked());
    QSoftCore::getCore()->getProjectCore()->headerObj_.byEncrypt = ui->chkProjectEncrypt->isChecked() ? 1 : 0;
    projInfoMgr.setPageScanPeriod(ui->editPageScanPeriod->text().toInt());
    projInfoMgr.setDataScanPeriod(ui->editDataScanPeriod->text().toInt());
    return true;
}
