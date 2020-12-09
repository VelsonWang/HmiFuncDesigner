#include "qhostfactory.h"

#include "QAbstractHost.h"
#include "GraphPage.h"


QMap<QString,tagHostInfo*> QHostFactory::m_meta_map;

QHostFactory::QHostFactory()
{
}

void QHostFactory::registerHost(const QString name, GET_SHOW_ICON icon, GET_SHOW_NAME get_name, GET_SHOW_GROUP get_group, const QMetaObject *host)
{
    tagHostInfo *info=m_meta_map.value(name);
    if(info==NULL)
    {
        info=new tagHostInfo;
    }
    info->m_host_object=host;
    info->m_name=name;
    info->get_show_icon=icon;
    info->get_show_name=get_name;
    info->get_show_group=get_group;
    m_meta_map.insert(name,info);
}

QAbstractHost* QHostFactory::createHost(const QString &name)
{
    QAbstractHost* host=NULL;
//    if(name==FORM_TITLE)
//    {
//        host=new GraphPage;
//        host->init();
//        host->setAttribute(HOST_TYPE,FORM_TITLE);
//    }
//    else
//    {
//        tagHostInfo *info=m_meta_map.value(name);
//        if(info==NULL)
//        {
//            return NULL;
//        }
//        const QMetaObject* obj=info->m_host_object;

//        if(obj!=NULL)
//        {
//            host=(QAbstractHost*)obj->newInstance();
//            host->init();
//            host->setAttribute(HOST_TYPE,name);
//        }
//    }

    return host;
}

QMap<QString,tagHostInfo*> QHostFactory::getHostInfo()
{
    return m_meta_map;
}

//QAbstractHost* QHostFactory::createHost(XMLObject *xml)
//{
//    if(xml==NULL)
//    {
//        return NULL;
//    }

//    QAbstractHost* ret=NULL;

//    if(xml->get_title()==FORM_TITLE)
//    {
//        ret=new QFormHost;
//        ret->init();
//        ret->from_object(xml);
//        ret->set_attribute(HOST_TYPE,FORM_TITLE);
//    }
//    else
//    {
//        ret=createHost(xml->get_property(HOST_TYPE));
//        if(ret!=NULL)
//        {
//            ret->from_object(xml);
//        }
//    }
//    return ret;
//}

tagHostInfo* QHostFactory::getHostInfo(const QString &name)
{
    return m_meta_map.value(name);
}
