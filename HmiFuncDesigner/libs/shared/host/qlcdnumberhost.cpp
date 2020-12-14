#include "qlcdnumberhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QLCDNumber>

QLCDNumberHost::QLCDNumberHost(QAbstractHost *parent):
    QFrameHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QLCDNumberHost::get_show_name()
{
    return tr("LCD Number");
}

QString QLCDNumberHost::get_show_group()
{
    return tr("Display Widgets");
}

QString QLCDNumberHost::get_show_icon()
{
    return ":/images/lcdnumber.png";
}

void QLCDNumberHost::create_object()
{
    m_object=new QLCDNumber();
    m_object->setObjectName("lcdnumber");
}

void QLCDNumberHost::init_property()
{
    QFrameHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","digitCount");
        pro->set_attribute("show_name",tr("DigitCount"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","numDigits");
        pro->set_attribute("show_name",tr("NumDigits"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","intValue");
        pro->set_attribute("show_name",tr("IntValue"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Float");
    if(pro!=NULL)
    {
        pro->set_property("name","value");
        pro->set_attribute("show_name",tr("Value"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,2);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","mode");
        pro->set_attribute("show_name",tr("Mode"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("Hex");
        item.m_value=QLCDNumber::Hex;
        items.append(item);

        item.m_text=tr("Dec");
        item.m_value=QLCDNumber::Dec;
        items.append(item);

        item.m_text=tr("Oct");
        item.m_value=QLCDNumber::Oct;
        items.append(item);

        item.m_text=tr("Bin");
        item.m_value=QLCDNumber::Bin;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);

        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","segmentStyle");
        pro->set_attribute("show_name",tr("SegmentStyle"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("Outline");
        item.m_value=QLCDNumber::Outline;
        items.append(item);

        item.m_text=tr("Filled");
        item.m_value=QLCDNumber::Filled;
        items.append(item);

        item.m_text=tr("Flat");
        item.m_value=QLCDNumber::Flat;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);

        insert_property(pro);
    }

    set_property_value("geometry",QRect(0,0,100,30));
}

void QLCDNumberHost::setIntValue(int intValue)
{
    set_property_value("intValue",intValue);
}

int QLCDNumberHost::intValue()
{
    return get_property_value("intValue").toInt();
}

void QLCDNumberHost::setValue(float value)
{
    set_property_value("value",value);
}

float QLCDNumberHost::value()
{
    return get_property_value("value").toFloat();
}

void QLCDNumberHost::setMode(int mode)
{
    set_property_value("mode",mode);
}

int QLCDNumberHost::mode()
{
    return get_property_value("mode").toInt();
}

void QLCDNumberHost::setSegmentStyle(int segmentStyle)
{
    set_property_value("segmentStyle",segmentStyle);
}

int QLCDNumberHost::segmentStyle()
{
    return get_property_value("segmentStyle").toInt();
}

void QLCDNumberHost::setDigitCount(int digitCount)
{
    set_property_value("digitCount",digitCount);
}

int QLCDNumberHost::digitCount()
{
    return get_property_value("digitCount").toInt();
}

void QLCDNumberHost::setNumDigits(int numDigits)
{
    set_property_value("numDigits",numDigits);
}

int QLCDNumberHost::numDigits()
{
    return get_property_value("numDigits").toInt();
}
