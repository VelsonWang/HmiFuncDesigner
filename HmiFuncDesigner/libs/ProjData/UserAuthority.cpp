#include "UserAuthority.h"

UserAuthority::UserAuthority() {
    listUserAuthority_.clear();
}

UserAuthority::~UserAuthority() {
    qDeleteAll(listUserAuthority_);
    listUserAuthority_.clear();
}


bool UserAuthority::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(listUserAuthority_);
    listUserAuthority_.clear();
    XMLObject *pUsersObj = pXmlObj->getCurrentChild("users");
    if(pUsersObj == Q_NULLPTR) return false;
    QVector<XMLObject* > listUsersObj = pUsersObj->getCurrentChildren("user");
    foreach(XMLObject* pUserObj, listUsersObj) {
        UserAuthorityPrivate *pObj = new UserAuthorityPrivate();
        pObj->iID_ = pUserObj->getProperty("id").toInt();
        pObj->szIndex_ = pUserObj->getProperty("num");
        pObj->szName_ = pUserObj->getProperty("name");
        pObj->szPassword_ = pUserObj->getProperty("pass");
        pObj->szAuthority_ = pUserObj->getProperty("auth");
        pObj->szComments_ = pUserObj->getProperty("com");
        pObj->szName2_ = pUserObj->getProperty("name2");
        pObj->szName3_ = pUserObj->getProperty("name3");
        pObj->szName4_ = pUserObj->getProperty("name4");
        pObj->szName5_ = pUserObj->getProperty("name5");
        pObj->bLogin_ = pUserObj->getProperty("login") == "1";
        pObj->bLogout_= pUserObj->getProperty("logout") == "1";
        listUserAuthority_.append(pObj);
    }
    return true;
}


bool UserAuthority::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pUsersObj = new XMLObject(pXmlObj);
    pUsersObj->setTagName("users");

    for(int i=0; i<listUserAuthority_.count(); i++) {
        UserAuthorityPrivate *pObj = listUserAuthority_.at(i);
        XMLObject *pUserObj = new XMLObject(pUsersObj);
        pUserObj->setTagName("user");
        pUserObj->setProperty("id", QString::number(pObj->iID_));
        pUserObj->setProperty("num", pObj->szIndex_);
        pUserObj->setProperty("name", pObj->szName_);
        pUserObj->setProperty("pass", pObj->szPassword_);
        pUserObj->setProperty("auth", pObj->szAuthority_);
        pUserObj->setProperty("com", pObj->szComments_);
        pUserObj->setProperty("name2", pObj->szName2_);
        pUserObj->setProperty("name3", pObj->szName3_);
        pUserObj->setProperty("name4", pObj->szName4_);
        pUserObj->setProperty("name5", pObj->szName5_);
        pUserObj->setProperty("login", pObj->bLogin_ ? "1" : "0");
        pUserObj->setProperty("logout", pObj->bLogout_ ? "1" : "0");
    }

    return true;
}


int UserAuthority::getUserAuthorityID(int index) {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->iID_;
    return -1;
}

int UserAuthority::getCount() {
    return listUserAuthority_.count();
}


QString UserAuthority::getIndex(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szIndex_;
    return QString();
}

void UserAuthority::setIndex(int index, const QString &szIndex) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szIndex_ = szIndex;
}

QString UserAuthority::getName(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szName_;
    return QString();
}

void UserAuthority::setName(int index, const QString &name) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szName_ = name;
}

QString UserAuthority::getPassword(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szPassword_;
    return QString();
}

void UserAuthority::setPassword(int index, const QString &password) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szPassword_ = password;
}

QString UserAuthority::getAuthority(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szAuthority_;
    return QString();
}

void UserAuthority::setAuthority(int index, const QString &authority) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szAuthority_ = authority;
}

QString UserAuthority::getComments(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szComments_;
    return QString();
}

void UserAuthority::setComments(int index, const QString &comments) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szComments_ = comments;
}

QString UserAuthority::getName2(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szName2_;
    return QString();
}

void UserAuthority::setName2(int index, const QString &name) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szName2_ = name;
}

QString UserAuthority::getName3(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szName3_;
    return QString();
}

void UserAuthority::setName3(int index, const QString &name) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szName3_ = name;
}

QString UserAuthority::getName4(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szName4_;
    return QString();
}

void UserAuthority::setName4(int index, const QString &name) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szName4_ = name;
}

QString UserAuthority::getName5(int index) const {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->szName5_;
    return QString();
}

void UserAuthority::setName5(int index, const QString &name) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->szName5_ = name;
}

bool UserAuthority::isLogin(int index) {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->bLogin_;
    return false;
}

void UserAuthority::setLogin(int index, bool login) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->bLogin_ = login;
}

bool UserAuthority::isLogout(int index) {
    if(index < listUserAuthority_.count())
        return listUserAuthority_.at(index)->bLogout_;
    return false;
}

void UserAuthority::setLogout(int index, bool logout) {
    if(index < listUserAuthority_.count())
        listUserAuthority_.at(index)->bLogout_ = logout;
}

