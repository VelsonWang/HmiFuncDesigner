#include "qdriverfactory.h"

#include "driver/qabstractdriver.h"

QMap<QString,tagDriverInfo*> QDriverFactory::m_meta_map;

QDriverFactory::QDriverFactory()
{
}

void QDriverFactory::register_driver(const QString name,GET_SHOW_NAME get_name, GET_SHOW_GROUP get_group, const QMetaObject *driver)
{
    tagDriverInfo *info=m_meta_map.value(name);
    if(info==NULL)
    {
        info=new tagDriverInfo;
    }
    info->m_driver_object=driver;
    info->m_name=name;
    info->get_show_name=get_name;
    info->get_show_group=get_group;
    m_meta_map.insert(name,info);
}

QAbstractDriver* QDriverFactory::create_driver(const QString &name)
{
    QAbstractDriver* driver=NULL;
    tagDriverInfo *info=m_meta_map.value(name);
    if(info==NULL)
    {
        return NULL;
    }
    const QMetaObject* obj=info->m_driver_object;

    if(obj!=NULL)
    {
        driver=(QAbstractDriver*)obj->newInstance();
        driver->init();
        driver->set_attribute(HOST_TYPE,name);
        driver->set_property_value("driver_type",info->get_show_group()+"."+info->get_show_name());
    }

    return driver;
}

QMap<QString,tagDriverInfo*> QDriverFactory::get_driver_info()
{
    return m_meta_map;
}

tagDriverInfo* QDriverFactory::get_driver_info(const QString &name)
{
    return m_meta_map.value(name);
}
