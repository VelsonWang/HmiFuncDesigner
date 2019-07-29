#ifndef NETSETTING_H
#define NETSETTING_H

#include <QString>
#include <QObject>

class NetSettingPrivate;
class ProjectDataSQLiteDatabase;

class NetSetting
{
public:
    NetSetting();
    ~NetSetting();

    bool load(ProjectDataSQLiteDatabase *pDB);
    bool save(ProjectDataSQLiteDatabase *pDB);

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
