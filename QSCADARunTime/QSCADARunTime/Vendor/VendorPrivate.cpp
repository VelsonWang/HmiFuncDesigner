#include "VendorPrivate.h"
#include "ProjectData.h"
#include <QFile>



VendorPrivate::VendorPrivate()
{

}


/*
* 从文件读取配置数据
*/
bool ComDevicePrivate::LoadData(const QString &devName)
{
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);
    DeviceInfoObject *pObj = deviceInfo.getDeviceInfoObjectByName(devName);

    m_sDeviceName = pObj->szDeviceName_;
    m_iFrameLen = pObj->iFrameLen_;
    m_sProtocol = pObj->szProtocol_;
    m_sLink = pObj->szLink_;
    m_iStateVar = pObj->iStateVar_;
    m_iFrameTimePeriod = pObj->iFrameTimePeriod_;
    m_iCtrlVar = pObj->iCtrlVar_;
    m_bDynamicOptimization = pObj->bDynamicOptimization_;
    m_iRemotePort = pObj->iRemotePort_;
    m_sDeviceType = "COM";

    ComDevice comDev;
    comDev.fromString(pObj->szPortParameters_);

    m_sPortNumber = comDev.szPortNumber_;
    m_iBaudrate = comDev.iBaudrate_;
    m_iDatabit = comDev.iDatabit_;
    m_fStopbit = comDev.fStopbit_;
    m_sVerifybit = comDev.szVerifybit_;
    m_iTimeout = comDev.iTimeout_;

    return true;
}



/*
* 从文件读取配置数据
*/
bool NetDevicePrivate::LoadData(const QString &devName)
{
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);
    DeviceInfoObject *pObj = deviceInfo.getDeviceInfoObjectByName(devName);

    m_sDeviceName = pObj->szDeviceName_;
    m_iFrameLen = pObj->iFrameLen_;
    m_sProtocol = pObj->szProtocol_;
    m_sLink = pObj->szLink_;
    m_iStateVar = pObj->iStateVar_;
    m_iFrameTimePeriod = pObj->iFrameTimePeriod_;
    m_iCtrlVar = pObj->iCtrlVar_;
    m_bDynamicOptimization = pObj->bDynamicOptimization_;
    m_iRemotePort = pObj->iRemotePort_;
    m_sDeviceType = "NET";

    NetDevice netDev;
    netDev.fromString(pObj->szPortParameters_);
    m_sIpAddress = netDev.szIpAddress_;
    m_iPort = netDev.iPort_;
    m_sIpAddress1 = netDev.szIpAddress1_;
    m_iPort1 = netDev.iPort1_;

    return true;
}
