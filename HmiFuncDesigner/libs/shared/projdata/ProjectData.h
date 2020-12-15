#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QObject>
#include <QString>
#include "ProjectInfoManager.h"
#include "NetSetting.h"
#include "DatabaseSetting.h"
#include "UserAuthority.h"
#include "DeviceInfo.h"
#include "PictureResourceManager.h"
#include "Tag.h"
#include "Script.h"
#include "IGraphPageSaveLoad.h"
#include "sharedlibglobal.h"

#pragma pack(push)
#pragma pack(1)
typedef struct FileHeader
{
    quint16 wSize; // 文件头部大小(2 Byte)
    quint16 wVersion; // 文件头版本(2 Byte)
    quint32 dwProjSize; // 工程数据大小(4 Byte)
    quint8 byEncrypt; // 工程加密
    quint8 byOpenVerifyPassword; // 打开工程需要验证密码
    char szPassword[32]; // 打开工程的密码

} TFileHeader, *PFileHeader;
#pragma pack(pop)


////////////////////////////////////////////////////////////////////////////////

class SHAREDLIB_EXPORT ProjectData
{
public:
    explicit ProjectData();
    ~ProjectData();

    bool openFromXml(const QString &szProjFile);
    bool saveToXml(const QString &szProjFile);

    static ProjectData* getInstance();

    //获取工程所有变量的名称
    void getAllTagName(QStringList &varList, const QString &type = "ALL");

    // 获取工程路径
    QString getProjectPath(const QString &projectName);
    // 获取包含后缀工程名称
    QString getProjectNameWithSuffix(const QString &projectName);
    // 获取不包含后缀工程名称
    QString getProjectNameWithOutSuffix(const QString &projectName);
    // 获取工程所有控件的ID名称
    void getAllElementIDName(QStringList &szIDList);
    // 获取工程所有画面名称
    void getAllGraphPageName(QStringList &szList);

public:
    QString szProjFile_; // 工程文件名
    QString szProjPath_; // 工程文件所在的路径
    QString szProjName_; // 工程文件名称
    QString szProjVersion_; // 工程管理器版本
    ProjectInfoManager projInfoMgr_; // 工程信息管理
    NetSetting netSetting_; // 网络配置
    DatabaseSetting dbSetting_; // 数据库配置
    UserAuthority userAuthority_; // 用户权限
    DeviceInfo deviceInfo_; // 设备配置信息
    PictureResourceManager pictureResourceMgr_; // 图片资源管理
    TagManager tagMgr_; // 标签变量
    Script script_; // 脚本
    IGraphPageSaveLoad *pImplGraphPageSaveLoadObj_; // 画面
    TFileHeader headerObj_;
private:  

    Q_DISABLE_COPY(ProjectData)
};


#endif // PROJECTDATA_H
