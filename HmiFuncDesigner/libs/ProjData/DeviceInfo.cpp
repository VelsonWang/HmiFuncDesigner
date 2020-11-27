#include "DeviceInfo.h"
#include "ulog.h"

int DeviceInfo::iStartNewDeviceID_ = 0;

DeviceInfo::DeviceInfo()
{
    listDeviceInfoObject_.clear();
}

DeviceInfo::~DeviceInfo()
{
    qDeleteAll(listDeviceInfoObject_);
    listDeviceInfoObject_.clear();
}



bool DeviceInfo::openFromXml(XMLObject *pXmlObj) {
    qDeleteAll(listDeviceInfoObject_);
    listDeviceInfoObject_.clear();
    XMLObject *pDevicesObj = pXmlObj->getCurrentChild("devices");
    if(pDevicesObj == Q_NULLPTR) return false;
    QVector<XMLObject* > listUsersObj = pDevicesObj->getCurrentChildren("device");
    foreach(XMLObject* pDevObj, listUsersObj) {
        DeviceInfoObject *pObj = new DeviceInfoObject();
        pObj->iID_ = pDevObj->getProperty("id").toInt();
        if(iStartNewDeviceID_ < pObj->iID_) iStartNewDeviceID_ = pObj->iID_;
        pObj->szDeviceType_ = pDevObj->getProperty("type");
        pObj->szName_ = pDevObj->getProperty("name");
        pObj->szDeviceName_ = pDevObj->getProperty("dev_name");
        pObj->iFrameLen_ = pDevObj->getProperty("frame_len").toInt();
        pObj->szProtocol_ = pDevObj->getProperty("protocol");
        pObj->szLink_ = pDevObj->getProperty("link");
        pObj->iStateVar_ = pDevObj->getProperty("state").toInt();
        pObj->iFrameTimePeriod_ = pDevObj->getProperty("frame_period").toInt();
        pObj->iCtrlVar_ = pDevObj->getProperty("ctrl").toInt();
        pObj->bDynamicOptimization_= pDevObj->getProperty("dyn") == "1";
        pObj->iRemotePort_ = pDevObj->getProperty("remote").toInt();
        pObj->szPortParameters_ = pDevObj->getProperty("param");
        pObj->szProperties_ = pDevObj->getProperty("prop");
        listDeviceInfoObject_.append(pObj);
    }
    return true;
}


bool DeviceInfo::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pDevicesObj = new XMLObject(pXmlObj);
    pDevicesObj->setTagName("devices");

    for(int i=0; i<listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = listDeviceInfoObject_.at(i);
        XMLObject *pDevObj = new XMLObject(pDevicesObj);
        pDevObj->setTagName("device");
        pDevObj->setProperty("id", QString::number(pObj->iID_));
        pDevObj->setProperty("type", pObj->szDeviceType_);
        pDevObj->setProperty("name", pObj->szName_);
        pDevObj->setProperty("dev_name", pObj->szDeviceName_);
        pDevObj->setProperty("frame_len", QString::number(pObj->iFrameLen_));
        pDevObj->setProperty("protocol", pObj->szProtocol_);
        pDevObj->setProperty("link", pObj->szLink_);
        pDevObj->setProperty("state", QString::number(pObj->iStateVar_));
        pDevObj->setProperty("frame_period", QString::number(pObj->iFrameTimePeriod_));
        pDevObj->setProperty("ctrl", QString::number(pObj->iCtrlVar_));
        pDevObj->setProperty("dyn", pObj->bDynamicOptimization_ ? "1" : "0");
        pDevObj->setProperty("remote", QString::number(pObj->iRemotePort_));
        pDevObj->setProperty("param", pObj->szPortParameters_);
        pDevObj->setProperty("prop", pObj->szProperties_);
    }

    return true;
}

DeviceInfoObject *DeviceInfo::newObject() {
    DeviceInfoObject *pObj = new DeviceInfoObject();
    if(pObj != Q_NULLPTR) {
        pObj->iID_ = allocNewDeviceID();
        listDeviceInfoObject_.append(pObj);
    }
    return pObj;
}

DeviceInfoObject *DeviceInfo::getObjectByID(int id) {
    for(int i=0; i<listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = listDeviceInfoObject_.at(i);
        if(pObj->iID_ == id) return pObj;
    }
    return Q_NULLPTR;
}

DeviceInfoObject *DeviceInfo::getObjectByName(const QString &name) {
    for(int i=0; i<listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = listDeviceInfoObject_.at(i);
        if(pObj->szName_ == name) return pObj;
    }
    return Q_NULLPTR;
}


/**
 * @brief DeviceInfo::allocNewDeviceID
 * @details 分配一个设备ID
 * @return 设备ID
 */
int DeviceInfo::allocNewDeviceID()
{
    ++iStartNewDeviceID_;
    return iStartNewDeviceID_;
}


