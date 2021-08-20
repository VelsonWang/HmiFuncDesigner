#include "qhostfactory.h"
#include "host/qabstracthost.h"
#include "host/qformhost.h"
#include "xmlobject.h"

QMap<QString, tagHostInfo*> QHostFactory::m_meta_map;

QHostFactory::QHostFactory()
{
}

void QHostFactory::register_host(const QString name, GET_SHOW_ICON icon, GET_SHOW_NAME get_name, GET_SHOW_GROUP get_group, const QMetaObject *host)
{
    tagHostInfo *info = m_meta_map.value(name);
    if(info == Q_NULLPTR) {
        info = new tagHostInfo;
    }
    info->m_host_object = host;
    info->m_name = name;
    info->getShowIcon = icon;
    info->getShowName = get_name;
    info->getShowGroup = get_group;
    m_meta_map.insert(name, info);
}

QAbstractHost* QHostFactory::create_host(const QString &name)
{
    QAbstractHost* host = Q_NULLPTR;
    if(name == FORM_TITLE) {
        host = new QFormHost;
        host->init();
        host->setAttribute(HOST_TYPE, FORM_TITLE);
    } else {
        tagHostInfo *info = m_meta_map.value(name);
        if(info == Q_NULLPTR) {
            return Q_NULLPTR;
        }
        const QMetaObject* obj = info->m_host_object;

        if(obj != Q_NULLPTR) {
            host = (QAbstractHost*)obj->newInstance();
            host->init();
            host->setAttribute(HOST_TYPE, name);
        }
    }

    return host;
}

QMap<QString, tagHostInfo*> QHostFactory::get_host_info()
{
    return m_meta_map;
}

QAbstractHost* QHostFactory::create_host(XMLObject *xml)
{
    if(xml == Q_NULLPTR) {
        return Q_NULLPTR;
    }

    QAbstractHost* ret = Q_NULLPTR;

    if(xml->getTagName() == FORM_TITLE) {
        ret = new QFormHost;
        ret->init();
        ret->fromObject(xml);
        ret->setAttribute(HOST_TYPE, FORM_TITLE);
    } else {
        ret = create_host(xml->getProperty(HOST_TYPE));
        if(ret != Q_NULLPTR) {
            ret->fromObject(xml);
        }
    }
    return ret;
}

tagHostInfo* QHostFactory::get_host_info(const QString &name)
{
    return m_meta_map.value(name);
}
