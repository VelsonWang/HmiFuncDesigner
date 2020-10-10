#ifndef _PROJECTDATA_H_
#define _PROJECTDATA_H_

#include <QObject>
#include <QString>
#include "ProjectInfoManager.h"
#include "NetSetting.h"
#include "DatabaseSetting.h"
#include "UserAuthority.h"
#include "DeviceInfo.h"
#include "PictureResourceManager.h"
#include "TagSys.h"
#include "TagTmp.h"
#include "TagIO.h"


class ProjectDataSQLiteDatabase;

class ProjectData
{
public:
    explicit ProjectData();
    ~ProjectData();

    bool openFromXml(const QString &szProjFile);
    bool saveToXml(const QString &szProjFile);

    static ProjectData* getInstance();

    // 创建或打开工程数据
    bool createOrOpenProjectData(const QString &projPath, const QString &projName);
    // 获取工程数据文件路径
    QString getDBPath() const;

    //获取工程所有变量的名称
    void getAllTagName(QStringList &varList, const QString &type = "ALL");

public:
    static ProjectDataSQLiteDatabase *dbData_;
    static QString szProjPath_;
    static QString szProjName_;
    QString szProjVersion_; // 工程管理器版本
    ProjectInfoManager projInfoMgr_; // 工程信息管理
    NetSetting netSetting_; // 网络配置
    DatabaseSetting dbSetting_; // 数据库配置
    UserAuthority userAuthority_; // 用户权限
    DeviceInfo deviceInfo_; // 设备配置信息
    PictureResourceManager pictureResourceMgr_; // 图片资源管理
    TagSys tagSys_; // 系统标签变量
    TagTmp tagTmp_; // 中间标签变量
    TagIO tagIO_; // 设备标签变量
    TagIOGroup tagIOGroup_; // 设备标签变量组

private:  
    QString dbPath_;

    Q_DISABLE_COPY(ProjectData)
};


#endif // _PROJECTDATA_H
