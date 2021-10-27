#include "newnetdevicedialog.h"
#include "ui_newnetdevicedialog.h"
#include "devicelistdialog.h"
#include "selectprotocoldialog.h"
#include "../../Devices/IDevicePlugin/IDevicePlugin.h"
#include "../shared/confighelper.h"
#include "qsoftcore.h"
#include "../shared/qprojectcore.h"
#include "../shared/qpropertyfactory.h"
#include "../shared/property/qabstractproperty.h"
#include "./qpropertylist/qpropertylistview.h"
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QPluginLoader>
#include "devicepluginloader.h"
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

    m_propertyView = new QPropertyListView(this);
    connect(m_propertyView, SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)),
            this, SLOT(onPropertyEdit(QAbstractProperty*, QVariant)));

    ui->vLayoutPropertyEditor->addWidget(m_propertyView);
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
    if(pDlg->exec() == QDialog::Accepted) {
        QString devName = pDlg->GetDeviceName();
        m_szPluginName = devName;
        // 查找相同的设备名称
        int findCnt = 0;
        DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
continueFind:
        for(int i = 0; i < deviceInfo.listDeviceInfoObject_.count(); i++) {
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
        if(xmlObj.load(szDeviceDescInfo, NULL)) {
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

void NewNetDeviceDialog::on_btnExit_clicked()
{
    QDialog::reject();
}



bool NewNetDeviceDialog::check_data()
{
    bool ret = true;
#if 0
    if(ui->editProjectName->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr(""));
        ret = false;
    }
#endif
    return ret;
}


QString NewNetDeviceDialog::GetDeviceName() const
{
    return ui->editDeviceName->text();
}


void NewNetDeviceDialog::load(const QString &szName)
{
    if(szName == "") {
        return;
    }
    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    DeviceInfoObject *pObj = deviceInfo.getObjectByName(szName);
    if(pObj == NULL) {
        return;
    }
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
    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    DeviceInfoObject *pObj = deviceInfo.getObjectByName(szName);

    if(pObj == NULL) {
        pObj = deviceInfo.newObject();
        if(pObj == NULL) {
            return;
        }
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


///
/// \brief NewNetDeviceDialog::updatePropertyEditor
/// \details 更新PropertyEditor数据
///
void NewNetDeviceDialog::updatePropertyEditor()
{
    if(m_properties.count() != m_prop_type.count()) {
        return;
    }

    QList<QAbstractProperty *> listProperties;
    QAbstractProperty* pProObj = NULL;

    for (int i = 0; i < m_properties.size(); ++i) {
        QString szKey = m_properties[i].first;
        QString szValue = m_properties[i].second;
        QString szType = m_prop_type[i].second;

        if(szType == QString("int")) {
            pProObj = QPropertyFactory::create_property("Number");
            if(pProObj != NULL) {
                pProObj->setObjectProperty("name", szKey);
                pProObj->setAttribute("show_name", szKey);
                pProObj->setAttribute("group", "Attributes");
                pProObj->setAttribute(ATTR_CAN_SAME, true);
                QVariant val;
                val.setValue(szValue);
                pProObj->set_value(val);
                listProperties.append(pProObj);
            }
        } else if(szType == QString("bool")) {
            pProObj = QPropertyFactory::create_property("Bool");
            if(pProObj != NULL) {
                pProObj->setObjectProperty("name", szKey);
                pProObj->setAttribute("show_name", szKey);
                pProObj->setAttribute("group", "Attributes");
                pProObj->setAttribute(ATTR_CAN_SAME, true);
                QVariant val;
                val.setValue(szValue);
                pProObj->set_value(val);
                listProperties.append(pProObj);
            }
        }
    }

    m_propertyView->setPropertys(listProperties);
}





void NewNetDeviceDialog::onPropertyEdit(QAbstractProperty *pro, const QVariant &value)
{
    if(m_properties.count() != m_prop_type.count()) {
        return;
    }

    QString id = pro->getObjectProperty("name").toString();
    QString szType = getValue2ByValue1(id, m_prop_type);

    if(szType == QString("int")) {
        setValue2ByValue1(id, value.toString(), m_properties);
    } else if(szType == QString("bool")) {
        bool val = value.toBool();
        QString szVal = val ? "true" : "false";
        setValue2ByValue1(id, szVal, m_properties);
    }

    pro->set_value(value);
}



