#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QStringList>
#include <QList>

class ProjectDataSQLiteDatabase;

class DeviceInfoObject
{
public:
    explicit DeviceInfoObject()
    {

    }

    DeviceInfoObject(const DeviceInfoObject &obj)
    {
        copyObject(obj);
    }

    ~DeviceInfoObject()
    {

    }

    DeviceInfoObject &operator=(const DeviceInfoObject &obj)
    {
        copyObject(obj);
        return *this;
    }

    void copyObject(const DeviceInfoObject &obj)
    {
        iID_ = obj.iID_;
        szDeviceType_ = obj.szDeviceType_;
        szDeviceName_ = obj.szDeviceName_;
        iFrameLen_ = obj.iFrameLen_;
        szProtocol_ = obj.szProtocol_;
        szLink_ = obj.szLink_;
        iStateVar_ = obj.iStateVar_;
        iFrameTimePeriod_ = obj.iFrameTimePeriod_;
        iCtrlVar_ = obj.iCtrlVar_;
        bDynamicOptimization_ = obj.bDynamicOptimization_;
        iRemotePort_ = obj.iRemotePort_;
        szPortParameters_ = obj.szPortParameters_;
    }

public:
    int iID_;
    QString szDeviceType_; //COM, NET, BUS, OPC
    QString szDeviceName_;
    int iFrameLen_;
    QString szProtocol_;
    QString szLink_;
    int iStateVar_;
    int iFrameTimePeriod_;
    int iCtrlVar_;
    bool bDynamicOptimization_;
    int iRemotePort_;
    QString szPortParameters_;
};

class ComDevice : public DeviceInfoObject
{
public:
    explicit ComDevice()
    {

    }

    ComDevice(const ComDevice &obj)
    {
        copyObject(obj);
    }

    ~ComDevice()
    {

    }

    ComDevice &operator=(const ComDevice &obj)
    {
        copyObject(obj);
        return *this;
    }

    void copyObject(const ComDevice &obj)
    {
        DeviceInfoObject::copyObject(obj);
        szPortNumber_ = obj.szPortNumber_;
        iBaudrate_ = obj.iBaudrate_;
        iDatabit_ = obj.iDatabit_;
        fStopbit_ = obj.fStopbit_;
        szVerifybit_ = obj.szVerifybit_;
        iTimeout_ = obj.iTimeout_;
    }

    void fromString(const QString &param) {
        QStringList listParam = param.split('|');
        if(listParam.count() == 6) {
            szPortNumber_ = listParam.at(0);
            iBaudrate_ = listParam.at(1).toInt();
            iDatabit_ = listParam.at(2).toInt();
            fStopbit_ = listParam.at(3).toFloat();
            szVerifybit_ = listParam.at(4);
            iTimeout_ = listParam.at(5).toInt();
        }
    }

    QString toString() {
        QStringList param;
        param.append(szPortNumber_);
        param.append(QString::number(iBaudrate_));
        param.append(QString::number(iDatabit_));
        param.append(QString::number(fStopbit_));
        param.append(szVerifybit_);
        param.append(QString::number(iTimeout_));
        return param.join("|");
    }

public:
    QString szPortNumber_;
    int iBaudrate_;
    int iDatabit_;
    float fStopbit_;
    QString szVerifybit_;
    int iTimeout_;
};


class NetDevice : public DeviceInfoObject
{
public:
    explicit NetDevice()
    {

    }

    NetDevice(const NetDevice &obj)
    {
        copyObject(obj);
    }

    ~NetDevice()
    {

    }

    NetDevice &operator=(const NetDevice &obj)
    {
        copyObject(obj);
        return *this;
    }

    void copyObject(const NetDevice &obj)
    {
        DeviceInfoObject::copyObject(obj);
        szIpAddress_ = obj.szIpAddress_;
        iPort_ = obj.iPort_;
        szIpAddress1_ = obj.szIpAddress1_;
        iPort1_ = obj.iPort1_;
    }

    void fromString(const QString &param) {
        QStringList listParam = param.split('|');
        if(listParam.count() == 4) {
            szIpAddress_ = listParam.at(0);
            iPort_ = listParam.at(1).toInt();
            szIpAddress1_ = listParam.at(2).toInt();
            iPort1_ = listParam.at(3).toFloat();
        }
    }

    QString toString() {
        QStringList param;
        param.append(szIpAddress_);
        param.append(QString::number(iPort_));
        param.append(szIpAddress1_);
        param.append(QString::number(iPort1_));
        return param.join("|");
    }

public:
    QString szIpAddress_;
    int iPort_;
    QString szIpAddress1_;
    int iPort1_;
};



class DeviceInfo
{
public:
    explicit DeviceInfo();
    ~DeviceInfo();

    bool load(ProjectDataSQLiteDatabase *pDB);
    bool save(ProjectDataSQLiteDatabase *pDB);

    bool insert(ProjectDataSQLiteDatabase *pDB, DeviceInfoObject *pObj);
    bool del(ProjectDataSQLiteDatabase *pDB, DeviceInfoObject *pObj);
    bool delAll(ProjectDataSQLiteDatabase *pDB);
    bool update(ProjectDataSQLiteDatabase *pDB, DeviceInfoObject *pObj);

    DeviceInfoObject *getDeviceInfoObjectByID(int id);
    DeviceInfoObject *getDeviceInfoObjectByName(const QString &name);
    int getLastInsertId(ProjectDataSQLiteDatabase *pDB);

public:
    QList<DeviceInfoObject *> listDeviceInfoObject_;
};

#endif // DEVICEINFO_H
