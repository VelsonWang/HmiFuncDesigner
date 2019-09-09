#ifndef _PROJECTDATA_H
#define _PROJECTDATA_H

#include <QMutex>
#include <QObject>
#include <QString>
#include "ProjectInfoManager.h"
#include "NetSetting.h"
#include "DatabaseSetting.h"
#include "UserAuthority.h"
#include "DeviceInfo.h"
#include "PictureResourceManager.h"

class ProjectDataSQLiteDatabase;

class ProjectData
{

public:
    static ProjectData* getInstance();
    static void releaseInstance();
    // 创建或打开工程数据
    bool createOrOpenProjectData(const QString &projPath, const QString &projName);
    // 获取工程数据文件路径
    QString getDBPath() const;

public:
    static ProjectDataSQLiteDatabase *dbData_;
    static QString szProjPath_;
    static QString szProjName_;
    ProjectInfoManager projInfoMgr_; // 工程信息管理
    NetSetting netSetting_; // 网络配置
    DatabaseSetting dbSetting_; // 数据库配置
    UserAuthority userAuthority_; // 用户权限
    DeviceInfo deviceInfo_; // 设备配置信息
    PictureResourceManager pictureResourceMgr_; // 图片资源管理

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
