#ifndef QHOSTFACTORY_H
#define QHOSTFACTORY_H

#include <QMetaObject>
#include <QMap>
#include <QString>

class QAbstractHost;

typedef QString (*GET_SHOW_NAME)();
typedef QString (*GET_SHOW_ICON)();
typedef QString (*GET_NAME)();
typedef QString (*GET_SHOW_GROUP)();

struct tagHostInfo
{
    const QMetaObject *m_host_object;
    QString         m_name;
    GET_SHOW_ICON   get_show_icon;
    GET_SHOW_NAME   get_show_name;
    GET_SHOW_GROUP  get_show_group;
};

class QHostFactory
{
protected:
    QHostFactory();

public:
    static void registerHost(const QString name,GET_SHOW_ICON icon,GET_SHOW_NAME get_name,GET_SHOW_GROUP get_group,const QMetaObject* host);
    static QAbstractHost* createHost(const QString& name);
    static QMap<QString,tagHostInfo*> getHostInfo();
    static tagHostInfo *getHostInfo(const QString &name);

protected:
    static QMap<QString,tagHostInfo*> m_meta_map;
};


#ifndef qRegisterHost
    #define qRegisterHost(name,class_name) QHostFactory::registerHost(name,class_name::get_show_icon,class_name::get_show_name,class_name::get_show_group,&class_name::staticMetaObject)
#endif

#endif // QHOSTFACTORY_H
