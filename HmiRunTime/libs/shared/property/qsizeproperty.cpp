#include "qsizeproperty.h"

#include "qintproperty.h"

QSizeProperty::QSizeProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Size");

    m_width = new QIntProperty(this);
    m_width->setAttribute("show_name", tr("宽度")); // tr("Width")
    m_width->setObjectProperty("name", "Width");

    m_height = new QIntProperty(this);
    m_height->setAttribute("show_name", tr("高度")); // tr("Height")
    m_height->setObjectProperty("name", "Height");
}

void QSizeProperty::set_value(const QVariant &value)
{
    QSize sz = value.toSize();
    disconnect_children();
    m_width->set_value(sz.width());
    m_height->set_value(sz.height());
    QAbstractProperty::set_value(value);
    connect_children();
}

void QSizeProperty::child_value_changed(const QVariant &, const QVariant &)
{
    QSize sz;
    sz.setWidth(m_width->get_value().toInt());
    sz.setHeight(m_height->get_value().toInt());
    QAbstractProperty::set_value(sz);
}

QString QSizeProperty::get_value_text()
{
    return QString("%1 x %2").arg(m_width->get_value().toInt()).arg(m_height->get_value().toInt());
}

QIcon QSizeProperty::get_value_icon()
{
    return QIcon();
}

void QSizeProperty::make_value()
{
    QSize sz;
    sz.setWidth(m_width->get_value().toInt());
    sz.setHeight(m_height->get_value().toInt());
    m_value.setValue<QSize>(sz);
}

void QSizeProperty::write_value()
{

}

