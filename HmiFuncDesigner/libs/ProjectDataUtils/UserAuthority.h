#ifndef USERAUTHORITY_H
#define USERAUTHORITY_H

#include <QString>
#include <QObject>
#include "XMLObject.h"

class UserAuthorityPrivate
{
public:
    explicit UserAuthorityPrivate()
    {

    }

    UserAuthorityPrivate(const UserAuthorityPrivate &obj)
    {
        copyObject(obj);
    }

    ~UserAuthorityPrivate()
    {

    }

    UserAuthorityPrivate &operator=(const UserAuthorityPrivate &obj)
    {
        copyObject(obj);
        return *this;
    }

    void copyObject(const UserAuthorityPrivate &obj)
    {
        iID_ = obj.iID_;
        szIndex_ = obj.szIndex_;
        szName_ = obj.szName_;
        szPassword_ = obj.szPassword_;
        szAuthority_ = obj.szAuthority_;
        szComments_ = obj.szComments_;
        szName2_ = obj.szName2_;
        szName3_ = obj.szName3_;
        szName4_ = obj.szName4_;
        szName5_ = obj.szName5_;
        bLogin_ = obj.bLogin_;
        bLogout_ = obj.bLogout_;
    }

    int iID_;
    QString szIndex_;
    QString szName_;
    QString szPassword_;
    QString szAuthority_;
    QString szComments_;
    QString szName2_;
    QString szName3_;
    QString szName4_;
    QString szName5_;
    bool bLogin_;
    bool bLogout_;
};



class UserAuthority
{
public:
    explicit UserAuthority();
    ~UserAuthority();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);
    int getUserAuthorityID(int index);
    int getCount();

public:
    QString getIndex(int index) const;
    void setIndex(int index, const QString &szIndex);

    QString getName(int index) const;
    void setName(int index, const QString &name);

    QString getPassword(int index) const;
    void setPassword(int index, const QString &password);

    QString getAuthority(int index) const;
    void setAuthority(int index, const QString &authority);

    QString getComments(int index) const;
    void setComments(int index, const QString &comments);

    QString getName2(int index) const;
    void setName2(int index, const QString &name);

    QString getName3(int index) const;
    void setName3(int index, const QString &name);

    QString getName4(int index) const;
    void setName4(int index, const QString &name);

    QString getName5(int index) const;
    void setName5(int index, const QString &name);

    bool isLogin(int index);
    void setLogin(int index, bool login);

    bool isLogout(int index);
    void setLogout(int index, bool logout);

public:
    QList<UserAuthorityPrivate *> listUserAuthority_;

    Q_DISABLE_COPY(UserAuthority)
};




#endif // USERAUTHORITY_H
