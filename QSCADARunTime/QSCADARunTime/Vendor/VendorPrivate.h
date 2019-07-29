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
    bool LoadData(const QString &devName);

    void fromString(const QString &param) {
        QStringList listParam = param.split('|');
        if(listParam.count() == 6) {
            m_sPortNumber = listParam.at(0);
            m_iBaudrate = listParam.at(1).toInt();
            m_iDatabit = listParam.at(2).toInt();
            m_fStopbit = listParam.at(3).toFloat();
            m_sVerifybit = listParam.at(4);
            m_iTimeout = listParam.at(5).toInt();
        }
    }

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
    bool LoadData(const QString &devName);

    void fromString(const QString &param) {
        QStringList listParam = param.split('|');
        if(listParam.count() == 4) {
            m_sIpAddress = listParam.at(0);
            m_iPort = listParam.at(1).toInt();
            m_sIpAddress1 = listParam.at(2).toInt();
            m_iPort1 = listParam.at(3).toFloat();
        }
    }

public:
    QString m_sIpAddress;
    int m_iPort;
    QString m_sIpAddress1;
    int m_iPort1;
};

#endif // VENDORPRIVATE_H
