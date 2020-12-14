#ifndef QDRIVERFACTORY_H
#define QDRIVERFACTORY_H

#include "sharedlibglobal.h"

#include <QMetaObject>
#include <QMap>
#include <QString>

class QAbstractDriver;
class XMLObject;

typedef QString (*GET_SHOW_NAME)();
typedef QString (*GET_SHOW_GROUP)();

struct tagDriverInfo
{
    const QMetaObject *m_driver_object;
    QString         m_name;
    GET_SHOW_NAME   get_show_name;
    GET_SHOW_GROUP  get_show_group;
};

class SHAREDLIB_EXPORT QDriverFactory
{
protected:
    QDriverFactory();

public:
    static void register_driver(const QString name,GET_SHOW_NAME get_name,GET_SHOW_GROUP get_group,const QMetaObject* driver);
    static QAbstractDriver* create_driver(const QString& name);
    static QMap<QString,tagDriverInfo*> get_driver_info();
    static tagDriverInfo *  get_driver_info(const QString &name);
protected:
    static QMap<QString,tagDriverInfo*>        m_meta_map;
};


#ifndef qRegisterDriver
    #define qRegisterDriver(name,class_name) QDriverFactory::register_driver(name,class_name::get_show_name,class_name::get_show_group,&class_name::staticMetaObject)
#endif
#endif // QDRIVERFACTORY_H
