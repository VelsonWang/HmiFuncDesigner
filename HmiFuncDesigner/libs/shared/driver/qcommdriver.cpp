#include "qcommdriver.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

QCommDriverHost::QCommDriverHost(QAbstractHost *parent):
    QAbstractDriver(parent)
{
}

void QCommDriverHost::create_object()
{
    m_object=new QCommDriverObject();
    m_object->setObjectName("comm");
}

void QCommDriverHost::init_property()
{
    QAbstractDriver::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","serial_number");
        pro->set_attribute("show_name",tr("Serial Number"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        m_object->setProperty("serial_number",1);
        insert_property(pro);
    }
}

QString QCommDriverHost::get_show_name()
{
    return tr("Serial");
}

void QCommDriverObject::start()
{

}

void QCommDriverObject::close()
{

}
