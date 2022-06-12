#ifndef VENDOR_H
#define VENDOR_H

#include <QString>
#include <QStringList>
#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include <QQueue>
#include <QList>
#include <QMap>
#include "../Vendors/IVendorPlugin/IVendorPlugin.h"
#include "realtimedb.h"
#include "Port/IPort.h"
#include "public.h"


class IVendorPlugin;
class QProjectCore;

class VendorPrivate
{
public:
    VendorPrivate();
    void parsePropertiesFromString(const QString &szProperty, QMap<QString, QVariant> &mapProperties);

public:
    QString m_sDeviceType;
    QString m_sDeviceName;
    int m_iFrameLen;
    QString m_sProtocol;
    QString m_sLink;
    int m_iStateVar;
    int m_iFrameTimePeriod; // 帧间隔
    int m_iCtrlVar;
    bool m_bDynamicOptimization;
    int m_iRemotePort;
    int m_iRetryTimes; // 通信失败重试次数
    int m_iCommTimeout; // 通信超时时间
    int m_iCommResumeTime; // 尝试恢复通讯间隔时间
    QMap<QString, QVariant> m_mapProperties; // 设备私有属性
};

//-----------------------------------------------------------------------------

class ComDevicePrivate : public VendorPrivate
{
public:
    bool LoadData(const QString &devName, QProjectCore *coreObj);

    void fromString(const QString &param)
    {
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

//-----------------------------------------------------------------------------

class NetDevicePrivate : public VendorPrivate
{
public:
    bool LoadData(const QString &devName, QProjectCore *coreObj);

    void fromString(const QString &param)
    {
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

//-----------------------------------------------------------------------------

class Vendor
{
public:
    Vendor(QProjectCore *coreObj);
    ~Vendor();

    // 获取设备名称
    QString getDeviceName();
    // 打开设备
    bool open();
    // 设备初始化
    void initialize();
    // 添加设备变量至变量列表
    void addIOTagToDeviceTagList(RunTimeTag* pTag);
    // 查找设备变量
    RunTimeTag* findIOTagByID(int id);
    // 写变量
    bool writeIOTag(RunTimeTag* pTag);
    // 写变量列表
    bool writeIOTags();
    // 读变量
    bool readIOTag(RunTimeTag* pTag);
    // 读变量列表
    bool readIOTags();
    // 是否运行
    bool isRunning();
    // 启动
    void start();
    // 停止
    void stop();
    // 循环处理
    void doLoop();
    // 是否重启运行
    bool isRestart();
    // 是否在线
    bool isOnLine();
    // 关闭设备
    bool close();
    // 获取端口名称
    QString getPortName();
    // 设备端口重新连接
    bool reconnect();

public:
    quint8 readBuf[512] = {0};
    quint8 writeBuf[512] = {0};
    unsigned char m_extraData[32] = {0}; // 变量私有数据

    void clearReadBuffer();
    void clearWriteBuffer();

public:
    IPort *m_pPortObj;
    QList<RunTimeTag *> m_readList;
    bool m_bIsRunning = false;
    IVendorPlugin *m_pVendorPluginObj = NULL;
    VendorPrivate *m_pVendorPrivateObj = NULL;
    bool m_bOnlineStatus = false; // true-在线, false-离线
    bool m_bOffLine = false; // 通讯失败次数大于通信失败重试次数视为离线
    qint64 m_iStartOffLineTime = 0; // 离线起始时间

private:
    QProjectCore *projCore;
};


#endif // VENDOR_H
