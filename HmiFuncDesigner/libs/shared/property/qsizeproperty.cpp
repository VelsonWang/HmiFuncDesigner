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

void QSizeProperty::setValue(const QVariant &value)
{
    QSize sz = value.toSize();
    disconnect_children();
    m_width->setValue(sz.width());
    m_height->setValue(sz.height());
    QAbstractProperty::setValue(value);
    connect_children();
}

void QSizeProperty::child_value_changed(const QVariant &, const QVariant &)
{
    QSize sz;
    sz.setWidth(m_width->getValue().toInt());
    sz.setHeight(m_height->getValue().toInt());
    QAbstractProperty::setValue(sz);
}

QString QSizeProperty::getValueText()
{
    return QString("%1 x %2").arg(m_width->getValue().toInt()).arg(m_height->getValue().toInt());
}

QIcon QSizeProperty::getValueIcon()
{
    return QIcon();
}

void QSizeProperty::makeValue()
{
    QSize sz;
    sz.setWidth(m_width->getValue().toInt());
    sz.setHeight(m_height->getValue().toInt());
    m_value.setValue<QSize>(sz);
}

void QSizeProperty::writeValue()
{

}

