#include "qsliderhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QSlider>

QSliderHost::QSliderHost(QAbstractHost *parent):
    QAbstractSliderHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QSliderHost::get_show_name()
{
    return tr("Slider");
}

QString QSliderHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QSliderHost::get_show_icon()
{
    return ":/images/hslider.png";
}

void QSliderHost::create_object()
{
    m_object=new QSlider();
    m_object->setObjectName("slider");
}

void QSliderHost::init_property()
{
    QAbstractSliderHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","tickPosition");
        pro->set_attribute("show_name",tr("TickPosition"));
        pro->set_attribute("group","Attributes");
        ComboItems items;
        tagComboItem item;
        item.m_text="NoTicks";
        item.m_value=QSlider::NoTicks;
        items.append(item);

        item.m_text="TicksAbove";
        item.m_value=QSlider::TicksAbove;
        items.append(item);

        item.m_text="TicksBelow";
        item.m_value=QSlider::TicksBelow;
        items.append(item);

        item.m_text="TicksLeft";
        item.m_value=QSlider::TicksLeft;
        items.append(item);

        item.m_text="TicksRight";
        item.m_value=QSlider::TicksRight;
        items.append(item);

        item.m_text="TicksBothSides";
        item.m_value=QSlider::TicksBothSides;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        insert_property(pro);
    }

    set_property_value("geometry",QRect(0,0,100,20));
}
