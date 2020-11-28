#include "NewNetDeviceDialog.h"
#include "ui_NewNetDeviceDialog.h"
#include "DeviceListDialog.h"
#include "SelectProtocolDialog.h"
#include "IDevicePlugin.h"
#include "ConfigUtils.h"
#include "ProjectData.h"
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include "Helper.h"
#include <QFile>
#include <QPluginLoader>
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "DevicePluginLoader.h"
#include <QDebug>



NewNetDeviceDialog::NewNetDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewNetDeviceDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->editProtocol->setReadOnly(true);

    ui->editIpAddress->setText(QString("localhost"));
    ui->editPort->setText(QString("0"));
    ui->editIpAddress1->setText(QString("0"));
    ui->editPort1->setText(QString("0"));

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
    DeviceListDialog *pDlg = new DeviceListDialog("NET", this);
    if(pDlg->exec() == QDialog::Accepted)
    {
        QString devName = pDlg->GetDeviceName();
        m_szPluginName = devName;
        // 查找相同的设备名称
        int findCnt = 0;
        DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
continueFind:
        for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
            DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
            if(pObj->szName_ == devName) {
                findCnt++;
                devName = pDlg->GetDeviceName() + QString("_%1").arg(findCnt);
                goto continueFind;
            }
        }

        ui->editDeviceName->setText(devName);
        IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(m_szPluginName);
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
void NewNetDeviceDialog::on_btnProtocolSelect_clicked()
{
    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(m_szPluginName);
    if (pDevPluginObj) {
        SelectProtocolDialog *pDlg = new SelectProtocolDialog(this);
        QString szDeviceDescInfo = pDevPluginObj->getDeviceDescInfo();
        XMLObject xmlObj;
        if(xmlObj.load(szDeviceDescInfo, Q_NULLPTR)) {
            pDlg->SetProtocolList(xmlObj.getProperty("SupportProtocol").split("|"));
            if(pDlg->exec() == QDialog::Accepted) {
                ui->editProtocol->setText(pDlg->GetProtocolName());
            }
        }
    }
}


void NewNetDeviceDialog::on_btnCheck_clicked()
{
    if(check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewNetDeviceDialog::on_btnOk_clicked()
{
    if(check_data()) {
        QDialog::accept();
    }
}

void NewNetDeviceDialog::on_btnExit_clicked() {
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


QString NewNetDeviceDialog::GetDeviceName() const {
    return ui->editDeviceName->text();
}


void NewNetDeviceDialog::load(const QString &szName)
{
    if(szName == "") return;
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    DeviceInfoObject *pObj = deviceInfo.getObjectByName(szName);
    if(pObj == Q_NULLPTR) return;
    ui->editDeviceName->setText(pObj->szName_);
    m_szPluginName = pObj->szDeviceName_;
    ui->editFrameLen->setText(QString::number(pObj->iFrameLen_));
    ui->editProtocol->setText(pObj->szProtocol_);
    ui->cboLink->setCurrentText(pObj->szLink_);
    ui->editStateVar->setText(QString::number(pObj->iStateVar_));
    ui->editFrameTimePeriod->setText(QString::number(pObj->iFrameTimePeriod_));
    ui->editCtrlVar->setText(QString::number(pObj->iCtrlVar_));
    ui->checkDynamicOptimization->setChecked(pObj->bDynamicOptimization_);
    ui->editRemotePort->setText(QString::number(pObj->iRemotePort_));

    NetDevice netDev;
    netDev.fromString(pObj->szPortParameters_);
    ui->editIpAddress->setText(netDev.szIpAddress_);
    ui->editPort->setText(QString::number(netDev.iPort_));
    ui->editIpAddress1->setText(netDev.szIpAddress1_);
    ui->editPort1->setText(QString::number(netDev.iPort1_));

    QString pluginName = pObj->szDeviceName_;
    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pluginName);
    if (pDevPluginObj) {
        if(pObj->szProperties_ == "") {
            pDevPluginObj->getDefaultDeviceProperty(m_properties);
        } else {
            pDevPluginObj->readProperties(pObj->szProperties_, m_properties);
        }
        pDevPluginObj->getDefaultDevicePropertyDataType(m_prop_type);
    }
    this->updatePropertyEditor();
}

void NewNetDeviceDialog::save(const QString &szName)
{
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    DeviceInfoObject *pObj = deviceInfo.getObjectByName(szName);

    if(pObj == Q_NULLPTR) {
        pObj = deviceInfo.newObject();
        if(pObj == Q_NULLPTR) return;
    }

    pObj->szDeviceType_ = "NET";
    pObj->szName_ = ui->editDeviceName->text();
    pObj->szDeviceName_ = m_szPluginName;
    pObj->iFrameLen_ = ui->editFrameLen->text().toInt();
    pObj->szProtocol_ = ui->editProtocol->text();
    pObj->szLink_ = ui->cboLink->currentText();
    pObj->iStateVar_ = ui->editStateVar->text().toInt();
    pObj->iFrameTimePeriod_ = ui->editFrameTimePeriod->text().toInt();
    pObj->iCtrlVar_ = ui->editCtrlVar->text().toInt();
    pObj->bDynamicOptimization_ = ui->checkDynamicOptimization->isChecked();
    pObj->iRemotePort_ = ui->editRemotePort->text().toInt();

    NetDevice netDev;
    netDev.szIpAddress_ = ui->editIpAddress->text();
    netDev.iPort_ = ui->editPort->text().toInt();
    netDev.szIpAddress1_ = ui->editIpAddress1->text();
    netDev.iPort1_ = ui->editPort1->text().toInt();

    pObj->szPortParameters_ = netDev.toString();

    QString pluginName = pObj->szDeviceName_;
    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pluginName);
    if (pDevPluginObj) {
        pDevPluginObj->writeProperties(pObj->szProperties_, m_properties);
    }
}


void NewNetDeviceDialog::addProperty(QtVariantProperty *property, const QString &id, bool bAddToList)
{
    if(bAddToList) m_listProp.append(property);
    m_propertyToId[property] = id;
    m_idToProperty[id] = property;
}


void NewNetDeviceDialog::clearProperties()
{
    QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
    while (itProp != m_propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    m_propertyToId.clear();
    m_idToProperty.clear();
}

void NewNetDeviceDialog::createPropertyList()
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

QString NewNetDeviceDialog::getValue2ByValue1(const QString &szVal1,
                                              QVector<QPair<QString, QString>>& properties)
{
    for (int i = 0; i < properties.size(); ++i) {
        if (properties[i].first == szVal1) {
            return properties[i].second;
        }
    }
    return "";
}


void NewNetDeviceDialog::setValue2ByValue1(const QString &szVal1,
                                           const QString &szVal2,
                                           QVector<QPair<QString, QString>>& properties)
{
    for (int i = 0; i < properties.size(); ++i) {
        if (properties[i].first == szVal1) {
            properties[i].second = szVal2;
        }
    }
}


void NewNetDeviceDialog::onPropertyValueChanged(QtProperty *property, const QVariant &value)
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

void NewNetDeviceDialog::updatePropertyModel()
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
/// \brief NewNetDeviceDialog::updatePropertyEditor
/// \details 更新PropertyEditor数据
///
void NewNetDeviceDialog::updatePropertyEditor()
{
    createPropertyList();
    updatePropertyModel();

    QListIterator<QtProperty*> iter(m_listProp);
    while (iter.hasNext()) {
        m_pPropertyEditor->addProperty(iter.next());
    }
}







