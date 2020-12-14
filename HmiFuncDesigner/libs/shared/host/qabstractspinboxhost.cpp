#include "qabstractspinboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QAbstractSpinBox>

QAbstractSpinBoxHost::QAbstractSpinBoxHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
}

void QAbstractSpinBoxHost::init_property()
{
    QWidgetHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","frame");
        pro->set_attribute("show_name",tr("Frame"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","wrapping");
        pro->set_attribute("show_name",tr("Wrapping"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","readOnly");
        pro->set_attribute("show_name",tr("ReadOnly"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","buttonSymbols");
        pro->set_attribute("show_name",tr("ButtonSymbols"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("UpDownArrows");
        item.m_value=QAbstractSpinBox::UpDownArrows;
        items.append(item);

        item.m_text=tr("NoButtons");
        item.m_value=QAbstractSpinBox::NoButtons;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","alignment");
        pro->set_attribute("show_name",tr("Alignment"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("AlignLeft");
        item.m_value=Qt::AlignLeft;
        items.append(item);

        item.m_text=tr("AlignRight");
        item.m_value=Qt::AlignRight;
        items.append(item);

        item.m_text=tr("AlignHCenter");
        item.m_value=Qt::AlignHCenter;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        insert_property(pro);
    }

    set_property_value("alignment",Qt::AlignLeft);
}

void QAbstractSpinBoxHost::setFrame(bool frame)
{
    set_property_value("frame",frame);
}

bool QAbstractSpinBoxHost::frame()
{
    return get_property_value("frame").toBool();
}

void QAbstractSpinBoxHost::setWraping(bool wraping)
{
    set_property_value("wraping",wraping);
}

bool QAbstractSpinBoxHost::wraping()
{
    return get_property_value("wraping").toBool();
}

void QAbstractSpinBoxHost::setReadOnly(bool readonly)
{
    set_property_value("readOnly",readonly);
}

bool QAbstractSpinBoxHost::readonly()
{
    return get_property_value("readOnly").toBool();
}

void QAbstractSpinBoxHost::setButtonSymbols(int buttonSymbols)
{
    set_property_value("buttonSymbols",buttonSymbols);
}

int QAbstractSpinBoxHost::buttonSymbols()
{
    return get_property_value("buttonSymbols").toInt();
}

void QAbstractSpinBoxHost::setAlignment(int alignment)
{
    set_property_value("alignment",alignment);
}

int QAbstractSpinBoxHost::alignment()
{
    return get_property_value("alignment").toInt();
}
