#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QtSql/QtSql>
#include <Database.h>



class SQLiteDatabase : public Database
{
    Q_OBJECT
public:
    explicit SQLiteDatabase(const QString &dbname = "sqlitedb",
                   const QString &user = "root",
                   const QString &pwd = "725431",
                   QObject *parent = 0);

    virtual ~SQLiteDatabase();

    bool openDatabase() override;
    bool closeDatabase() override;
    bool isOpenDatabase() override;
    bool createDatabase() override;
    //when table is not exist then create it
    virtual bool createTables();

    int createTable(const QString &table,
                    QStringList fieldList,
                    QStringList typeList,
                    const QString &index="") override;

    bool insertOrUpdateRecord(const QString &table,
                              const QStringList &keyList,
                              const QStringList &valueList) override;

    bool isContain(const QString& table);

};

#endif // SQLITEDATABASE_H
