#include "ProjectData.h"
#include "ProjectDataSQLiteDatabase.h"
#include "XMLObject.h"
#include "Helper.h"
#include <QFileInfo>
#include <QDebug>

ProjectDataSQLiteDatabase *ProjectData::dbData_ = Q_NULLPTR;



ProjectData::ProjectData()
    : dbPath_(""),
      szProjVersion_("V1.0.0")
{
    szProjPath_ = "";
    szProjName_ = "";
}

ProjectData::~ProjectData()
{
    if(dbData_ != Q_NULLPTR) {
        delete dbData_;
        dbData_ = Q_NULLPTR;
    }
}


ProjectData* ProjectData::getInstance()
{
    static ProjectData instance_;
    return &instance_;
}


bool ProjectData::openFromXml(const QString &szProjFile)
{
    QString buffer = Helper::readString(szProjFile);
    XMLObject xml;
    if(!xml.load(buffer, 0)) return false;

    QList<XMLObject*> projObjs = xml.getChildren();
    foreach(XMLObject* pProjObj, projObjs) {
        // 工程信息管理
        projInfoMgr_.openFromXml(pProjObj);
        // 网络配置
        netSetting_.openFromXml(pProjObj);
        // 数据库配置
        dbSetting_.openFromXml(pProjObj);
        // 用户权限
        userAuthority_.openFromXml(pProjObj);
        // 设备配置信息
        deviceInfo_.openFromXml(pProjObj);
        // 设备标签变量组
        tagIOGroup_.openFromXml(pProjObj);
    }

    return true;
}


bool ProjectData::saveToXml(const QString &szProjFile)
{
    XMLObject projObjs;
    projObjs.setTagName("projects");

    XMLObject *pProjObj = new XMLObject(&projObjs);
    pProjObj->setTagName("project");
    pProjObj->setProperty("application_version", szProjVersion_);

    // 工程信息管理
    projInfoMgr_.saveToXml(pProjObj);
    // 网络配置
    netSetting_.saveToXml(pProjObj);
    // 数据库配置
    dbSetting_.saveToXml(pProjObj);
    // 用户权限
    userAuthority_.saveToXml(pProjObj);
    // 设备配置信息
    deviceInfo_.saveToXml(pProjObj);
    // 设备标签变量组
    tagIOGroup_.saveToXml(pProjObj);


    Helper::writeString(szProjFile, projObjs.write());

    return true;
}


/**
 * @brief ProjectData::createOrOpenProjectData
 * @details 创建或打开工程数据
 * @param projFile 工程路径
 * @param projFile 工程名称
 * @return true-成功, false-失败
 */
bool ProjectData::createOrOpenProjectData(const QString &projPath, const QString &projName)
{
    QString fileName = projPath;

    if(fileName.endsWith('/'))
        fileName.chop(1);

    szProjPath_ = fileName;
    szProjName_ = projName;

    if(szProjPath_.isEmpty() || szProjName_.isEmpty()) {
        qWarning() << __FILE__ << __LINE__ <<__FUNCTION__
                   << "szProjPath_: " << szProjPath_ << " "
                   << "szProjName_: " << szProjName_;
        return false;
    }

    fileName = szProjPath_ + "/" + szProjName_ + ".pdt";

    if(dbPath_ != "") {
        if(dbData_ != Q_NULLPTR) {
            delete dbData_;
            dbData_ = Q_NULLPTR;
        }
    }

    dbPath_ = fileName;
    dbData_ = new ProjectDataSQLiteDatabase(fileName);
    if(dbData_->openDatabase()) {
        dbData_->createTables();
        return true;
    }

    return false;
}


/**
 * @brief ProjectData::getDBPath
 * @details 获取工程数据文件路径
 * @return 工程数据文件路径
 */
QString ProjectData::getDBPath() const
{
    return dbPath_;
}


/**
 * @brief ProjectData::GetAllProjectVariableName
 * @details 获取工程所有变量的名称
 * @param varList 存储变量列表
 * @param type IO, TMP, SYS, ALL
 */
void ProjectData::getAllTagName(QStringList &varList, const QString &type)
{
    if(dbData_ == Q_NULLPTR)
        return;

    varList.clear();
    QString szType = type.toUpper();

    //-------------设备变量------------------//
    if(szType == "ALL" || szType == "IO") {
        tagIO_.load(dbData_);
        for(int i=0; i<tagIO_.listTagIODBItem_.count(); i++) {
            TagIODBItem *pObj = tagIO_.listTagIODBItem_.at(i);
            varList << (QObject::tr("设备变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
        qDeleteAll(tagIO_.listTagIODBItem_);
        tagIO_.listTagIODBItem_.clear();
    }

    //-------------中间变量------------------//
    if(szType == "ALL" || szType == "TMP") {
        tagTmp_.load(dbData_);
        for(int i=0; i<tagTmp_.listTagTmpDBItem_.count(); i++) {
            TagTmpDBItem *pObj = tagTmp_.listTagTmpDBItem_.at(i);
            varList << (QObject::tr("中间变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
        qDeleteAll(tagTmp_.listTagTmpDBItem_);
        tagTmp_.listTagTmpDBItem_.clear();
    }

    //-------------系统变量------------------//
    if(szType == "ALL" || szType == "SYS") {
        tagSys_.load(dbData_);
        for(int i=0; i<tagSys_.listTagSysDBItem_.count(); i++) {
            TagSysDBItem *pObj = tagSys_.listTagSysDBItem_.at(i);
            varList << (QObject::tr("系统变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
        qDeleteAll(tagSys_.listTagSysDBItem_);
        tagSys_.listTagSysDBItem_.clear();
    }
}

/**
 * @brief ProjectData::getProjectPath
 * @details 获取工程路径
 * @param projectName 工程名称全路径
 * @return 工程路径
 */
QString ProjectData::getProjectPath(const QString &projectName) {
    QString path = QString();
    int pos = projectName.lastIndexOf("/");
    if (pos != -1) {
        path = projectName.left(pos);
    }
    return path;
}

/**
 * @brief ProjectData::getProjectNameWithSuffix
 * @details 获取包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称包含后缀
 */
QString ProjectData::getProjectNameWithSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.fileName();
}

/**
 * @brief ProjectData::getProjectNameWithOutSuffix
 * @details 获取不包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称不包含后缀
 */
QString ProjectData::getProjectNameWithOutSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.baseName();
}
