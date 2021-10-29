#include "ProjectInfoManager.h"

class ProjectInfoManager;

class ProjectInfoPrivate
{
public:
    ProjectInfoPrivate() { }

    QString m_projectName;
    QString m_projectDescription;
    QString m_projectPath;
    QString m_deviceType;
    int m_stationNumber;
    QString m_startPage;
    QString m_stationAddress;
    bool m_projectEncrypt;
    int m_pageScanPeriod;
    int m_dataScanPeriod;
};



ProjectInfoManager::ProjectInfoManager() : m_dPtr(new ProjectInfoPrivate())
{

}

ProjectInfoManager::~ProjectInfoManager()
{
    if(m_dPtr != NULL) {
        delete m_dPtr;
    }
}

/**
 * @brief ProjectInfoManager::openFromXml
 * @details 读取工程信息
 * @param pXmlObj
 * @return true-成功, false-失败
 */
bool ProjectInfoManager::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *projInfoObj = pXmlObj->getCurrentChild("project_info");
    if(projInfoObj == NULL) {
        return false;
    }
    m_dPtr->m_projectEncrypt = projInfoObj->getProperty("encrypt") == "1";
    m_dPtr->m_dataScanPeriod = projInfoObj->getProperty("data_scan").toUInt();
    m_dPtr->m_deviceType = projInfoObj->getProperty("device");
    m_dPtr->m_pageScanPeriod = projInfoObj->getProperty("page_scan").toUInt();
    m_dPtr->m_projectDescription = projInfoObj->getProperty("desc");
    m_dPtr->m_projectName = projInfoObj->getProperty("name");
    m_dPtr->m_projectPath = projInfoObj->getProperty("path");
    m_dPtr->m_startPage = projInfoObj->getProperty("start");
    m_dPtr->m_stationAddress = projInfoObj->getProperty("address");
    m_dPtr->m_stationNumber = projInfoObj->getProperty("number").toInt();
    return true;
}

/**
 * @brief ProjectInfoManager::saveToXml
 * @details 保存工程信息
 * @param pXmlObj
 * @return true-成功, false-失败
 */
bool ProjectInfoManager::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *projInfoObj = new XMLObject(pXmlObj);
    projInfoObj->setTagName("project_info");
    projInfoObj->setProperty("encrypt", m_dPtr->m_projectEncrypt ? "1" : "0");
    projInfoObj->setProperty("data_scan", QString::number(m_dPtr->m_dataScanPeriod));
    projInfoObj->setProperty("device", m_dPtr->m_deviceType);
    projInfoObj->setProperty("page_scan", QString::number(m_dPtr->m_pageScanPeriod));
    projInfoObj->setProperty("desc", m_dPtr->m_projectDescription);
    projInfoObj->setProperty("name", m_dPtr->m_projectName);
    projInfoObj->setProperty("path", m_dPtr->m_projectPath);
    projInfoObj->setProperty("start", m_dPtr->m_startPage);
    projInfoObj->setProperty("address", m_dPtr->m_stationAddress);
    projInfoObj->setProperty("number", QString::number(m_dPtr->m_stationNumber));
    return true;
}

QString ProjectInfoManager::getProjectName() const
{
    return m_dPtr->m_projectName;
}


void ProjectInfoManager::setProjectName(const QString &name)
{
    m_dPtr->m_projectName = name;
}

QString ProjectInfoManager::getProjectDescription() const
{
    return m_dPtr->m_projectDescription;
}


void ProjectInfoManager::setProjectDescription(const QString &desc)
{
    m_dPtr->m_projectDescription = desc;
}


QString ProjectInfoManager::getProjectPath() const
{
    return m_dPtr->m_projectPath;
}

void ProjectInfoManager::setProjectPath(const QString &path)
{
    m_dPtr->m_projectPath = path;
}

QString ProjectInfoManager::getDeviceType() const
{
    return m_dPtr->m_deviceType;
}

void ProjectInfoManager::setDeviceType(const QString &type)
{
    m_dPtr->m_deviceType = type;
}

int ProjectInfoManager::getStationNumber() const
{
    return m_dPtr->m_stationNumber;
}

void ProjectInfoManager::setStationNumber(int number)
{
    m_dPtr->m_stationNumber = number;
}

QString ProjectInfoManager::getStartPage() const
{
    return m_dPtr->m_startPage;
}

void ProjectInfoManager::setStartPage(const QString &page)
{
    m_dPtr->m_startPage = page;
}

QString ProjectInfoManager::getStationAddress() const
{
    return m_dPtr->m_stationAddress;
}

void ProjectInfoManager::setStationAddress(const QString &address)
{
    m_dPtr->m_stationAddress = address;
}

bool ProjectInfoManager::getProjectEncrypt()
{
    return m_dPtr->m_projectEncrypt;
}

void ProjectInfoManager::setProjectEncrypt(bool encrypt)
{
    m_dPtr->m_projectEncrypt = encrypt;
}

int ProjectInfoManager::getPageScanPeriod() const
{
    return m_dPtr->m_pageScanPeriod;
}

void ProjectInfoManager::setPageScanPeriod(int period)
{
    m_dPtr->m_pageScanPeriod = period;
}

int ProjectInfoManager::getDataScanPeriod() const
{
    return m_dPtr->m_dataScanPeriod;
}

void ProjectInfoManager::setDataScanPeriod(int period)
{
    m_dPtr->m_dataScanPeriod = period;
}

bool getWidthHeight(const QString &input, int &width, int &height)
{

    QString str = input;
    int posLeft = str.indexOf("(");
    int posRight = str.indexOf(")");

    if(posLeft == -1 || posRight == -1) {
        return false;
    }

    QString strWidthHeight = str.mid(posLeft + 1, posRight - posLeft - 1);

    QStringList listWidthHeight = strWidthHeight.split('*');

    if(listWidthHeight.size() != 2) {
        return false;
    }

    width = listWidthHeight.at(0).toInt();
    height = listWidthHeight.at(1).toInt();

    return true;
}

/**
 * @brief ProjectInfoManager::getGraphPageWidth
 * @return 画面宽度
 */
int ProjectInfoManager::getGraphPageWidth()
{
    QString devType = m_dPtr->m_deviceType;
    int width = 0;
    int height = 0;
    if(getWidthHeight(devType, width, height)) {
        return width;
    }
    return 0;
}

/**
 * @brief ProjectInfoManager::getGraphPageHeight
 * @return 画面高度
 */
int ProjectInfoManager::getGraphPageHeight()
{
    QString devType = m_dPtr->m_deviceType;
    int width = 0;
    int height = 0;
    if(getWidthHeight(devType, width, height)) {
        return height;
    }
    return 0;
}

