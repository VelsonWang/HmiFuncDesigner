#include "qprojectcore.h"
#include "xmlobject.h"
#include "qpagemanager.h"
#include "host/qprojecthost.h"
#include "property/qabstractproperty.h"
#include "qcommonstruct.h"
#include "Helper.h"
#include "DataAES.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QUuid>
#include <QScriptEngine>


QProjectCore::QProjectCore(QObject *parent)
    : QObject(parent),
      m_pProjectHostObj(new QProjectHost),
      m_bOpen(false),
      m_pPageMgrObj(new QPageManager)
{
    m_version = "V1.0.0";
    m_path = "";
    m_name = "";
    m_pProjectHostObj->setPageManager(m_pPageMgrObj);
}


QProjectCore::~QProjectCore()
{
    close();
    delete m_pPageMgrObj;
}

void QProjectCore::close()
{
    if(m_bOpen) {
        notifyClosed();
        if(m_pProjectHostObj != NULL) {
            delete m_pProjectHostObj;
            m_pProjectHostObj = NULL;
        }
        m_path = "";
        m_pPageMgrObj->clear();
        m_bOpen = false;
    }
}

QPageManager* QProjectCore::getPageManager()
{
    return m_pPageMgrObj;
}

QAbstractHost* QProjectCore::getProjectHost()
{
    return m_pProjectHostObj;
}

bool QProjectCore::isOpened()
{
    return m_bOpen;
}

void QProjectCore::initScriptEngine()
{
    //    initScriptEngine(m_pProjectHostObj);
    //    foreach(QAbstractHost* h, m_pPageMgrObj->getPages()) {
    //        initScriptEngine(h);
    //    }
}

void QProjectCore::initScriptEngine(QAbstractHost *host)
{
    /*
    QScriptEngine *engine = host->getScriptEngine();
    QScriptValue global = engine->globalObject();
    QScriptValue temp = getScriptObject(m_pProjectHostObj, engine);

    global.setProperty("global", temp);

    bool keyboard = host->property("title").toString() == "keyboard";
    bool project = host->property("title").toString() == "Project";

    if(!keyboard || project) {
        foreach(QAbstractHost* h, m_pPageMgrObj->getPages()) {
            if(project || h->property("title").toString() == "keyboard") {
                temp = getScriptObject(host, engine);
                global.setProperty(h->getPropertyValue("objectName").toString(), temp);
            }
        }
    }

    temp = getScriptObject(host, engine);
    global.setProperty("self", temp);
    if(!keyboard || project) {
        if(host->getParent() != NULL) {
            temp = getScriptObject(host->getParent(), engine);
            global.setProperty("parent", temp);
        }
    } else {
        temp = getScriptObject(host, engine);
        global.setProperty(host->getPropertyValue("objectName").toString(), temp);
    }

    engine->setGlobalObject(global);

    foreach(QAbstractHost* h, host->getChildren()) {
        initScriptEngine(h);
    }
    */
}

QScriptValue QProjectCore::getScriptObject(QAbstractHost *host, QScriptEngine *engine)
{
    /*
    QScriptValue value = engine->newQObject(host);

    foreach(QAbstractHost* h, host->getChildren()) {
        QScriptValue temp = getScriptObject(h, engine);
        value.setProperty(h->getPropertyValue("objectName").toString(), temp);
    }
    return value;
    */
}


