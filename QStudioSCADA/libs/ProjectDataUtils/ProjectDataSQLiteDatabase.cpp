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


/**
 * @brief ProjectDataSQLiteDatabase::createTableDatabaseSetting
 * @details 创建数据库设置表
 * @return true-成功, false-失败
 */
bool ProjectDataSQLiteDatabase::createTableDatabaseSetting()
{
    QString autoincrement = "integer not null primary key autoincrement";
    QStringList keyList,valueList;
    int ret = 0;

    keyList.clear();
    valueList.clear();

    keyList << "id" << "alarm_size" << "special_db" << "auto_delete"
            << "db_type" << "data_keep_days" << "ip_address" << "use_sd"
            << "user" << "save_period" << "password" << "send_period"
            << "db_name" << "start_time" << "port";

    valueList << autoincrement << "int" << "int" << "int"
              << "varchar(32)" << "int" << "varchar(32)" << "int"
              << "varchar(32)" << "int" << "varchar(32)" << "int"
              << "varchar(32)" << "int" << "int";

    ret = createTable("t_database_setting", keyList, valueList, "");
    if(ret == 1) {
        excSQL("delete from t_database_setting");
        excSQL("insert into t_database_setting values(1, 50, 0, 0, 'MYSQL', 10, 'localhost', 0, 'root', 60, '123456', 500, 'hisdb', 1, 3306)");
    }

    return ret;
}


/**
 * @brief ProjectDataSQLiteDatabase::createTableUserAuthority
 * @details 创建用户权限表
 * @return true-成功, false-失败
 */
bool ProjectDataSQLiteDatabase::createTableUserAuthority()
{
    QString autoincrement = "integer not null primary key autoincrement";
    QStringList keyList,valueList;
    int ret = 0;

    keyList.clear();
    valueList.clear();

    keyList << "id" << "number" << "name" << "password"
            << "authority" << "comments" << "name2" << "name3"
            << "name4" << "name5" << "login" << "logout";

    valueList << autoincrement << "varchar(4)" << "varchar(32)" << "varchar(16)"
              << "varchar(2)" << "varchar(512)" << "varchar(32)" << "varchar(32)"
              << "varchar(32)" << "varchar(32)" << "int" << "int";

    ret = createTable("t_user_authority", keyList, valueList, "");
    if(ret == 1) {

    }

    return ret;
}


/**
 * @brief ProjectDataSQLiteDatabase::createTableDeviceInfo
 * @details 创建设备信息表
 * @return true-成功, false-失败
 */
bool ProjectDataSQLiteDatabase::createTableDeviceInfo()
{
    QString autoincrement = "integer not null primary key autoincrement";
    QStringList keyList,valueList;
    int ret = 0;

    keyList.clear();
    valueList.clear();

    keyList << "id" << "type" << "name" << "frame_length"
            << "protocol" << "link" << "state_var" << "frame_time_period"
            << "ctrl_var" << "dynamic_optimization" << "remote_port" << "port_parameters";

    valueList << autoincrement << "varchar(8)" << "varchar(64)" << "int"
              << "varchar(64)" << "varchar(32)" << "int" << "int"
              << "int" << "int" << "int" << "varchar(512)";

    ret = createTable("t_device_info", keyList, valueList, "");
    if(ret == 1) {

    }

    return ret;
}


/**
 * @brief ProjectDataSQLiteDatabase::createTablePictureResourceInfo
 * @details 图片资源信息表
 * @return true-成功, false-失败
 */
bool ProjectDataSQLiteDatabase::createTablePictureResourceInfo()
{
    QString autoincrement = "integer not null primary key autoincrement";
    QStringList keyList,valueList;
    int ret = 0;

    keyList.clear();
    valueList.clear();

    keyList << "id" << "name" << "ref_count";

    valueList << autoincrement << "varchar(64)" << "int";

    ret = createTable("t_picture_resource_info", keyList, valueList, "");
    if(ret == 1) {

    }

    return ret;
}


/**
 * @brief ProjectDataSQLiteDatabase::createTables
 * @details 创建工程参数表
 * @return true-成功, false-失败
 */
bool ProjectDataSQLiteDatabase::createTables()
{
#define RET_MAX    (16)
    int i = 0;
    bool ret[RET_MAX];
    for(i=0; i<RET_MAX; i++)
        ret[i] = true;

    // 创建系统参数表
    ret[0] = createTableSystemParameters();
    // 创建组网设置表
    ret[1] = createTableNetSetting();
    // 创建数据库设置表
    ret[2] = createTableDatabaseSetting();
    // 创建用户权限表
    ret[3] = createTableUserAuthority();
    // 创建设备信息表
    ret[4] = createTableDeviceInfo();
    // 图片资源信息表
    ret[5] = createTablePictureResourceInfo();

    for(i=0; i<RET_MAX; i++) {
        if(!ret[i])
            return false;
    }

    return true;
}


