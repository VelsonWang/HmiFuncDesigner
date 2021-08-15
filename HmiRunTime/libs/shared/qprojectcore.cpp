#include "qprojectcore.h"
#include "xmlobject.h"
#include "qpagemanager.h"
#include "host/qprojecthost.h"
#include "host/qformhost.h"
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
      m_pPageManagerObj(new QPageManager)
{
    m_szProjVersion = "V1.0.0";
    m_szProjPath = "";
    m_szProjName = "";
    m_pProjectHostObj->setPageManager(m_pPageManagerObj);
    connect(m_pPageManagerObj, SIGNAL(host_name_changed(QAbstractHost*)),
            this, SLOT(onFormRefresh(QAbstractHost*)));
}


QProjectCore::~QProjectCore()
{
    close();
    delete m_pPageManagerObj;
}

void QProjectCore::close()
{
    if(m_bOpen) {
        notifyClosed();
        if(m_pProjectHostObj != Q_NULLPTR) {
            delete m_pProjectHostObj;
            m_pProjectHostObj = Q_NULLPTR;
        }
        m_szProjPath = "";
        m_pPageManagerObj->clear();
        m_bOpen = false;
    }
}

QPageManager* QProjectCore::getPageManager()
{
    return m_pPageManagerObj;
}

QAbstractHost* QProjectCore::getProjectHost()
{
    return m_pProjectHostObj;
}

bool QProjectCore::createNewProj(const QString &szfile)
{
    close();
    int index = szfile.lastIndexOf("/");
    m_szProjPath = szfile.left(index);
    index = m_szProjPath.lastIndexOf("/");
    m_szProjName = m_szProjPath.mid(index + 1);
    m_pPageManagerObj->load(Q_NULLPTR);
    m_bOpen = true;
    emit notifyOpened();
    return true;
}


bool QProjectCore::isOpened()
{
    return m_bOpen;
}

void QProjectCore::onFormRefresh(QAbstractHost *form)
{
    if(form != Q_NULLPTR && (form->getHostType() != "form" || form->getParent() != Q_NULLPTR)) {
        return;
    }
    QAbstractProperty* pro = m_pProjectHostObj->getProperty("start_page");
    if(pro != Q_NULLPTR) {
        QList<QAbstractHost*> list = m_pPageManagerObj->getPages_by_title("form");
        tagComboItem item;
        ComboItems items;
        foreach(QAbstractHost* host, list) {
            item.m_text = host->getPropertyValue("objectName").toString();
            item.m_value = host->getUuid();
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);
        if(form != Q_NULLPTR && pro->get_value().toString() == form->getUuid()) {
            pro->set_value("");
            pro->set_value(form->getUuid());
        }
    }
}

QAbstractHost* QProjectCore::getHostByUuid(const QString &uuid)
{
    QList<QAbstractHost*> list=m_pPageManagerObj->getPages();
    list.append(m_pProjectHostObj);

    while(list.size() > 0) {
        QAbstractHost* h=list.takeFirst();
        if(h->getUuid() == uuid) {
            return h;
        }
        list += h->getChildren();
    }

    return Q_NULLPTR;
}


void QProjectCore::initScriptEngine()
{
    initScriptEngine(m_pProjectHostObj);
    foreach(QAbstractHost* h,m_pPageManagerObj->getPages()) {
        initScriptEngine(h);
    }
}

void QProjectCore::initScriptEngine(QAbstractHost *host)
{
    QScriptEngine *engine = host->getScriptEngine();
    QScriptValue global = engine->globalObject();
    QScriptValue temp = getScriptObject(m_pProjectHostObj, engine);

    global.setProperty("global", temp);

    bool keyboard = host->property("title").toString() == "keyboard";
    bool project = host->property("title").toString() == "Project";

    if(!keyboard || project) {
        foreach(QAbstractHost* h, m_pPageManagerObj->getPages()) {
            if(project || h->property("title").toString() == "keyboard") {
                temp = getScriptObject(host,engine);
                global.setProperty(h->getPropertyValue("objectName").toString(), temp);
            }
        }
    }

    temp = getScriptObject(host,engine);
    global.setProperty("self",temp);
    if(!keyboard || project) {
        if(host->getParent() != Q_NULLPTR) {
            temp = getScriptObject(host->getParent(), engine);
            global.setProperty("parent", temp);
        }
    } else {
        temp = getScriptObject(host, engine);
        global.setProperty(host->getPropertyValue("objectName").toString(), temp);
    }

    engine->setGlobalObject(global);

    foreach(QAbstractHost* h,host->getChildren()) {
        initScriptEngine(h);
    }
}

QScriptValue QProjectCore::getScriptObject(QAbstractHost *host, QScriptEngine *engine)
{
    QScriptValue value = engine->newQObject(host);

    foreach(QAbstractHost* h, host->getChildren()) {
        QScriptValue temp = getScriptObject(h, engine);
        value.setProperty(h->getPropertyValue("objectName").toString(), temp);
    }
    return value;
}


bool QProjectCore::openFromXml(const QString &szProjFile)
{
    close();

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
        XMLObject *pPagesObj = pProjObj->getCurrentChild("forms");
        if(pPagesObj != Q_NULLPTR) {
            m_pPageManagerObj->load(pPagesObj);
        }
    }

    m_bOpen = true;
    emit notifyOpened();

    return true;
}


bool QProjectCore::saveToXml(const QString &szProjFile)
{
    if(!m_bOpen) {
        return false;
    }

    XMLObject projObjs;
    projObjs.setTagName("projects");

    XMLObject *pProjObj = new XMLObject(&projObjs);
    pProjObj->setTagName("project");
    pProjObj->setProperty("application_version", m_szProjVersion);

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
    pPagesObj->setTagName("forms");
    m_pPageManagerObj->save(pPagesObj);

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
 * @brief QProjectCore::GetAllProjectVariableName
 * @details 获取工程所有变量的名称
 * @param varList 存储变量列表
 * @param type IO, TMP, SYS, ALL
 */
void QProjectCore::getAllTagName(QStringList &varList, const QString &type)
{
    varList.clear();
    QString szType = type.toUpper();

    foreach(Tag *pTagObj, tagMgr_.m_vecTags) {
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
QString QProjectCore::getProjectPath(const QString &projectName) {
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
QString QProjectCore::getProjectNameWithSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.fileName();
}

/**
 * @brief QProjectCore::getProjectNameWithOutSuffix
 * @details 获取不包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称不包含后缀
 */
QString QProjectCore::getProjectNameWithOutSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.baseName();
}

/**
 * @brief QProjectCore::getAllElementIDName
 * @details 获取工程所有控件的ID名称
 * @param szIDList 所有控件的ID名称
 */
void QProjectCore::getAllElementIDName(QStringList &szIDList)
{
    m_pPageManagerObj->getAllElementIDName(szIDList);
}

/**
 * @brief QProjectCore::getAllGraphPageName
 * @details 获取工程所有画面名称
 * @param szList 所有画面名称
 */
void QProjectCore::getAllGraphPageName(QStringList &szList)
{
    m_pPageManagerObj->getAllPageName(szList);
}


