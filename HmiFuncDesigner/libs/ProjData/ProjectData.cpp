#include "ProjectData.h"
#include "XMLObject.h"
#include "Helper.h"
#include "DataAES.h"
#include <QFileInfo>



ProjectData::ProjectData() : szProjVersion_("V1.0.0")
{
    this->pImplGraphPageSaveLoadObj_ = Q_NULLPTR;
    szProjPath_ = "";
    szProjName_ = "";
}

ProjectData::~ProjectData()
{

}


ProjectData* ProjectData::getInstance()
{
    static ProjectData instance_;
    return &instance_;
}


bool ProjectData::openFromXml(const QString &szProjFile)
{
    QFileInfo fileInfoSrc(szProjFile);
    if(fileInfoSrc.size() <= 512) return false;

    // 读取文件头信息
    QFile fileProj;
    fileProj.setFileName(szProjFile);
    if(!fileProj.open(QIODevice::ReadOnly)) return false;

    quint8 buf[1024] = {0};
    quint16 wSize = (sizeof(TFileHeader) < 512) ? 512 : sizeof(TFileHeader);

    if(fileProj.read((char *)buf, wSize) != wSize) {
        fileProj.close();
        return false;
    }

    memcpy((void *)&headerObj_, (void *)buf, sizeof(TFileHeader));

    // 读取工程数据
    QByteArray baTmpProjData;
    baTmpProjData.resize(headerObj_.dwProjSize);
    if(fileProj.read((char *)baTmpProjData.data(), headerObj_.dwProjSize) != headerObj_.dwProjSize) {
        fileProj.close();
        return false;
    }
    fileProj.close();

    QByteArray baProjData;
    if(headerObj_.byEncrypt) {
        if(DataAES::Decrypt(baTmpProjData, baProjData, AES_DEFAULT_KEY) != 0) return false;
    } else {
        baProjData = baTmpProjData;
    }

    QString szProjData = QString::fromUtf8(baProjData);

    XMLObject xml;
    if(!xml.load(szProjData, Q_NULLPTR)) return false;

    QVector<XMLObject*> projObjs = xml.getChildren();
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

        XMLObject *pTagsObj = pProjObj->getCurrentChild("tags");
        if(pTagsObj != Q_NULLPTR) {
            // 标签变量组
            tagMgr_.openFromXml(pTagsObj);
        }

        // 脚本
        script_.openFromXml(pProjObj);
        // 图片资源管理
        pictureResourceMgr_.openFromXml(pProjObj);

        // 加载画面
        XMLObject *pPagesObj = pProjObj->getCurrentChild("pages");
        if(pPagesObj != Q_NULLPTR) {
            if(pImplGraphPageSaveLoadObj_) {
                pImplGraphPageSaveLoadObj_->openFromXml(pPagesObj);
            }
        }
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

    XMLObject *pTagsObj = new XMLObject(pProjObj);
    pTagsObj->setTagName("tags");
    // 标签变量组
    tagMgr_.saveToXml(pTagsObj);

    // 脚本
    script_.saveToXml(pProjObj);
    // 图片资源管理
    pictureResourceMgr_.saveToXml(pProjObj);

    // 保存画面
    XMLObject *pPagesObj = new XMLObject(pProjObj);
    pPagesObj->setTagName("pages");
    if(pImplGraphPageSaveLoadObj_) {
        pImplGraphPageSaveLoadObj_->saveToXml(pPagesObj);
    }

    QByteArray baProjData;
    QByteArray baTmpProjData = projObjs.write().toUtf8();
    if(headerObj_.byEncrypt) {
        if(DataAES::Encrypt(baTmpProjData, baProjData, AES_DEFAULT_KEY) != 0) return false;
    } else {
        baProjData = baTmpProjData;
    }

    QFile fileProj;
    fileProj.setFileName(szProjFile);
    if(!fileProj.open(QIODevice::WriteOnly|QIODevice::Truncate))
        return false;

    // 写文件头
    quint8 buf[1024] = {0};
    headerObj_.wSize = (sizeof(TFileHeader) < 512) ? 512 : sizeof(TFileHeader);
    headerObj_.wVersion = 0x0001;
    headerObj_.dwProjSize = baProjData.length();
    memcpy((void *)buf, (void *)&headerObj_, sizeof(TFileHeader));
    if(fileProj.write((const char *)buf, headerObj_.wSize) != headerObj_.wSize) {
        fileProj.close();
        return false;
    }

    if(fileProj.write(baProjData, baProjData.size()) != baProjData.size()) {
        fileProj.close();
        return false;
    }

    fileProj.close();

    return true;
}


/**
 * @brief ProjectData::GetAllProjectVariableName
 * @details 获取工程所有变量的名称
 * @param varList 存储变量列表
 * @param type IO, TMP, SYS, ALL
 */
void ProjectData::getAllTagName(QStringList &varList, const QString &type)
{
    varList.clear();
    QString szType = type.toUpper();

    foreach(Tag *pTagObj, tagMgr_.m_vecTags) {
        //-------------设备变量------------------//
        if(szType == "ALL" || szType == "IO") {
            if(pTagObj->m_szDevType != "MEMORY" && pTagObj->m_szDevType != "SYSTEM") {
                varList << (QObject::tr("设备变量.") + pTagObj->m_szName + "[" + QString::number(pTagObj->m_iID) + "]");
            }
        }

        //-------------中间变量------------------//
        if(szType == "ALL" || szType == "TMP") {
            if(pTagObj->m_szDevType != "MEMORY") {
                varList << (QObject::tr("内存变量.") + pTagObj->m_szName + "[" + QString::number(pTagObj->m_iID) + "]");
            }
        }

        //-------------系统变量------------------//
        if(szType == "ALL" || szType == "SYS") {
            if(pTagObj->m_szDevType != "SYSTEM") {
                varList << (QObject::tr("系统变量.") + pTagObj->m_szName + "[" + QString::number(pTagObj->m_iID) + "]");
            }
        }
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

/**
 * @brief ProjectData::getAllElementIDName
 * @details 获取工程所有控件的ID名称
 * @param szIDList 所有控件的ID名称
 */
void ProjectData::getAllElementIDName(QStringList &szIDList) {
    if(pImplGraphPageSaveLoadObj_) {
        pImplGraphPageSaveLoadObj_->getAllElementIDName(szIDList);
    }
}

/**
 * @brief ProjectData::getAllGraphPageName
 * @details 获取工程所有画面名称
 * @param szList 所有画面名称
 */
void ProjectData::getAllGraphPageName(QStringList &szList) {
    if(pImplGraphPageSaveLoadObj_) {
        pImplGraphPageSaveLoadObj_->getAllGraphPageName(szList);
    }
}


