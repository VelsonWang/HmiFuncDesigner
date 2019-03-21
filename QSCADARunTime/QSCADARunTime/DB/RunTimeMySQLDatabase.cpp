#include "RunTimeMySQLDatabase.h"



RunTimeMySQLDatabase::RunTimeMySQLDatabase(const QString &dbname,
                                     const QString &user,
                                     const QString &pwd,
                                     QObject *parent)
    : MySQLDatabase(dbname, user, pwd, parent)
{

}

RunTimeMySQLDatabase::~RunTimeMySQLDatabase()
{

}

bool RunTimeMySQLDatabase::createTables()
{
#if 0  // test code
    QString autoincrement;
    QStringList keyList,valueList;
    bool ok1;

    autoincrement = "int not null auto_increment primary key";

    // test table
    keyList.clear();
    valueList.clear();
    keyList << "id"
            << "name";
    valueList << autoincrement
              << "varchar(20)";

    QString table = "test";
    QString index = "";
    ok1 = createTable(table, keyList, valueList, index);
#endif
    return true;
}


