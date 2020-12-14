#include "qabstractsliderhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QAbstractSlider>

QAbstractSliderHost::QAbstractSliderHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
}

void QAbstractSliderHost::init_property()
{
    QWidgetHost::init_property();

    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","value");
        pro->set_attribute("show_name",tr("Value"));
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
        pro->set_property("name","maximum");
        pro->set_attribute("show_name",tr("Maximum"));
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

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","orientation");
        pro->set_attribute("show_name",tr("Orientation"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("Horizontal");
        item.m_value=Qt::Horizontal;
        items.append(item);

        item.m_text=tr("Vertical");
        item.m_value=Qt::Vertical;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        m_object->setProperty("orientation",Qt::Horizontal);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","invertedAppearance");
        pro->set_attribute("show_name",tr("InvertedAppearance"));
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
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    QAbstractSlider *slider=(QAbstractSlider*)m_object;
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void QAbstractSliderHost::setValue(int value)
{
    set_property_value("value",value);
}

int QAbstractSliderHost::value()
{
    return get_property_value("value").toInt();
}

void QAbstractSliderHost::setSingleStep(int singleStep)
{
    set_property_value("singleStep",singleStep);
}

int QAbstractSliderHost::singleStep()
{
    return get_property_value("singleStep").toInt();
}

void QAbstractSliderHost::setOrientation(int orientation)
{
    set_property_value("orientation",orientation);
}

int QAbstractSliderHost::orientation()
{
    return get_property_value("orientation").toInt();
}

void QAbstractSliderHost::setInvertedAppearance(bool invertedAppearance)
{
    set_property_value("invertedAppearance",invertedAppearance);
}

bool QAbstractSliderHost::invertedAppearance()
{
    return get_property_value("invertedAppearance").toBool();
}

void QAbstractSliderHost::valueChanged(int value)
{
    QString code=get_property_value("valueChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_value",QString::number(value));
        exec(code,param);
    }
}
