#ifndef DATABASESETTING_H
#define DATABASESETTING_H

#include <QString>
#include <QObject>
#include "../xmlobject.h"
#include "sharedlibglobal.h"

class DatabaseSettingPrivate;

class SHAREDLIB_EXPORT DatabaseSetting
{
public:
    DatabaseSetting();
    ~DatabaseSetting();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

public:
    int getAlarmSize();
    void setAlarmSize(int size);

    bool isSpecialDB();
    void setSpecialDB(bool special);

    bool isAutoDelete();
    void setAutoDelete(bool aut);

    QString getDBType() const;
    void setDBType(const QString &type);

    int getDataKeepDays();
    void setDataKeepDays(int days);

    QString getIPAddress() const;
    void setIPAddress(const QString &ip);

    bool isUseSD();
    void setUseSD(bool sd);

    QString getUserName() const;
    void setUserName(const QString &name);

    int getSavePeriod();
    void setSavePeriod(int period);

    QString getPassword() const;
    void setPassword(const QString &password);

    int getSendPeriod();
    void setSendPeriod(int period);

    QString getDBName() const;
    void setDBName(const QString &name);

    int getStartTime();
    void setStartTime(int time);

    int getPort();
    void setPort(int port);

private:
    DatabaseSettingPrivate *const dPtr_;

    Q_DISABLE_COPY(DatabaseSetting)
};

#endif // DATABASESETTING_H
