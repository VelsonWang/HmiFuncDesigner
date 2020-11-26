#include "TagIOEditDialog.h"
#include "ui_TagIOEditDialog.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "IDevicePlugin.h"
#include "ProjectData.h"
#include <QStringList>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QPluginLoader>
#include <QDebug>

TagIOEditDialog::TagIOEditDialog(QString projName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagIOEditDialog),
    m_szProjectName(projName),
    devPlugin_(Q_NULLPTR)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->tabWidget->setCurrentIndex(0);

    // 已经建立设备列表
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        ui->cboDeviceName->addItem(pObj->szDeviceName_);
    }
    if(ui->cboDeviceName->count() > 0) {
        ui->cboDeviceName->setCurrentIndex(0);
    }

    ui->tabBaseLayout->addStretch();

    ui->spinBoxCreateTagNum->setRange(1, 32767);
    ui->spinBoxCreateTagNum->setValue(1);
    ui->spinBoxRegAddrOffset->setRange(1, 100);
    ui->spinBoxRegAddrOffset->setValue(1);
}

TagIOEditDialog::~TagIOEditDialog()
{
    delete ui;
}


void TagIOEditDialog::on_btnOk_clicked()
{
    bool ok = false;
    m_iCreateTagNum = ui->spinBoxCreateTagNum->value();
    m_iAddrOffset = ui->spinBoxRegAddrOffset->value();
    m_szTagName = ui->editTagName->text();
    m_szTagDesc = ui->editTagDescription->text();
    m_szDeviceName = ui->cboDeviceName->currentText();
    m_szDeviceAddr = ui->editDeviceAddress->text();
    m_szRegSection = ui->cboRegisterSection->currentText();
    m_szRegAddr = ui->editRegisterAddress->text();
    m_szRegAddrOffset = ui->editAddressOffset->text();
    m_szTypeReadWrite = ui->cboTypeReadWrite->currentText();
    m_szDataType = ui->cboIODataType->currentText();
    m_szTagInitValue = ui->editIOInitValue->text();
    m_szTagMinValue = ui->editIOMinValue->text();
    m_szTagMaxValue = ui->editIOMaxValue->text();
    m_szScale = ui->editIOScale->text();

    int iRegAddr = m_szRegAddr.toInt(&ok);
    if(!ok)
        iRegAddr = -1;

    ok = false;
    int iOffset = m_szRegAddrOffset.toInt(&ok);
    if(!ok)
        iOffset = -1;

    quint32 lowerLimit;
    quint32 upperLimit;
    QString area = ui->cboRegisterSection->currentText();
    if(devPlugin_ != Q_NULLPTR) {
        DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
        DeviceInfoObject *pObj = deviceInfo.getObjectByName(m_szDeviceName);
        QVector<QPair<QString, QString>> m_properties;
        if(pObj != Q_NULLPTR) {
            if(pObj->szProperties_ == "") {
                devPlugin_->getDefaultDeviceProperty(m_properties);
            } else {
                //devPlugin_->devicePropertiesFromString(pObj->szProperties_, m_properties);
            }
        }
        // 设置设备属性
        devPlugin_->setDeviceProperty(m_properties);
        // 获取寄存器区地址的下限和上限
        //devPlugin_->GetRegisterAreaLimit(area, lowerLimit, upperLimit);
        if((iRegAddr + iOffset) < lowerLimit || (iRegAddr + iOffset) > upperLimit) {
            QMessageBox::warning(this,
                                 tr("提示"),
                                 QString(tr("地址设置不在范围[%1,%2]"))
                                 .arg(QString::number(lowerLimit))
                                 .arg(QString::number(upperLimit)));
            return;
        }

    }
    accept();
}

void TagIOEditDialog::on_btnCancel_clicked()
{
    reject();
}


/**
 * @brief TagIOEditDialog::on_cboDeviceName_currentTextChanged
 * @details 设备名称改变
 * @param deviceName 设备名称
 */
void TagIOEditDialog::on_cboDeviceName_currentTextChanged(const QString &deviceName) {
    QString pluginNmae = deviceName;
    if(pluginNmae.indexOf("_") >= 0)
        pluginNmae = pluginNmae.left(pluginNmae.indexOf("_"));

    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cdUp();
    pluginsDir.cd("deviceplugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
#ifdef Q_OS_WIN
        QString szFileEndWith = QString(".dll");
#endif

#ifdef Q_OS_LINUX
        QString szFileEndWith = QString(".so");
#endif
        if(fileName.indexOf(pluginNmae) != -1 && fileName.endsWith(szFileEndWith)) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDevicePlugin *iDevPlugin = qobject_cast<IDevicePlugin *>(plugin);
                devPlugin_ = iDevPlugin;
                if (iDevPlugin) {
                    // 获取设备支持的所有寄存器区
                    //QStringList listReg = iDevPlugin->GetDeviceSupportRegisterArea();
                    ui->cboRegisterSection->clear();
                    //ui->cboRegisterSection->addItems(listReg);

                    // 获取设备支持的所有数据类型
                    //QStringList listType = iDevPlugin->GetDeviceSupportDataType();
                    ui->cboIODataType->clear();
                    //ui->cboIODataType->addItems(listType);
                } else {
                    QMessageBox::critical(this,
                                          tr("系统错误"),
                                          tr("插件加载失败！\n") + fileName);
                }
            }
        }
    }
}

