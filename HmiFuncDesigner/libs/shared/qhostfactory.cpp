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
    if(info == NULL) {
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
    QAbstractHost* pHostObj = NULL;
    if(name == FORM_TITLE) {
        pHostObj = new QFormHost;
        pHostObj->init();
        pHostObj->setAttribute(HOST_TYPE, FORM_TITLE);
    } else {
        tagHostInfo *info = m_meta_map.value(name);
        if(info == NULL) {
            return NULL;
        }
        const QMetaObject* pObj = info->m_host_object;

        if(pObj != NULL) {
            pHostObj = (QAbstractHost*)pObj->newInstance();
            pHostObj->init();
            pHostObj->setAttribute(HOST_TYPE, name);
        }
    }

    return pHostObj;
}

QMap<QString, tagHostInfo*> QHostFactory::get_host_info()
{
    return m_meta_map;
}

QAbstractHost* QHostFactory::create_host(XMLObject *xml)
{
    if(xml == NULL) {
        return NULL;
    }

    QAbstractHost* pHostObj = NULL;

    if(xml->getTagName() == FORM_TITLE) {
        pHostObj = new QFormHost;
        pHostObj->init();
        pHostObj->fromObject(xml);
        pHostObj->setAttribute(HOST_TYPE, FORM_TITLE);
    } else {
        pHostObj = create_host(xml->getProperty(HOST_TYPE));
        if(pHostObj != NULL) {
            pHostObj->fromObject(xml);
        }
    }
    return pHostObj;
}

tagHostInfo* QHostFactory::get_host_info(const QString &name)
{
    return m_meta_map.value(name);
}
