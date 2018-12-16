#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <QString>

class DeviceBase
{
public:
    DeviceBase();

public:
    QString m_sDeviceType;
    QString m_sDeviceName;
    int m_iFrameLen;
    QString m_sProtocol;
    QString m_sLink;
    int m_iStateVar;
    int m_iFrameTimePeriod;
    int m_iCtrlVar;
    bool m_bDynamicOptimization;
    int m_iRemotePort;

};

#endif // DEVICEBASE_H
