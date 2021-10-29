#include "qhostfactory.h"
#include "host/qabstracthost.h"
#include "host/qformhost.h"
#include "xmlobject.h"

QMap<QString, tagHostInfo*> QHostFactory::m_metaMap;

QHostFactory::QHostFactory()
{
}

void QHostFactory::registerHost(const QString name, GET_SHOW_ICON icon, GET_SHOW_NAME get_name, GET_SHOW_GROUP get_group, const QMetaObject *host)
{
    tagHostInfo *info = m_metaMap.value(name);
    if(info == NULL) {
        info = new tagHostInfo;
    }
    info->m_hostObject = host;
    info->m_name = name;
    info->getShowIcon = icon;
    info->getShowName = get_name;
    info->getShowGroup = get_group;
    m_metaMap.insert(name, info);
}

QAbstractHost* QHostFactory::createHost(const QString &name)
{
    QAbstractHost* host = NULL;
    if(name == FORM_TITLE) {
        host = new QFormHost;
        host->init();
        host->setAttribute(HOST_TYPE, FORM_TITLE);
    } else {
        tagHostInfo *info = m_metaMap.value(name);
        if(info == NULL) {
            return NULL;
        }
        const QMetaObject* obj = info->m_hostObject;

        if(obj != NULL) {
            host = (QAbstractHost*)obj->newInstance();
            host->init();
            host->setAttribute(HOST_TYPE, name);
        }
    }

    return host;
}

QMap<QString, tagHostInfo*> QHostFactory::getHostInfo()
{
    return m_metaMap;
}

QAbstractHost* QHostFactory::createHost(XMLObject *xml)
{
    if(xml == NULL) {
        return NULL;
    }

    QAbstractHost* ret = NULL;

    if(xml->getTagName() == FORM_TITLE) {
        ret = new QFormHost;
        ret->init();
        ret->fromObject(xml);
        ret->setAttribute(HOST_TYPE, FORM_TITLE);
    } else {
        ret = createHost(xml->getProperty(HOST_TYPE));
        if(ret != NULL) {
            ret->fromObject(xml);
        }
    }
    return ret;
}

tagHostInfo* QHostFactory::getHostInfo(const QString &name)
{
    return m_metaMap.value(name);
}
