#include "qlcdnumberhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QLCDNumber>

QLCDNumberHost::QLCDNumberHost(QAbstractHost *parent):
    QFrameHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QLCDNumberHost::getShowName()
{
    return tr("LCD Number");
}

QString QLCDNumberHost::getShowGroup()
{
    return tr("Display Widgets");
}

QString QLCDNumberHost::getShowIcon()
{
    return ":/images/lcdnumber.png";
}

void QLCDNumberHost::createObject()
{
    m_object = new QLCDNumber();
    m_object->setObjectName("lcdnumber");
}

void QLCDNumberHost::initProperty()
{
    QFrameHost::initProperty();

    QAbstractProperty *pro;

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "digitCount");
        pro->setAttribute("show_name", tr("DigitCount"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "numDigits");
        pro->setAttribute("show_name", tr("NumDigits"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "intValue");
        pro->setAttribute("show_name", tr("IntValue"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro, 1);
    }

    pro = QPropertyFactory::create_property("Float");
    if(pro != NULL) {
        pro->setObjectProperty("name", "value");
        pro->setAttribute("show_name", tr("Value"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro, 2);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "mode");
        pro->setAttribute("show_name", tr("Mode"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("Hex");
        item.m_value = QLCDNumber::Hex;
        items.append(item);

        item.m_text = tr("Dec");
        item.m_value = QLCDNumber::Dec;
        items.append(item);

        item.m_text = tr("Oct");
        item.m_value = QLCDNumber::Oct;
        items.append(item);

        item.m_text = tr("Bin");
        item.m_value = QLCDNumber::Bin;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items", v);

        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "segmentStyle");
        pro->setAttribute("show_name", tr("SegmentStyle"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("Outline");
        item.m_value = QLCDNumber::Outline;
        items.append(item);

        item.m_text = tr("Filled");
        item.m_value = QLCDNumber::Filled;
        items.append(item);

        item.m_text = tr("Flat");
        item.m_value = QLCDNumber::Flat;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items", v);

        insertProperty(pro);
    }

    setPropertyValue("geometry", QRect(0, 0, 100, 30));
}

void QLCDNumberHost::setIntValue(int intValue)
{
    setPropertyValue("intValue", intValue);
}

int QLCDNumberHost::intValue()
{
    return getPropertyValue("intValue").toInt();
}

void QLCDNumberHost::setValue(float value)
{
    setPropertyValue("value", value);
}

float QLCDNumberHost::value()
{
    return getPropertyValue("value").toFloat();
}

void QLCDNumberHost::setMode(int mode)
{
    setPropertyValue("mode", mode);
}

int QLCDNumberHost::mode()
{
    return getPropertyValue("mode").toInt();
}

void QLCDNumberHost::setSegmentStyle(int segmentStyle)
{
    setPropertyValue("segmentStyle", segmentStyle);
}

int QLCDNumberHost::segmentStyle()
{
    return getPropertyValue("segmentStyle").toInt();
}

void QLCDNumberHost::setDigitCount(int digitCount)
{
    setPropertyValue("digitCount", digitCount);
}

int QLCDNumberHost::digitCount()
{
    return getPropertyValue("digitCount").toInt();
}

void QLCDNumberHost::setNumDigits(int numDigits)
{
    setPropertyValue("numDigits", numDigits);
}

int QLCDNumberHost::numDigits()
{
    return getPropertyValue("numDigits").toInt();
}

