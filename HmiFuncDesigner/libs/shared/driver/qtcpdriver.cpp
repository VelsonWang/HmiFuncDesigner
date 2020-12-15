#include "qtcpdriver.h"


#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

QTCPDriverHost::QTCPDriverHost(QAbstractHost *parent):
    QAbstractDriver(parent)
{
}

void QTCPDriverHost::createObject()
{
    m_object=new QTCPDriverObject();
    m_object->setObjectName("tcp");
}

void QTCPDriverHost::initProperty()
{
    QAbstractDriver::initProperty();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->setProperty("name","ip_address");
        pro->setAttribute("show_name",tr("IP Address"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_NEEDSAVE,true);
        m_object->setProperty("ip_address","192.168.1.1");
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->setProperty("name","ip_port");
        pro->setAttribute("show_name",tr("IP Port"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_NEEDSAVE,true);
        m_object->setProperty("ip_port",100);
        insertProperty(pro);
    }
}

QString QTCPDriverHost::getShowName()
{
    return tr("TCP");
}

void QTCPDriverObject::start()
{

}

void QTCPDriverObject::close()
{

}
