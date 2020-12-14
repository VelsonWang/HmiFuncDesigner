#include "qdoublespinboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QDoubleSpinBox>

QDoubleSpinBoxHost::QDoubleSpinBoxHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QDoubleSpinBoxHost::get_show_name()
{
    return tr("Double Spin Box");
}

QString QDoubleSpinBoxHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QDoubleSpinBoxHost::get_show_icon()
{
    return ":/images/doublespinbox.png";
}

void QDoubleSpinBoxHost::create_object()
{
    m_object=new QDoubleSpinBox();
    m_object->setObjectName("doublespinbox");
}

void QDoubleSpinBoxHost::init_property()
{
    QAbstractSpinBoxHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Float");
    if(pro!=NULL)
    {
        pro->set_property("name","maximum");
        pro->set_attribute("show_name",tr("Maximum"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Float");
    if(pro!=NULL)
    {
        pro->set_property("name","minimum");
        pro->set_attribute("show_name",tr("Minimum"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Float");
    if(pro!=NULL)
    {
        pro->set_property("name","value");
        pro->set_attribute("show_name",tr("Value"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Float");
    if(pro!=NULL)
    {
        pro->set_property("name","singleStep");
        pro->set_attribute("show_name",tr("SingleStep"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","decimals");
        pro->set_attribute("show_name",tr("Decimals"));
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

    QDoubleSpinBox *s=(QDoubleSpinBox*)m_object;
    connect(s,SIGNAL(valueChanged(QString)),this,SLOT(valueChanged(QString)));
}

void QDoubleSpinBoxHost::setValue(float value)
{
    set_property_value("value",value);
}

float QDoubleSpinBoxHost::value()
{
    return get_property_value("value").toFloat();
}

void QDoubleSpinBoxHost::setSingleStep(float singleStep)
{
    set_property_value("singleStep",singleStep);
}

float QDoubleSpinBoxHost::singleStep()
{
    return get_property_value("singleStep").toFloat();
}

void QDoubleSpinBoxHost::setDecimals(int decimals)
{
    set_property_value("decimals",decimals);
}

int QDoubleSpinBoxHost::decimals()
{
    return get_property_value("decimals").toInt();
}

void QDoubleSpinBoxHost::valueChanged(const QString &value)
{
    QString code=get_property_value("valueChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_value",value);
        exec(code,param);
    }
}
