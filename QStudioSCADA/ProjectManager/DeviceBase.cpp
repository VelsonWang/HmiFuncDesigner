#include "DeviceBase.h"

DeviceBase::DeviceBase() :
    m_sDeviceType(QString()),
    m_sDeviceName(QString()),
    m_iFrameLen(0),
    m_sProtocol(QString()),
    m_sLink(QString()),
    m_iStateVar(0),
    m_iFrameTimePeriod(0),
    m_iCtrlVar(0),
    m_bDynamicOptimization(false),
    m_iRemotePort(0)
{

}


