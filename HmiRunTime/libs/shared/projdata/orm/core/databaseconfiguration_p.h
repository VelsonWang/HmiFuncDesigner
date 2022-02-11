#ifndef DATABASECONFIGURATION_P_H
#define DATABASECONFIGURATION_P_H

#include <QtCore/QString>
#include <QtCore/QUrl>

class DatabaseConfiguration
{
public:
    virtual ~DatabaseConfiguration();

    static DatabaseConfiguration* self();

    bool configureDatabase(const QUrl& url);

    QString databaseSystem() const;
    QString hostName() const;
    QString userName() const;
    QString databaseName() const;
    int port() const;

private:
    DatabaseConfiguration();
    static DatabaseConfiguration* s_instance;

    QString m_databaseSystem;
    QString m_hostName;
    QString m_userName;
    QString m_databaseName;
    int m_port;

};



#endif
/* vim: set et sts=4 sw=4 ts=16 tw=78 : */

