#ifndef QHOSTFACTORY_H
#define QHOSTFACTORY_H

#include "sharedlibglobal.h"

#include <QMetaObject>
#include <QMap>
#include <QString>

class QAbstractHost;
class XMLObject;

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

class SHAREDLIB_EXPORT QHostFactory
{
protected:
    QHostFactory();

public:
    static void register_host(const QString name,GET_SHOW_ICON icon,GET_SHOW_NAME get_name,GET_SHOW_GROUP get_group,const QMetaObject* host);
    static QAbstractHost* create_host(const QString& name);
    static QMap<QString,tagHostInfo*> get_host_info();
    static QAbstractHost* create_host(XMLObject* xml);
    static tagHostInfo *  get_host_info(const QString &name);
protected:
    static QMap<QString,tagHostInfo*>        m_meta_map;
};


#ifndef qRegisterHost
    #define qRegisterHost(name,class_name) QHostFactory::register_host(name,class_name::get_show_icon,class_name::get_show_name,class_name::get_show_group,&class_name::staticMetaObject)
#endif

#endif // QHOSTFACTORY_H
