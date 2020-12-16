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

QString QSliderHost::getShowName()
{
    return tr("Slider");
}

QString QSliderHost::getShowGroup()
{
    return tr("Input Widgets");
}

QString QSliderHost::getShowIcon()
{
    return ":/images/hslider.png";
}

void QSliderHost::createObject()
{
    m_object=new QSlider();
    m_object->setObjectName("slider");
}

void QSliderHost::initProperty()
{
    QAbstractSliderHost::initProperty();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","tickPosition");
        pro->setAttribute("show_name",tr("TickPosition"));
        pro->setAttribute("group","Attributes");
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
        pro->setAttribute("items",v);
        insertProperty(pro);
    }

    setPropertyValue("geometry",QRect(0,0,100,20));
}
