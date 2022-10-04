#include "deviceinfo.h"
#include "ulog.h"

int DeviceInfo::m_nextDeviceID = 0;

DeviceInfo::DeviceInfo()
{
    m_deviceInfoObject.clear();
}

DeviceInfo::~DeviceInfo()
{
    qDeleteAll(m_deviceInfoObject);
    m_deviceInfoObject.clear();
}

bool DeviceInfo::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(m_deviceInfoObject);
    m_deviceInfoObject.clear();
    XMLObject *pDevicesObj = pXmlObj->getCurrentChild("devices");
    if(pDevicesObj == NULL) {
        return false;
    }
    QVector<XMLObject* > listUsersObj = pDevicesObj->getCurrentChildren("device");
    foreach(XMLObject* pDevObj, listUsersObj) {
        DeviceInfoObject *pObj = new DeviceInfoObject();
        pObj->m_id = pDevObj->getProperty("id").toInt();
        if(m_nextDeviceID < pObj->m_id) {
            m_nextDeviceID = pObj->m_id;
        }
        pObj->m_deviceType = pDevObj->getProperty("type");
        pObj->m_name = pDevObj->getProperty("name");
        pObj->m_deviceName = pDevObj->getProperty("dev_name");
        pObj->m_frameLen = pDevObj->getProperty("frame_len").toInt();
        pObj->m_protocol = pDevObj->getProperty("protocol");
        pObj->m_link = pDevObj->getProperty("link");
        pObj->m_stateVar = pDevObj->getProperty("state").toInt();
        pObj->m_frameTimePeriod = pDevObj->getProperty("frame_period").toInt();
        pObj->m_ctrlVar = pDevObj->getProperty("ctrl").toInt();
        pObj->m_dynamicOptimization = pDevObj->getProperty("dyn") == "1";
        pObj->m_remotePort = pDevObj->getProperty("remote").toInt();
        pObj->m_portParameters = pDevObj->getProperty("param");
        pObj->m_properties = pDevObj->getProperty("prop");
        m_deviceInfoObject.append(pObj);
    }
    return true;
}


bool DeviceInfo::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pDevicesObj = new XMLObject(pXmlObj);
    pDevicesObj->setTagName("devices");

    for(int i = 0; i < m_deviceInfoObject.count(); i++) {
        DeviceInfoObject *pObj = m_deviceInfoObject.at(i);
        XMLObject *pDevObj = new XMLObject(pDevicesObj);
        pDevObj->setTagName("device");
        pDevObj->setProperty("id", QString::number(pObj->m_id));
        pDevObj->setProperty("type", pObj->m_deviceType);
        pDevObj->setProperty("name", pObj->m_name);
        pDevObj->setProperty("dev_name", pObj->m_deviceName);
        pDevObj->setProperty("frame_len", QString::number(pObj->m_frameLen));
        pDevObj->setProperty("protocol", pObj->m_protocol);
        pDevObj->setProperty("link", pObj->m_link);
        pDevObj->setProperty("state", QString::number(pObj->m_stateVar));
        pDevObj->setProperty("frame_period", QString::number(pObj->m_frameTimePeriod));
        pDevObj->setProperty("ctrl", QString::number(pObj->m_ctrlVar));
        pDevObj->setProperty("dyn", pObj->m_dynamicOptimization ? "1" : "0");
        pDevObj->setProperty("remote", QString::number(pObj->m_remotePort));
        pDevObj->setProperty("param", pObj->m_portParameters);
        pDevObj->setProperty("prop", pObj->m_properties);
    }

    return true;
}

DeviceInfoObject *DeviceInfo::newObject()
{
    DeviceInfoObject *pObj = new DeviceInfoObject();
    if(pObj != NULL) {
        pObj->m_id = allocNewDeviceID();
        m_deviceInfoObject.append(pObj);
    }
    return pObj;
}

DeviceInfoObject *DeviceInfo::getObjectByID(int id)
{
    for(int i = 0; i < m_deviceInfoObject.count(); i++) {
        DeviceInfoObject *pObj = m_deviceInfoObject.at(i);
        if(pObj->m_id == id) {
            return pObj;
        }
    }
    return NULL;
}

DeviceInfoObject *DeviceInfo::getObjectByName(const QString &name)
{
    for(int i = 0; i < m_deviceInfoObject.count(); i++) {
        DeviceInfoObject *pObj = m_deviceInfoObject.at(i);
        if(pObj->m_name == name) {
            return pObj;
        }
    }
    return NULL;
}


/**
 * @brief DeviceInfo::allocNewDeviceID
 * @details 分配一个设备ID
 * @return 设备ID
 */
int DeviceInfo::allocNewDeviceID()
{
    ++m_nextDeviceID;
    return m_nextDeviceID;
}


