#include "userauthority.h"

UserAuthority::UserAuthority()
{
    m_userAuthority.clear();
}

UserAuthority::~UserAuthority()
{
    qDeleteAll(m_userAuthority);
    m_userAuthority.clear();
}


bool UserAuthority::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(m_userAuthority);
    m_userAuthority.clear();
    XMLObject *pUsersObj = pXmlObj->getCurrentChild("users");
    if(pUsersObj == NULL) {
        return false;
    }
    QVector<XMLObject* > listUsersObj = pUsersObj->getCurrentChildren("user");
    foreach(XMLObject* pUserObj, listUsersObj) {
        UserAuthorityPrivate *pObj = new UserAuthorityPrivate();
        pObj->m_id = pUserObj->getProperty("id").toInt();
        pObj->m_index = pUserObj->getProperty("num");
        pObj->m_name = pUserObj->getProperty("name");
        pObj->m_password = pUserObj->getProperty("pass");
        pObj->m_authority = pUserObj->getProperty("auth");
        pObj->m_comments = pUserObj->getProperty("com");
        pObj->m_name2 = pUserObj->getProperty("name2");
        pObj->m_name3 = pUserObj->getProperty("name3");
        pObj->m_name4 = pUserObj->getProperty("name4");
        pObj->m_name5 = pUserObj->getProperty("name5");
        pObj->m_login = pUserObj->getProperty("login") == "1";
        pObj->m_logout = pUserObj->getProperty("logout") == "1";
        m_userAuthority.append(pObj);
    }
    return true;
}


bool UserAuthority::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pUsersObj = new XMLObject(pXmlObj);
    pUsersObj->setTagName("users");

    for(int i = 0; i < m_userAuthority.count(); i++) {
        UserAuthorityPrivate *pObj = m_userAuthority.at(i);
        XMLObject *pUserObj = new XMLObject(pUsersObj);
        pUserObj->setTagName("user");
        pUserObj->setProperty("id", QString::number(pObj->m_id));
        pUserObj->setProperty("num", pObj->m_index);
        pUserObj->setProperty("name", pObj->m_name);
        pUserObj->setProperty("pass", pObj->m_password);
        pUserObj->setProperty("auth", pObj->m_authority);
        pUserObj->setProperty("com", pObj->m_comments);
        pUserObj->setProperty("name2", pObj->m_name2);
        pUserObj->setProperty("name3", pObj->m_name3);
        pUserObj->setProperty("name4", pObj->m_name4);
        pUserObj->setProperty("name5", pObj->m_name5);
        pUserObj->setProperty("login", pObj->m_login ? "1" : "0");
        pUserObj->setProperty("logout", pObj->m_logout ? "1" : "0");
    }

    return true;
}


int UserAuthority::getUserAuthorityID(int index)
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_id;
    }
    return -1;
}

int UserAuthority::getCount()
{
    return m_userAuthority.count();
}


QString UserAuthority::getIndex(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_index;
    }
    return QString();
}

void UserAuthority::setIndex(int index, const QString &szIndex)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_index = szIndex;
    }
}

QString UserAuthority::getName(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_name;
    }
    return QString();
}

void UserAuthority::setName(int index, const QString &name)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_name = name;
    }
}

QString UserAuthority::getPassword(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_password;
    }
    return QString();
}

void UserAuthority::setPassword(int index, const QString &password)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_password = password;
    }
}

QString UserAuthority::getAuthority(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_authority;
    }
    return QString();
}

void UserAuthority::setAuthority(int index, const QString &authority)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_authority = authority;
    }
}

QString UserAuthority::getComments(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_comments;
    }
    return QString();
}

void UserAuthority::setComments(int index, const QString &comments)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_comments = comments;
    }
}

QString UserAuthority::getName2(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_name2;
    }
    return QString();
}

void UserAuthority::setName2(int index, const QString &name)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_name2 = name;
    }
}

QString UserAuthority::getName3(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_name3;
    }
    return QString();
}

void UserAuthority::setName3(int index, const QString &name)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_name3 = name;
    }
}

QString UserAuthority::getName4(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_name4;
    }
    return QString();
}

void UserAuthority::setName4(int index, const QString &name)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_name4 = name;
    }
}

QString UserAuthority::getName5(int index) const
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_name5;
    }
    return QString();
}

void UserAuthority::setName5(int index, const QString &name)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_name5 = name;
    }
}

bool UserAuthority::isLogin(int index)
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_login;
    }
    return false;
}

void UserAuthority::setLogin(int index, bool login)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_login = login;
    }
}

bool UserAuthority::isLogout(int index)
{
    if(index < m_userAuthority.count()) {
        return m_userAuthority.at(index)->m_logout;
    }
    return false;
}

void UserAuthority::setLogout(int index, bool logout)
{
    if(index < m_userAuthority.count()) {
        m_userAuthority.at(index)->m_logout = logout;
    }
}

