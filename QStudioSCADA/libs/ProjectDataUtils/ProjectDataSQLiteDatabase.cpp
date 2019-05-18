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


/**
 * @brief ProjectDataSQLiteDatabase::createTableSystemParameters
 * @details 创建系统参数表
 * @return true-成功, false-失败
 */
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


/**
 * @brief ProjectDataSQLiteDatabase::createTableNetSetting
 * @details 创建组网设置表
 * @return true-成功, false-失败
 */
bool ProjectDataSQLiteDatabase::createTableNetSetting()
{
    QString autoincrement = "integer not null primary key autoincrement";
    QStringList keyList,valueList;
    int ret = 0;

    keyList.clear();
    valueList.clear();

    keyList << "id" << "hot_standby_mode" << "client_mode" << "server_station"
            << "client_station" << "client_address" << "server_address"
            << "heartbeat_time" << "database_sync_time";

    valueList << autoincrement << "int" << "int" << "int"
              << "int" << "varchar(32)" << "varchar(32)"
              << "int" << "int";

    ret = createTable("t_net_setting", keyList, valueList, "");
    if(ret == 1) {
        excSQL("delete from t_net_setting");
        excSQL("insert into t_net_setting values(1, 0, 0, 0, 0, '', '', 0, 0);");
    }

    return ret;
}


bool ProjectDataSQLiteDatabase::createTables()
{
    bool ret1 = false, ret2 = false;

    // 创建系统参数表
    ret1 = createTableSystemParameters();
    // 创建组网设置表
    ret2 = createTableNetSetting();




    return ret1 && ret2;
}


