#ifndef NETSETTING_H
#define NETSETTING_H

#include <QString>
#include <QObject>
#include "XMLObject.h"

class NetSettingPrivate;
class ProjectDataSQLiteDatabase;

class NetSetting
{
public:
    NetSetting();
    ~NetSetting();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

public:
    bool isHotStandbyMode();
    void setHotStandbyMode(bool mode);

    bool isClientMode();
    void setClientMode(bool mode);

    bool isServerStation();
    void setServerStation(bool server);

    bool isClientStation();
    void setClientStation(bool client);

    QString getClientAddress() const;
    void setClientAddress(const QString &addr);

    QString getServerAddress() const;
    void setServerAddress(const QString &addr);

    int getHeartbeatTime();
    void setHeartbeatTime(int time);

    int getDatabaseSyncTime();
    void setDatabaseSyncTime(int time);

private:
    NetSettingPrivate *const dPtr_;

    Q_DISABLE_COPY(NetSetting)
};

#endif // NETSETTING_H
