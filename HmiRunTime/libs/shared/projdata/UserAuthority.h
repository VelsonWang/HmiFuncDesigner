#ifndef USERAUTHORITY_H
#define USERAUTHORITY_H

#include <QString>
#include <QObject>
#include "../xmlobject.h"
#include "../sharedlibglobal.h"

class SHAREDLIB_EXPORT UserAuthorityPrivate
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
        m_id = obj.m_id;
        m_index = obj.m_index;
        m_name = obj.m_name;
        m_password = obj.m_password;
        m_authority = obj.m_authority;
        m_comments = obj.m_comments;
        m_name2 = obj.m_name2;
        m_name3 = obj.m_name3;
        m_name4 = obj.m_name4;
        m_name5 = obj.m_name5;
        m_login = obj.m_login;
        m_logout = obj.m_logout;
    }

    int m_id;
    QString m_index;
    QString m_name;
    QString m_password;
    QString m_authority;
    QString m_comments;
    QString m_name2;
    QString m_name3;
    QString m_name4;
    QString m_name5;
    bool m_login;
    bool m_logout;
};



class SHAREDLIB_EXPORT UserAuthority
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
    QList<UserAuthorityPrivate *> m_userAuthority;

    Q_DISABLE_COPY(UserAuthority)
};




#endif // USERAUTHORITY_H