QString TagIOEditDialog::scale() const
{
    return m_szScale;
}

void TagIOEditDialog::setScale(const QString &szScale)
{
    m_szScale = szScale;
    ui->editIOScale->setText(szScale);
}

QString TagIOEditDialog::tagMaxValue()
{
    return m_szTagMaxValue;
}

void TagIOEditDialog::setTagMaxValue(const QString &szMax)
{
    m_szTagMaxValue = szMax;
    ui->editIOMaxValue->setText(szMax);
}

QString TagIOEditDialog::tagMinValue()
{
    return m_szTagMinValue;
}

void TagIOEditDialog::setTagMinValue(const QString &szMin)
{
    m_szTagMinValue = szMin;
    ui->editIOMinValue->setText(szMin);
}

QString TagIOEditDialog::tagInitValue()
{
    return m_szTagInitValue;
}

void TagIOEditDialog::setTagInitValue(const QString &szInit)
{
    m_szTagInitValue = szInit;
    ui->editIOInitValue->setText(szInit);
}

QString TagIOEditDialog::dataType()
{
    return m_szDataType;
}

void TagIOEditDialog::setDataType(const QString &szType)
{
    m_szDataType = szType;
    ui->cboIODataType->setCurrentText(szType);
}

QString TagIOEditDialog::typeReadWrite()
{
    return m_szTypeReadWrite;
}

void TagIOEditDialog::setTypeReadWrite(const QString &szType)
{
    m_szTypeReadWrite = szType;
    ui->cboTypeReadWrite->setCurrentText(szType);
}

QString TagIOEditDialog::regAddrOffset()
{
    return m_szRegAddrOffset;
}

void TagIOEditDialog::setRegAddrOffset(const QString &szOffset)
{
    m_szRegAddrOffset = szOffset;
    ui->editAddressOffset->setText(szOffset);
}

QString TagIOEditDialog::regAddr()
{
    return m_szRegAddr;
}

void TagIOEditDialog::setRegAddr(const QString &szRegAddr)
{
    m_szRegAddr = szRegAddr;
    ui->editRegisterAddress->setText(szRegAddr);
}

QString TagIOEditDialog::regSection()
{
    return m_szRegSection;
}

void TagIOEditDialog::setRegSection(const QString &szSect)
{
    m_szRegSection = szSect;
    ui->cboRegisterSection->setCurrentText(szSect);
}

QString TagIOEditDialog::deviceAddr()
{
    return m_szDeviceAddr;
}

void TagIOEditDialog::setDeviceAddr(const QString &szAddr)
{
    m_szDeviceAddr = szAddr;
    ui->editDeviceAddress->setText(szAddr);
}

QString TagIOEditDialog::deviceName()
{
    return m_szDeviceName;
}

void TagIOEditDialog::setDeviceName(const QString &szName)
{
    m_szDeviceName = szName;
    ui->cboDeviceName->setCurrentText(szName);
}

QString TagIOEditDialog::tagDesc()
{
    return m_szTagDesc;
}

void TagIOEditDialog::setTagDesc(const QString &szDesc)
{
    m_szTagDesc = szDesc;
    ui->editTagDescription->setText(szDesc);
}

QString TagIOEditDialog::tagName()
{
    return m_szTagName;
}

void TagIOEditDialog::setTagName(const QString &szName)
{
    m_szTagName = szName;
    ui->editTagName->setText(szName);
}

int TagIOEditDialog::addrOffset()
{
    return m_iAddrOffset;
}

void TagIOEditDialog::setAddrOffset(int iOffset)
{
    m_iAddrOffset = iOffset;
    ui->spinBoxRegAddrOffset->setValue(iOffset);
}

int TagIOEditDialog::createTagNum()
{
    return m_iCreateTagNum;
}

void TagIOEditDialog::setCreateTagNum(int iNum)
{
    m_iCreateTagNum = iNum;
    ui->spinBoxCreateTagNum->setValue(iNum);
}

void TagIOEditDialog::hideCreateNumUI()
{
    ui->groupBoxCreateNum->hide();
}
