#include "qcommdriver.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

QCommDriverHost::QCommDriverHost(QAbstractHost *parent):
    QAbstractDriver(parent)
{
}

void QCommDriverHost::createObject()
{
    m_object=new QCommDriverObject();
    m_object->setObjectName("comm");
}

void QCommDriverHost::initProperty()
{
    QAbstractDriver::initProperty();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->setProperty("name","serial_number");
        pro->setAttribute("show_name",tr("Serial Number"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_NEEDSAVE,true);
        m_object->setProperty("serial_number",1);
        insertProperty(pro);
    }
}

QString QCommDriverHost::getShowName()
{
    return tr("Serial");
}

void QCommDriverObject::start()
{

}

void QCommDriverObject::close()
{

}
