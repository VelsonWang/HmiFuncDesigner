#include "NewComDeviceDialog.h"
#include "ui_NewComDeviceDialog.h"
#include "DeviceListDialog.h"
#include "SelectProtocolDialog.h"
#include "IDevicePlugin.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "ProjectData.h"
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QPluginLoader>
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "DevicePluginLoader.h"

NewComDeviceDialog::NewComDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewComDeviceDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->editProtocol->setReadOnly(true);
    for(int idx=1; idx<33;idx++)
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

    m_dev.szDeviceType_ = "COM";

    VariantManager *pVariantManager  = new VariantManager(this);
    m_pVariantPropertyManager = pVariantManager;

    connect(m_pVariantPropertyManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(onPropertyValueChanged(QtProperty *, const QVariant &)));

    m_pVariantEditorFactory = new VariantFactory(this);

    m_pPropertyEditor = new QtTreePropertyBrowser(ui->tabPropertySetting);
    m_pPropertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pPropertyEditor->setHeaderLabels(QStringList() << tr("属性") << tr("值"));
    m_pPropertyEditor->setFactoryForManager(m_pVariantPropertyManager, m_pVariantEditorFactory);
    pVariantManager->setPropertyEditor(m_pPropertyEditor);
    ui->vLayoutPropertyEditor->addWidget(m_pPropertyEditor);

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
    DeviceListDialog *pDlg = new DeviceListDialog(m_dev.szDeviceType_, this);
    if(pDlg->exec() == QDialog::Accepted) {
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

        QString pluginName = devName;
        IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pluginName);
        if (pDevPluginObj) {
            pDevPluginObj->getDefaultDeviceProperty(m_properties);
            pDevPluginObj->getDefaultDevicePropertyDataType(m_prop_type);
        }
        this->updatePropertyEditor();
    }
}

