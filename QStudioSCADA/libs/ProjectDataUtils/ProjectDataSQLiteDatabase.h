#ifndef RUNTIMEMYSQLDATAbASE_H
#define RUNTIMEMYSQLDATAbASE_H

#include "SQLiteDatabase.h"

class ProjectDataSQLiteDatabase : public SQLiteDatabase
{
public:
    explicit ProjectDataSQLiteDatabase(const QString &dbname = "DefaultProject.pdt",
                   const QString &user = "root",
                   const QString &pwd = "725431",
                   const QString &hostname = "127.0.0.1",
                   int port = 3306,
                   QObject *parent = 0);
    ~ProjectDataSQLiteDatabase();

    // 创建系统参数表
    bool createTableSystemParameters();
    // 创建组网设置表
    bool createTableNetSetting();
    // 创建数据库设置表
    bool createTableDatabaseSetting();
    // 创建用户权限表
    bool createTableUserAuthority();
    // 创建设备信息表
    bool createTableDeviceInfo();
    // 图片资源信息表
    bool createTablePictureResourceInfo();

    // 创建数据库表
    bool createTables() override;
};

#endif // RUNTIMEMYSQLDATAbASE_H
