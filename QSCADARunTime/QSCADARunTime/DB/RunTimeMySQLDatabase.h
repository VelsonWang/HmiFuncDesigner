#ifndef RUNTIMEMYSQLDATAbASE_H
#define RUNTIMEMYSQLDATAbASE_H

#include "MySQLDatabase.h"

class RunTimeMySQLDatabase : public MySQLDatabase
{
public:
    explicit RunTimeMySQLDatabase(const QString &dbname = "xpendb",
                   const QString &user = "root",
                   const QString &pwd = "725431",
                   QObject *parent = 0);
    ~RunTimeMySQLDatabase();

    bool createTables() override;
};

#endif // RUNTIMEMYSQLDATAbASE_H
