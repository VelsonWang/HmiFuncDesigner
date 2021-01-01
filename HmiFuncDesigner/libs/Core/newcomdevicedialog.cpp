#include "newcomdevicedialog.h"
#include "ui_newcomdevicedialog.h"
#include "devicelistdialog.h"
#include "selectprotocoldialog.h"
#include "../../Devices/IDevicePlugin/IDevicePlugin.h"
#include "../shared/confighelper.h"
#include "qsoftcore.h"
#include "../shared/qprojectcore.h"
#include "../shared/qpropertyfactory.h"
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QList>
#include <QPluginLoader>
#include "devicepluginloader.h"
#include <QDebug>

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

    ui->cboVerifybit->addItem("NONE");
    ui->cboVerifybit->addItem("ODD");
    ui->cboVerifybit->addItem("EVEN");
    ui->cboVerifybit->setCurrentText("NONE");

    ui->editTimeout->setText("50");

    m_propertyView = new QPropertyListView(this);
    connect(m_propertyView, SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)),
            this, SLOT(onPropertyEdit(QAbstractProperty*, QVariant)));

    ui->vLayoutPropertyEditor->addWidget(m_propertyView);
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
    DeviceListDialog *pDlg = new DeviceListDialog("COM", this);
    if(pDlg->exec() == QDialog::Accepted) {
        QString devName = pDlg->GetDeviceName();
        m_szPluginName = devName;
        // 查找相同的设备名称
        int findCnt = 0;
        DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
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
void NewComDeviceDialog::on_btnProtocolSelect_clicked()
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

void NewComDeviceDialog::on_btnCheck_clicked()
{
    if(check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewComDeviceDialog::on_btnOk_clicked() {
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

QString NewComDeviceDialog::GetDeviceName() const
{
    return ui->editDeviceName->text();
}

void NewComDeviceDialog::load(const QString &szName)
{
    if(szName == "") return;
    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
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

    ComDevice comDev;
    comDev.fromString(pObj->szPortParameters_);
    ui->cboPortNumber->setCurrentText(comDev.szPortNumber_);
    ui->cboBaudrate->setCurrentText(QString::number(comDev.iBaudrate_));
    ui->cboDatabit->setCurrentText(QString::number(comDev.iDatabit_));
    ui->cboStopbit->setCurrentText(QString::number(comDev.fStopbit_));
    ui->cboVerifybit->setCurrentText(comDev.szVerifybit_);
    ui->editTimeout->setText(QString::number(comDev.iTimeout_));

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


void NewComDeviceDialog::save(const QString &szName)
{
    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    DeviceInfoObject *pObj = deviceInfo.getObjectByName(szName);
    if(pObj == Q_NULLPTR) {
        pObj = deviceInfo.newObject();
        if(pObj == Q_NULLPTR) return;
    }
    pObj->szDeviceType_ = "COM";
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

    ComDevice comDev;
    comDev.szPortNumber_ = ui->cboPortNumber->currentText();
    comDev.iBaudrate_ = ui->cboBaudrate->currentText().toInt();
    comDev.iDatabit_ = ui->cboDatabit->currentText().toInt();
    comDev.fStopbit_ = ui->cboStopbit->currentText().toFloat();
    comDev.szVerifybit_ = ui->cboVerifybit->currentText();
    comDev.iTimeout_ = ui->editTimeout->text().toInt();

    pObj->szPortParameters_ = comDev.toString();

    QString pluginName = pObj->szDeviceName_;
    IDevicePlugin *pDevPluginObj = DevicePluginLoader::getInstance()->getPluginObject(pluginName);
    if (pDevPluginObj) {
        pDevPluginObj->writeProperties(pObj->szProperties_, m_properties);
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


///
/// \brief NewComDeviceDialog::updatePropertyEditor
/// \details 更新PropertyEditor数据
///
void NewComDeviceDialog::updatePropertyEditor()
{
    if(m_properties.count() != m_prop_type.count())
        return;

    QList<QAbstractProperty *> listProperties;
    QAbstractProperty* pProObj = Q_NULLPTR;

    for (int i = 0; i < m_properties.size(); ++i) {
        QString szKey = m_properties[i].first;
        QString szValue = m_properties[i].second;
        QString szType = m_prop_type[i].second;

        if(szType == QString("int")) {
            pProObj = QPropertyFactory::create_property("Number");
            if(pProObj != Q_NULLPTR) {
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
        else if(szType == QString("bool")) {
            pProObj = QPropertyFactory::create_property("Bool");
            if(pProObj != Q_NULLPTR) {
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


void NewComDeviceDialog::onPropertyEdit(QAbstractProperty *pro, const QVariant &value)
{
    if(m_properties.count() != m_prop_type.count())
        return;

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

