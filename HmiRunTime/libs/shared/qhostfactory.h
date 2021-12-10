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

struct tagHostInfo {
    const QMetaObject *m_hostObject;
    QString m_name;
    GET_SHOW_ICON getShowIcon;
    GET_SHOW_NAME getShowName;
    GET_SHOW_GROUP getShowGroup;
};

class SHAREDLIB_EXPORT QHostFactory
{
protected:
    QHostFactory();

public:
    static void registerHost(const QString name, GET_SHOW_ICON icon, GET_SHOW_NAME get_name, GET_SHOW_GROUP get_group, const QMetaObject* host);
    static QAbstractHost* createHost(const QString& name);
    static QMap<QString, tagHostInfo*> getHostInfo();
    static QAbstractHost* createHost(XMLObject* xml);
    static tagHostInfo* getHostInfo(const QString &name);
protected:
    static QMap<QString, tagHostInfo*> m_metaMap;
};


#ifndef qRegisterHost
    #define qRegisterHost(name,class_name) QHostFactory::registerHost(name, class_name::getShowIcon, class_name::getShowName, class_name::getShowGroup, &class_name::staticMetaObject)
#endif

#endif // QHOSTFACTORY_H
