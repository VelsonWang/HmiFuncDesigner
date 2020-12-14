#ifndef QUSERMANAGER_H
#define QUSERMANAGER_H

#include "sharedlibglobal.h"

#include <QObject>
#include <QMap>

struct tagUserInfo
{
    QString m_name;
    QString m_information;
    int     m_level;
    QString m_password;
    QString m_uuid;
};

class XMLObject;

class SHAREDLIB_EXPORT QUserManager : public QObject
{
    Q_OBJECT
public:
    explicit QUserManager(QObject *parent = 0);
    ~QUserManager();

    void    load(const QString &path);
    void    save(const QString &path);
    void    clear();

    QList<tagUserInfo*> get_users();
    tagUserInfo*        get_user(int index);
    tagUserInfo*        get_user(const QString &uiid);

    void                insert_user(const tagUserInfo &info,int index=-1);
    void                remove_user(const QString &userName);

    void                emit_refresh(tagUserInfo *info);
public slots:
signals:
    void                refresh(tagUserInfo *info);
    void                inser_user_signal(tagUserInfo* info,int index);
    void                remove_user_signal(tagUserInfo* info);
protected:
    QList<tagUserInfo*>         m_users;
    QMap<QString,tagUserInfo*>  m_id_to_user;
};

#endif // QUSERMANAGER_H
