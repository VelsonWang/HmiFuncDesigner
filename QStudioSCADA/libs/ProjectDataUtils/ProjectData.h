#ifndef _PROJECTDATA_H
#define _PROJECTDATA_H

#include <QMutex>
#include <QObject>
#include <QString>
#include "ProjectInfoManger.h"
#include "NetSetting.h"
#include "DatabaseSetting.h"
#include "UserAuthority.h"

class ProjectDataSQLiteDatabase;

class ProjectData
{

public:
    static ProjectData* getInstance();
    static void releaseInstance();
    // 创建或打开工程数据
    bool createOrOpenProjectData(const QString &projPath, const QString &projName);

public:
    static ProjectDataSQLiteDatabase *dbData_;
    ProjectInfoManger projInfoMgr_; // 工程信息管理
    NetSetting netSetting_; // 网络配置
    DatabaseSetting dbSetting_; // 数据库配置
    UserAuthority userAuthority_; // 用户权限

private:
    explicit ProjectData();
    ~ProjectData();

private:  
    static ProjectData *instance_;
    static QMutex mutex_;
    QString dbPath_;
    Q_DISABLE_COPY(ProjectData)
};


#endif // _PROJECTDATA_H
