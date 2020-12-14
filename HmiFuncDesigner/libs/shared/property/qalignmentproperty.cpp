#include "qalignmentproperty.h"

#include "qenumproperty.h"
#include "../qcommonstruct.h"


QAlignmentProperty::QAlignmentProperty(QAbstractProperty *parent):
        QAbstractProperty(parent)
{
    m_propertys.insert("type","Alignment");

    m_horizonta=new QEnumProperty(this);
    m_horizonta->set_attribute("show_name",tr("Horizonta"));
    m_horizonta->set_property("name","Horizonta");
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
    m_horizonta->set_attribute("items",v);

    m_vertical=new QEnumProperty(this);
    m_vertical->set_attribute("show_name",tr("Vertical"));
    m_vertical->set_property("name","Vertical");

    items.clear();
    item.m_text=tr("AlignTop");
    item.m_value=Qt::AlignTop;
    items.append(item);

    item.m_text=tr("AlignBottom");
    item.m_value=Qt::AlignBottom;
    items.append(item);

    item.m_text=tr("AlignVCenter");
    item.m_value=Qt::AlignVCenter;
    items.append(item);

    v.setValue<ComboItems>(items);
    m_vertical->set_attribute("items",v);
}

QString QAlignmentProperty::get_value_text()
{
    return m_horizonta->get_value_text()+" | "+m_vertical->get_value_text();
}

QIcon QAlignmentProperty::get_value_icon()
{
    return QIcon();
}

void QAlignmentProperty::set_value(const QVariant &value)
{
    disconnect_children();

    int align=value.toInt();
    m_horizonta->set_value(align & Qt::AlignHorizontal_Mask);
    m_vertical->set_value(align & Qt::AlignVertical_Mask);
    QAbstractProperty::set_value(value);

    connect_children();
}

void QAlignmentProperty::child_value_changed(const QVariant &, const QVariant &)
{
    int align=m_horizonta->get_value().toInt() | m_vertical->get_value().toInt();
    QAbstractProperty::set_value(align);
}

void QAlignmentProperty::make_value()
{
    m_value=m_horizonta->get_value().toInt() | m_vertical->get_value().toInt();
}

void QAlignmentProperty::write_value()
{

}
