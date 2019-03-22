#ifndef RUNTIMEMYSQLDATAbASE_H
#define RUNTIMEMYSQLDATAbASE_H

#include "MySQLDatabase.h"

class RunTimeMySQLDatabase : public MySQLDatabase
{
public:
    explicit RunTimeMySQLDatabase(const QString &dbname = "runtimedb",
                   const QString &user = "root",
                   const QString &pwd = "725431",
                   const QString &hostname = "127.0.0.1",
                   int port = 3306,
                   QObject *parent = 0);
    ~RunTimeMySQLDatabase();

    bool createTables() override;
};

#endif // RUNTIMEMYSQLDATAbASE_H
