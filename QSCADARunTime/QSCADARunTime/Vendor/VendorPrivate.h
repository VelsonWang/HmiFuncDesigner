#ifndef VENDORPRIVATE_H
#define VENDORPRIVATE_H

#include <QObject>
#include <QString>
#include "public.h"


class VendorPrivate
{
public:
    VendorPrivate();

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


class ComDevicePrivate : public VendorPrivate
{
public:
    bool LoadData(SaveFormat saveFormat, QString fileName);

public:
    QString m_sPortNumber;
    int m_iBaudrate;
    int m_iDatabit;
    float m_fStopbit;
    QString m_sVerifybit;
    int m_iTimeout;
};

class NetDevicePrivate : public VendorPrivate
{
public:
    bool LoadData(SaveFormat saveFormat, QString fileName);

public:
    QString m_sIpAddress;
    int m_iPort;
    QString m_sIpAddress1;
    int m_iPort1;
};

#endif // VENDORPRIVATE_H
