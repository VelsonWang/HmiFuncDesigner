#include "UserAuthority.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>



UserAuthority::UserAuthority() {
    listUserAuthority_.clear();
}

UserAuthority::~UserAuthority() {
    qDeleteAll(listUserAuthority_);
    listUserAuthority_.clear();
}


/**
 * @brief UserAuthority::load
 * @details 读取用户权限
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool UserAuthority::load(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;

    bool ret = query.exec("select * from t_user_authority");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_user_authority")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    qDeleteAll(listUserAuthority_);
    listUserAuthority_.clear();

    while (query.next()) {
        rec = query.record();
        UserAuthorityPrivate *pObj = new UserAuthorityPrivate();
        pObj->iID_ = rec.value("id").toInt();
        pObj->szIndex_ = rec.value("number").toString();
        pObj->szName_ = rec.value("name").toString();
        pObj->szPassword_ = rec.value("password").toString();
        pObj->szAuthority_ = rec.value("authority").toString();
        pObj->szComments_ = rec.value("comments").toString();
        pObj->szName2_ = rec.value("name2").toString();
        pObj->szName3_ = rec.value("name3").toString();
        pObj->szName4_ = rec.value("name4").toString();
        pObj->szName5_ = rec.value("name5").toString();
        pObj->bLogin_ = rec.value("login").toInt() > 0 ? true : false;
        pObj->bLogout_ = rec.value("logout").toInt() > 0 ? true : false;
        listUserAuthority_.append(pObj);
    }

    return ret;
}

/**
 * @brief UserAuthority::save
 * @details 保存用户权限
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool UserAuthority::save(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    pDB->db_.transaction();
    for(int i=0; i<listUserAuthority_.count(); i++) {
        UserAuthorityPrivate *pObj = listUserAuthority_.at(i);
        query.prepare("update t_user_authority set number = :num, "
                      "name = :name, password = :pass, authority = :auth, comments = :com, "
                      "name2 = :name2, name3 = :name3, name4 = :name4, name5 = :name5, "
                      "login = :login, logout = :logout where id = :id");

        query.bindValue(":id", pObj->iID_);
        query.bindValue(":num", pObj->szIndex_);
        query.bindValue(":name", pObj->szName_);
        query.bindValue(":pass", pObj->szPassword_);
        query.bindValue(":auth", pObj->szAuthority_);
        query.bindValue(":com", pObj->szComments_);
        query.bindValue(":name2", pObj->szName2_);
        query.bindValue(":name3", pObj->szName3_);
        query.bindValue(":name4", pObj->szName4_);
        query.bindValue(":name5", pObj->szName5_);
        query.bindValue(":login", pObj->bLogin_ ? 1 : 0);
        query.bindValue(":logout", pObj->bLogout_ ? 1 : 0);

        ret = query.exec();
        if(!ret) {
            LogError(QString("update record: %1 failed! %2 ,error: %3!")
                     .arg("t_user_authority")
                     .arg(query.lastQuery())
                     .arg(query.lastError().text()));
            pDB->db_.rollback();
        }
    }
    pDB->db_.commit();

    return ret;
}


bool UserAuthority::insert(ProjectDataSQLiteDatabase *pDB,
                           UserAuthorityPrivate *pObj) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("insert into t_user_authority (number, "
                  "name, password, authority, comments, "
                  "name2, name3, name4, name5, "
                  "login, logout) values (:num, "
                  ":name, :pass, :auth, :com, "
                  ":name2, :name3, :name4, :name5, "
                  ":login, :logout)");
    query.bindValue(":num", pObj->szIndex_);
    query.bindValue(":name", pObj->szName_);
    query.bindValue(":pass", pObj->szPassword_);
    query.bindValue(":auth", pObj->szAuthority_);
    query.bindValue(":com", pObj->szComments_);
    query.bindValue(":name2", pObj->szName2_);
    query.bindValue(":name3", pObj->szName3_);
    query.bindValue(":name4", pObj->szName4_);
    query.bindValue(":name5", pObj->szName5_);
    query.bindValue(":login", pObj->bLogin_ ? 1 : 0);
    query.bindValue(":logout", pObj->bLogout_ ? 1 : 0);

    ret = query.exec();
    if(!ret) {
        LogError(QString("insert record: %1 failed! %2 ,error: %3!")
                 .arg("t_user_authority")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    this->load(pDB);
}

bool UserAuthority::del(ProjectDataSQLiteDatabase *pDB,
                        UserAuthorityPrivate *pObj) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("delete from t_user_authority where id = :id");
    query.bindValue(":id", pObj->iID_);

    ret = query.exec();
    if(!ret) {
        LogError(QString("delete record: %1 failed! %2 ,error: %3!")
                 .arg("t_user_authority")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    this->load(pDB);

    return ret;
}

bool UserAuthority::delAll(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("delete from t_user_authority");
    ret = query.exec();
    if(!ret) {
        LogError(QString("delete record: %1 failed! %2 ,error: %3!")
                 .arg("t_user_authority")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    this->load(pDB);

    return ret;
}


bool UserAuthority::update(ProjectDataSQLiteDatabase *pDB,
                           UserAuthorityPrivate *pObj) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("update t_user_authority set number = :num, "
                  "name = :name, password = :pass, authority = :auth, comments = :com, "
                  "name2 = :name2, name3 = :name3, name4 = :name4, name5 = :name5, "
                  "login = :login, logout = :logout where id = :id");

    query.bindValue(":id", pObj->iID_);
    query.bindValue(":num", pObj->szIndex_);
    query.bindValue(":name", pObj->szName_);
    query.bindValue(":pass", pObj->szPassword_);
    query.bindValue(":auth", pObj->szAuthority_);
    query.bindValue(":com", pObj->szComments_);
    query.bindValue(":name2", pObj->szName2_);
    query.bindValue(":name3", pObj->szName3_);
    query.bindValue(":name4", pObj->szName4_);
    query.bindValue(":name5", pObj->szName5_);
    query.bindValue(":login", pObj->bLogin_ ? 1 : 0);
    query.bindValue(":logout", pObj->bLogout_ ? 1 : 0);

    ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_user_authority")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    return ret;
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

