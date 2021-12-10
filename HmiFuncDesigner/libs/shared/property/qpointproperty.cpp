#include "qpointproperty.h"
#include "qintproperty.h"

QPointProperty::QPointProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Point");

    m_x = new QIntProperty(this);
    m_x->setAttribute("show_name", tr("水平原点")); // tr("x")
    m_x->setObjectProperty("name", "x");

    m_y = new QIntProperty(this);
    m_y->setAttribute("show_name", tr("垂直原点")); // tr("y")
    m_y->setObjectProperty("name", "y");
}

void QPointProperty::setValue(const QVariant &value)
{
    QPoint pt = value.toPoint();
    disconnect_children();
    m_x->setValue(pt.x());
    m_y->setValue(pt.y());
    QAbstractProperty::setValue(value);
    connect_children();
}

void QPointProperty::child_value_changed(const QVariant &, const QVariant &)
{
    QPoint pt;
    pt.setX(m_x->getValue().toInt());
    pt.setY(m_y->getValue().toInt());
    QAbstractProperty::setValue(pt);
}

QString QPointProperty::getValueText()
{
    return QString("%1 x %2").arg(m_x->getValue().toInt()).arg(m_y->getValue().toInt());
}

QIcon QPointProperty::getValueIcon()
{
    return QIcon();
}

void QPointProperty::makeValue()
{
    QPoint pt;
    pt.setX(m_x->getValue().toInt());
    pt.setY(m_y->getValue().toInt());
    m_value.setValue<QPoint>(pt);
}

void QPointProperty::writeValue()
{

}


