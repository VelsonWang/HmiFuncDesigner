#include "qtcpdriver.h"


#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

QTCPDriverHost::QTCPDriverHost(QAbstractHost *parent):
    QAbstractDriver(parent)
{
}

void QTCPDriverHost::create_object()
{
    m_object=new QTCPDriverObject();
    m_object->setObjectName("tcp");
}

void QTCPDriverHost::init_property()
{
    QAbstractDriver::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->set_property("name","ip_address");
        pro->set_attribute("show_name",tr("IP Address"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        m_object->setProperty("ip_address","192.168.1.1");
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","ip_port");
        pro->set_attribute("show_name",tr("IP Port"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        m_object->setProperty("ip_port",100);
        insert_property(pro);
    }
}

QString QTCPDriverHost::get_show_name()
{
    return tr("TCP");
}

void QTCPDriverObject::start()
{

}

void QTCPDriverObject::close()
{

}
