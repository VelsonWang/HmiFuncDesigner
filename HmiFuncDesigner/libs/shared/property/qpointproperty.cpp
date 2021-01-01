#include "qpointproperty.h"

#include "qintproperty.h"

QPointProperty::QPointProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Point");

    m_x=new QIntProperty(this);
    m_x->setAttribute("show_name",tr("x"));
    m_x->setObjectProperty("name", "x");

    m_y=new QIntProperty(this);
    m_y->setAttribute("show_name",tr("y"));
    m_y->setObjectProperty("name", "y");
}

void QPointProperty::set_value(const QVariant &value)
{
    QPoint pt=value.toPoint();

    disconnect_children();

    m_x->set_value(pt.x());
    m_y->set_value(pt.y());
    QAbstractProperty::set_value(value);

    connect_children();
}

void QPointProperty::child_value_changed(const QVariant &, const QVariant &)
{
    QPoint pt;
    pt.setX(m_x->get_value().toInt());
    pt.setY(m_y->get_value().toInt());
    QAbstractProperty::set_value(pt);
}

QString QPointProperty::get_value_text()
{
    return QString("%1 x %2").arg(m_x->get_value().toInt()).arg(m_y->get_value().toInt());
}

QIcon QPointProperty::get_value_icon()
{
    return QIcon();
}

void QPointProperty::make_value()
{
    QPoint pt;
    pt.setX(m_x->get_value().toInt());
    pt.setY(m_y->get_value().toInt());
    m_value.setValue<QPoint>(pt);
}

void QPointProperty::write_value()
{

}
