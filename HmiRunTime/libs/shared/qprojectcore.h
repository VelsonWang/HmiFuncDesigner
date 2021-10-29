#ifndef QPROJECTCORE_H
#define QPROJECTCORE_H

#include "sharedlibglobal.h"
#include <QObject>
#include <QString>
#include "./projdata/projectinfomanager.h"
#include "./projdata/netsetting.h"
#include "./projdata/databasesetting.h"
#include "./projdata/userauthority.h"
#include "./projdata/deviceinfo.h"
#include "./projdata/pictureresourcemanager.h"
#include "./projdata/tag.h"
#include "./projdata/script.h"
#include "sharedlibglobal.h"

#pragma pack(push)
#pragma pack(1)
typedef struct FileHeader {
    quint16 wSize; // 文件头部大小(2 Byte)
    quint16 wVersion; // 文件头版本(2 Byte)
    quint32 dwProjSize; // 工程数据大小(4 Byte)
    quint8 byEncrypt; // 工程加密
    quint8 byOpenVerifyPassword; // 打开工程需要验证密码
    char szPassword[32]; // 打开工程的密码

} TFileHeader, *PFileHeader;
#pragma pack(pop)


////////////////////////////////////////////////////////////////////////////////

class QProjectHost;
class QFormHost;
class QPageManager;
class QAbstractHost;
class QScriptValue;
class QScriptEngine;
class XMLObject;


class SHAREDLIB_EXPORT QProjectCore : public QObject
{
    Q_OBJECT
public:
    explicit QProjectCore(QObject *parent = NULL);
    ~QProjectCore();

    void close();
    bool isOpened();

    bool openFromXml(const QString &szProjFile);
    bool saveToXml(const QString &szProjFile);

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


    bool createNewProj(const QString &szfile);


    QPageManager* getPageManager();
    QAbstractHost* getProjectHost();
    QAbstractHost* getHostByUuid(const QString& uuid);

    void initScriptEngine();

protected:
    void initScriptEngine(QAbstractHost* host);
    QScriptValue getScriptObject(QAbstractHost* host, QScriptEngine *engine);

signals:
    void notifyOpened();
    void notifyClosed();

protected slots:
    void onFormRefresh(QAbstractHost* form);

public:
    QString m_file; // 工程文件名
    QString m_path; // 工程文件所在的路径
    QString m_name; // 工程文件名称
    QString m_version; // 工程管理器版本
    ProjectInfoManager m_projInfoMgr; // 工程信息管理
    NetSetting m_netSetting; // 网络配置
    DatabaseSetting m_dbSetting; // 数据库配置
    UserAuthority m_userAuthority; // 用户权限
    DeviceInfo m_deviceInfo; // 设备配置信息
    PictureResourceManager m_picResMgr; // 图片资源管理
    TagManager m_tagMgr; // 标签变量
    Script m_script; // 脚本
    TFileHeader m_headerObj;

protected:
    QProjectHost* m_pProjectHostObj;
    bool m_bOpen;
    QPageManager* m_pPageMgrObj;

};


#endif // QPROJECTCORE_H
