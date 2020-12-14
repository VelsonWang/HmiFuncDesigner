#include "qspinboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QSpinBox>

QSpinBoxHost::QSpinBoxHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QSpinBoxHost::get_show_name()
{
    return tr("Spin Box");
}

QString QSpinBoxHost::get_show_group()
{
    return tr("Input Widgets");
}


QString QSpinBoxHost::get_show_icon()
{
    return ":/images/spinbox.png";
}

void QSpinBoxHost::create_object()
{
    m_object=new QSpinBox();
    m_object->setObjectName("spinbox");
}

void QSpinBoxHost::init_property()
{
    QAbstractSpinBoxHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","maximum");
        pro->set_attribute("show_name",tr("Maximum"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","minimum");
        pro->set_attribute("show_name",tr("Minimum"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","value");
        pro->set_attribute("show_name",tr("Value"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","singleStep");
        pro->set_attribute("show_name",tr("SingleStep"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }


    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","valueChanged");
        pro->set_attribute("show_name",tr("ValueChanged"));
        pro->set_attribute("group","Events");
        m_object->setProperty("valueChanged","");
        insert_property(pro);
    }

    set_property_value("geometry",QRect(0,0,100,20));

    QSpinBox *e=(QSpinBox*)m_object;
    connect(e,SIGNAL(valueChanged(QString)),this,SLOT(valueChanged(QString)));
}

void QSpinBoxHost::setValue(int value)
{
    set_property_value("value",value);
}

int QSpinBoxHost::value()
{
    return get_property_value("value").toInt();
}

void QSpinBoxHost::setSingleStep(int singleStep)
{
    set_property_value("singleStep",singleStep);
}

int QSpinBoxHost::singleStep()
{
    return get_property_value("singleStep").toInt();
}

void QSpinBoxHost::valueChanged(const QString &value)
{
    QString code=get_property_value("valueChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_value",value);
        exec(code,param);
    }
}