bool QProjectCore::openFromXml(const QString &szProjFile)
{
    close();

    QFileInfo fileInfoSrc(szProjFile);
    if(fileInfoSrc.size() <= 512) {
        return false;
    }

    // 读取文件头信息
    QFile fileProj;
    fileProj.setFileName(szProjFile);
    if(!fileProj.open(QIODevice::ReadOnly)) {
        return false;
    }

    quint8 buf[1024] = {0};
    quint16 wSize = (sizeof(TFileHeader) < 512) ? 512 : sizeof(TFileHeader);

    if(fileProj.read((char *)buf, wSize) != wSize) {
        fileProj.close();
        return false;
    }

    memcpy((void *)&m_headerObj, (void *)buf, sizeof(TFileHeader));

    // 读取工程数据
    QByteArray baTmpProjData;
    baTmpProjData.resize(m_headerObj.dwProjSize);
    if(fileProj.read((char *)baTmpProjData.data(), m_headerObj.dwProjSize) != m_headerObj.dwProjSize) {
        fileProj.close();
        return false;
    }
    fileProj.close();

    QByteArray baProjData;
    if(m_headerObj.byEncrypt) {
        if(DataAES::Decrypt(baTmpProjData, baProjData, AES_DEFAULT_KEY) != 0) {
            return false;
        }
    } else {
        baProjData = baTmpProjData;
    }

    QString szProjData = QString::fromUtf8(baProjData);

    XMLObject xml;
    if(!xml.load(szProjData, NULL)) {
        return false;
    }

    QList<XMLObject*> projObjs = xml.getChildren();
    foreach(XMLObject* pProjObj, projObjs) {
        // 工程信息管理
        m_projInfoMgr.openFromXml(pProjObj);
        // 网络配置
        m_netSetting.openFromXml(pProjObj);
        // 数据库配置
        m_dbSetting.openFromXml(pProjObj);
        // 用户权限
        m_userAuthority.openFromXml(pProjObj);
        // 设备配置信息
        m_deviceInfo.openFromXml(pProjObj);

        XMLObject *pTagsObj = pProjObj->getCurrentChild("tags");
        if(pTagsObj != NULL) {
            // 标签变量组
            m_tagMgr.openFromXml(pTagsObj);
        }

        // 脚本
        m_script.openFromXml(pProjObj);
        // 图片资源管理
        m_picResMgr.openFromXml(pProjObj);

        // 加载画面
        XMLObject *pPagesObj = pProjObj->getCurrentChild("forms");
        if(pPagesObj != NULL) {
            m_pPageMgrObj->load(pPagesObj);
        }
    }

    m_bOpen = true;
    emit notifyOpened();

    return true;
}

/**
 * @brief QProjectCore::GetAllProjectVariableName
 * @details 获取工程所有变量的名称
 * @param varList 存储变量列表
 * @param type IO, TMP, SYS, ALL
 */
void QProjectCore::getAllTagName(QStringList &varList, const QString &type)
{
    varList.clear();
    QString szType = type.toUpper();

    foreach(Tag *pTagObj, m_tagMgr.m_vecTags) {
        //-------------设备变量------------------//
        if(szType == "ALL" || szType == "IO") {
            if(pTagObj->m_szDevType != "MEMORY" && pTagObj->m_szDevType != "SYSTEM") {
                varList << (QObject::tr("设备变量_") + pTagObj->m_szName + "[" + QString::number(pTagObj->m_iID) + "]");
            }
        }

        //-------------中间变量------------------//
        if(szType == "ALL" || szType == "TMP") {
            if(pTagObj->m_szDevType != "MEMORY") {
                varList << (QObject::tr("内存变量_") + pTagObj->m_szName + "[" + QString::number(pTagObj->m_iID) + "]");
            }
        }

        //-------------系统变量------------------//
        if(szType == "ALL" || szType == "SYS") {
            if(pTagObj->m_szDevType != "SYSTEM") {
                varList << (QObject::tr("系统变量_") + pTagObj->m_szName + "[" + QString::number(pTagObj->m_iID) + "]");
            }
        }
    }
}

/**
 * @brief QProjectCore::getProjectPath
 * @details 获取工程路径
 * @param projectName 工程名称全路径
 * @return 工程路径
 */
QString QProjectCore::getProjectPath(const QString &projectName)
{
    QString path = QString();
    int pos = projectName.lastIndexOf("/");
    if (pos != -1) {
        path = projectName.left(pos);
    }
    return path;
}

/**
 * @brief QProjectCore::getProjectNameWithSuffix
 * @details 获取包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称包含后缀
 */
QString QProjectCore::getProjectNameWithSuffix(const QString &projectName)
{
    QFileInfo projFileInfo(projectName);
    return projFileInfo.fileName();
}

/**
 * @brief QProjectCore::getProjectNameWithOutSuffix
 * @details 获取不包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称不包含后缀
 */
QString QProjectCore::getProjectNameWithOutSuffix(const QString &projectName)
{
    QFileInfo projFileInfo(projectName);
    return projFileInfo.baseName();
}

