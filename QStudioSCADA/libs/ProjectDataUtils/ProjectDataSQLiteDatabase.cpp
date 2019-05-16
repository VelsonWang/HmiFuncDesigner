#include "ProjectDataSQLiteDatabase.h"



ProjectDataSQLiteDatabase::ProjectDataSQLiteDatabase(const QString &dbname,
                                     const QString &user,
                                     const QString &pwd,
                                     const QString &hostname,
                                     int port,
                                     QObject *parent)
    : SQLiteDatabase(dbname, user, pwd, hostname, port, parent)
{

}

ProjectDataSQLiteDatabase::~ProjectDataSQLiteDatabase()
{

}

// 创建系统参数表
bool ProjectDataSQLiteDatabase::createTableSystemParameters()
{
    QString autoincrement = "integer not null primary key autoincrement";
    QStringList keyList,valueList;
    int ret = 0;

    keyList.clear();
    valueList.clear();

    keyList << "id" << "project_encrypt" << "data_scan_period" << "device_type"
            << "page_scan_period" << "project_description" << "project_name"
            << "project_path" << "start_page" << "station_address" << "station_number";

    valueList << autoincrement << "int" << "int" << "varchar(32)"
              << "int" << "varchar(1024)" << "varchar(128)"
              << "varchar(512)" << "varchar(32)" << "varchar(32)"<< "int";

    ret = createTable("t_system_parameters", keyList, valueList, "");
    if(ret == 1) {
        excSQL("delete from t_system_parameters");
        excSQL("insert into t_system_parameters values(1, 0, 500, 'TPC(800*600)', 500, 'demo1 project', '', '', 'NONE', '127.0.0.1', 0);");
    }

    return ret;
}


bool ProjectDataSQLiteDatabase::createTables()
{
    bool ret1 = false;

    // 创建系统参数表
    ret1 = createTableSystemParameters();


    return ret1;
}


