#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <QtSql/QtSql>
#include "Database.h"


class MySQLDatabase : public Database
{
    Q_OBJECT
public:
    explicit MySQLDatabase(const QString &dbname = "mysqldb",
                   const QString &user = "root",
                   const QString &pwd = "725431",
                   const QString &hostname = "127.0.0.1",
                   int port = 3306,
                   QObject *parent = 0);
    virtual ~MySQLDatabase();

    bool openDatabase() override;
    bool closeDatabase() override;
    bool isOpenDatabase() override;
    bool createDatabase() override;
    virtual bool createTables();

    int createTable(const QString &table,
                    QStringList fieldList,
                    QStringList typeList,
                    const QString &index="") override;


    bool insertOrUpdateRecord(const QString &table,
                              const QStringList &keyList,
                              const QStringList &valueList) override;

};

#endif // MYSQLDATABASE_H