/*
* 协议选择
*/
void NewComDeviceDialog::on_btnProtocolSelect_clicked()
{
    QString pluginName = ui->editDeviceName->text();
    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pluginName);
    if (pDevPluginObj) {
        SelectProtocolDialog *pDlg = new SelectProtocolDialog(this);
        pDlg->SetProtocolList(pDevPluginObj->GetDeviceSupportProtocol());
        if(pDlg->exec() == QDialog::Accepted) {
            ui->editProtocol->setText(pDlg->GetProtocolName());
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
    if(check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewComDeviceDialog::on_btnOk_clicked()
{
    if(check_data()) {
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

ComDevice* NewComDeviceDialog::GetComDevice()
{
    return &m_dev;
}

QString NewComDeviceDialog::GetDeviceName() const
{
    return ui->editDeviceName->text();
}

void NewComDeviceDialog::load(int id)
{
    if(id < 0) return;

    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);
    DeviceInfoObject *pObj = deviceInfo.getDeviceInfoObjectByID(id);

    ui->editDeviceName->setText(pObj->szDeviceName_);
    m_dev.szDeviceName_ = pObj->szDeviceName_;
    ui->editFrameLen->setText(QString::number(pObj->iFrameLen_));
    m_dev.iFrameLen_ = pObj->iFrameLen_;
    ui->editProtocol->setText(pObj->szProtocol_);
    m_dev.szProtocol_ = pObj->szProtocol_;
    ui->cboLink->setCurrentText(pObj->szLink_);
    m_dev.szLink_ = pObj->szLink_;
    ui->editStateVar->setText(QString::number(pObj->iStateVar_));
    m_dev.iStateVar_ = pObj->iStateVar_;
    ui->editFrameTimePeriod->setText(QString::number(pObj->iFrameTimePeriod_));
    m_dev.iFrameTimePeriod_ = pObj->iFrameTimePeriod_;
    ui->editCtrlVar->setText(QString::number(pObj->iCtrlVar_));
    m_dev.iCtrlVar_ = pObj->iCtrlVar_;
    ui->checkDynamicOptimization->setChecked(pObj->bDynamicOptimization_);
    m_dev.bDynamicOptimization_ = pObj->bDynamicOptimization_;
    ui->editRemotePort->setText(QString::number(pObj->iRemotePort_));
    m_dev.iRemotePort_ = pObj->iRemotePort_;

    ComDevice comDev;
    comDev.fromString(pObj->szPortParameters_);
    ui->cboPortNumber->setCurrentText(comDev.szPortNumber_);
    m_dev.szPortNumber_ = comDev.szPortNumber_;
    ui->cboBaudrate->setCurrentText(QString::number(comDev.iBaudrate_));
    m_dev.iBaudrate_ = comDev.iBaudrate_;
    ui->cboDatabit->setCurrentText(QString::number(comDev.iDatabit_));
    m_dev.iDatabit_ = comDev.iDatabit_;
    ui->cboStopbit->setCurrentText(QString::number(comDev.fStopbit_));
    m_dev.fStopbit_ = comDev.fStopbit_;
    ui->cboVerifybit->setCurrentText(comDev.szVerifybit_);
    m_dev.szVerifybit_ = comDev.szVerifybit_;
    ui->editTimeout->setText(QString::number(comDev.iTimeout_));
    m_dev.iTimeout_ = comDev.iTimeout_;

    QString pluginName = pObj->szDeviceName_;
    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pluginName);
    if (pDevPluginObj) {
        if(pObj->szProperties_ == "") {
            pDevPluginObj->getDefaultDeviceProperty(m_properties);
        } else {
            pDevPluginObj->devicePropertiesFromString(pObj->szProperties_, m_properties);
        }
        pDevPluginObj->getDefaultDevicePropertyDataType(m_prop_type);
    }
    this->updatePropertyEditor();
}


void NewComDeviceDialog::save(int id)
{
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    DeviceInfoObject *pObj = deviceInfo.getDeviceInfoObjectByID(id);
    if(pObj == nullptr) {
        pObj = new DeviceInfoObject();
        if(pObj == nullptr)
            return;
        deviceInfo.insert(ProjectData::getInstance()->dbData_, pObj);
        pObj->iID_ = deviceInfo.getLastInsertId(ProjectData::getInstance()->dbData_);
    }
    pObj->szDeviceType_ = m_dev.szDeviceType_;
    pObj->szDeviceName_ = ui->editDeviceName->text();
    m_dev.szDeviceName_ = ui->editDeviceName->text();
    pObj->iFrameLen_ = ui->editFrameLen->text().toInt();
    m_dev.iFrameLen_ = ui->editFrameLen->text().toInt();
    pObj->szProtocol_ = ui->editProtocol->text();
    m_dev.szProtocol_ = ui->editProtocol->text();
    pObj->szLink_ = ui->cboLink->currentText();
    m_dev.szLink_ = ui->cboLink->currentText();
    pObj->iStateVar_ = ui->editStateVar->text().toInt();
    pObj->iStateVar_ = ui->editStateVar->text().toInt();
    pObj->iFrameTimePeriod_ = ui->editFrameTimePeriod->text().toInt();
    m_dev.iFrameTimePeriod_ = ui->editFrameTimePeriod->text().toInt();
    pObj->iCtrlVar_ = ui->editCtrlVar->text().toInt();
    m_dev.iCtrlVar_ = ui->editCtrlVar->text().toInt();
    pObj->bDynamicOptimization_ = ui->checkDynamicOptimization->isChecked();
    m_dev.bDynamicOptimization_ = ui->checkDynamicOptimization->isChecked();
    pObj->iRemotePort_ = ui->editRemotePort->text().toInt();
    m_dev.iRemotePort_ = ui->editRemotePort->text().toInt();

    ComDevice comDev;
    comDev.szPortNumber_ = ui->cboPortNumber->currentText();
    m_dev.szPortNumber_ = ui->cboPortNumber->currentText();
    comDev.iBaudrate_ = ui->cboBaudrate->currentText().toInt();
    m_dev.iBaudrate_ = ui->cboBaudrate->currentText().toInt();
    comDev.iDatabit_ = ui->cboDatabit->currentText().toInt();
    m_dev.iDatabit_ = ui->cboDatabit->currentText().toInt();
    comDev.fStopbit_ = ui->cboStopbit->currentText().toFloat();
    m_dev.fStopbit_ = ui->cboStopbit->currentText().toFloat();
    comDev.szVerifybit_ = ui->cboVerifybit->currentText();
    m_dev.szVerifybit_ = ui->cboVerifybit->currentText();
    comDev.iTimeout_ = ui->editTimeout->text().toInt();
    m_dev.iTimeout_ = ui->editTimeout->text().toInt();

    pObj->szPortParameters_ = comDev.toString();

    QString pluginName = pObj->szDeviceName_;
    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pluginName);
    if (pDevPluginObj) {
        pObj->szProperties_ = pDevPluginObj->devicePropertiesToString(m_properties);
    }

    deviceInfo.update(ProjectData::getInstance()->dbData_, pObj);
}


void NewComDeviceDialog::addProperty(QtVariantProperty *property, const QString &id, bool bAddToList)
{
    if(bAddToList) {
        m_listProp.append(property);
    }
    m_propertyToId[property] = id;
    m_idToProperty[id] = property;
}


void NewComDeviceDialog::clearProperties()
{
    QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
    while (itProp != m_propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    m_propertyToId.clear();
    m_idToProperty.clear();
}

void NewComDeviceDialog::createPropertyList()
{
    m_listProp.clear();
    clearProperties();

    QtVariantProperty *property = Q_NULLPTR;
    if(m_properties.count() != m_prop_type.count())
        return;

    for (int i = 0; i < m_properties.size(); ++i) {
        QString szKey = m_properties[i].first;
        QString szValue = m_properties[i].second;
        QString szType = m_prop_type[i].second;
        if(szType == QString("int")) {
            property = m_pVariantPropertyManager->addProperty(QVariant::Int, szKey);
            property->setAttribute(QLatin1String("minimum"), 0);
            property->setAttribute(QLatin1String("maximum"), 99999);
            addProperty(property, szKey);
        }
        else if(szType == QString("bool")) {
            property = m_pVariantPropertyManager->addProperty(QVariant::Bool, szKey);
            addProperty(property, szKey);
        }
    }
}

QString NewComDeviceDialog::getValue2ByValue1(const QString &szVal1,
                                              QVector<QPair<QString, QString>>& properties)
{
    for (int i = 0; i < properties.size(); ++i) {
        if (properties[i].first == szVal1) {
            return properties[i].second;
        }
    }
    return "";
}


void NewComDeviceDialog::setValue2ByValue1(const QString &szVal1,
                                           const QString &szVal2,
                                           QVector<QPair<QString, QString>>& properties)
{
    for (int i = 0; i < properties.size(); ++i) {
        if (properties[i].first == szVal1) {
            properties[i].second = szVal2;
        }
    }
}


void NewComDeviceDialog::onPropertyValueChanged(QtProperty *property, const QVariant &value)
{
    if(m_properties.count() != m_prop_type.count())
        return;

    QString id = m_propertyToId[property];
    QString szType = getValue2ByValue1(id, m_prop_type);
    if(szType == QString("int")) {
        setValue2ByValue1(id, value.toString(), m_properties);
    }
    else if(szType == QString("bool")) {
        bool val = value.toBool();
        QString szVal = val ? "true" : "false";
        setValue2ByValue1(id, szVal, m_properties);
    }
}

void NewComDeviceDialog::updatePropertyModel()
{
    QtVariantProperty *property = Q_NULLPTR;

    for (int i = 0; i < m_properties.size(); ++i) {
        QString szKey = m_properties[i].first;
        QString szValue = m_properties[i].second;
        property = m_idToProperty[szKey];
        if(property != Q_NULLPTR) {
            property->setValue(szValue);
        }
    }
}

///
/// \brief NewComDeviceDialog::updatePropertyEditor
/// \details 更新PropertyEditor数据
///
void NewComDeviceDialog::updatePropertyEditor()
{
    createPropertyList();
    updatePropertyModel();

    QListIterator<QtProperty*> iter(m_listProp);
    while (iter.hasNext()) {
        m_pPropertyEditor->addProperty(iter.next());
    }
}





