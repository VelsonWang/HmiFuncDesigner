#include "qrectproperty.h"

#include "qintproperty.h"

QRectProperty::QRectProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setProperty("type","Rect");

    m_x=new QIntProperty(this);
    m_x->setAttribute("show_name",tr("x"));
    m_x->setProperty("name","x");

    m_y=new QIntProperty(this);
    m_y->setAttribute("show_name",tr("y"));
    m_y->setProperty("name","y");

    m_width=new QIntProperty(this);
    m_width->setAttribute("show_name",tr("Width"));
    m_width->setProperty("name","Width");

    m_height=new QIntProperty(this);
    m_height->setAttribute("show_name",tr("Height"));
    m_height->setProperty("name","Height");
}

void QRectProperty::set_value(const QVariant &value)
{
    QRect re=value.toRect();

    disconnect_children();

    if(m_x->getAttribute(ATTR_VISIBLE).toBool())
    {
        m_x->set_value(re.x());
        m_y->set_value(re.y());
    }
    else
    {
        m_x->set_value(0);
        m_y->set_value(0);
    }
    m_width->set_value(re.width());
    m_height->set_value(re.height());

    QAbstractProperty::set_value(value);

    connect_children();
}

void QRectProperty::child_value_changed(const QVariant &, const QVariant &)
{
    QRect re;
    re.setX(m_x->get_value().toInt());
    re.setY(m_y->get_value().toInt());
    re.setWidth(m_width->get_value().toInt());
    re.setHeight(m_height->get_value().toInt());
    QAbstractProperty::set_value(re);
}

QString QRectProperty::get_value_text()
{
    if(m_x->getAttribute(ATTR_VISIBLE).toBool())
    {
        return QString("[(%1,%2),%3 x %4]")
            .arg(m_x->get_value().toInt())
            .arg(m_y->get_value().toInt())
            .arg(m_width->get_value().toInt())
            .arg(m_height->get_value().toInt());
    }
    else
    {
        return QString("%1 x %2").arg(m_width->get_value().toInt()).arg(m_height->get_value().toInt());
    }
}

QIcon QRectProperty::get_value_icon()
{
    return QIcon();
}

QVariant QRectProperty::get_value()
{
    QVariant v=QAbstractProperty::get_value();
    if(!m_x->getAttribute(ATTR_VISIBLE).toBool())
    {
        QRect rect=v.toRect();
        rect.setRect(0,0,rect.width(),rect.height());
        v.setValue<QRect>(rect);
    }
    return v;
}

void QRectProperty::make_value()
{
    QRect re;
    re.setX(m_x->get_value().toInt());
    re.setY(m_y->get_value().toInt());
    re.setWidth(m_width->get_value().toInt());
    re.setHeight(m_height->get_value().toInt());
    m_value.setValue<QRect>(re);
}

void QRectProperty::write_value()
{

}
