#include "qrectproperty.h"
#include "qintproperty.h"

QRectProperty::QRectProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Rect");

    m_x = new QIntProperty(this);
    m_x->setAttribute("show_name", tr("水平原点")); // tr("x")
    m_x->setObjectProperty("name", "x");

    m_y = new QIntProperty(this);
    m_y->setAttribute("show_name", tr("垂直原点")); // tr("y")
    m_y->setObjectProperty("name", "y");

    m_width = new QIntProperty(this);
    m_width->setAttribute("show_name", tr("宽度")); // tr("Width")
    m_width->setObjectProperty("name", "Width");

    m_height = new QIntProperty(this);
    m_height->setAttribute("show_name", tr("高度")); // tr("Height")
    m_height->setObjectProperty("name", "Height");
}

void QRectProperty::setValue(const QVariant &value)
{
    QRect re = value.toRect();
    disconnect_children();
    if(m_x->getAttribute(ATTR_VISIBLE).toBool()) {
        m_x->setValue(re.x());
        m_y->setValue(re.y());
    } else {
        m_x->setValue(0);
        m_y->setValue(0);
    }
    m_width->setValue(re.width());
    m_height->setValue(re.height());
    QAbstractProperty::setValue(value);
    connect_children();
}

void QRectProperty::child_value_changed(const QVariant &, const QVariant &)
{
    QRect re;
    re.setX(m_x->getValue().toInt());
    re.setY(m_y->getValue().toInt());
    re.setWidth(m_width->getValue().toInt());
    re.setHeight(m_height->getValue().toInt());
    QAbstractProperty::setValue(re);
}

QString QRectProperty::getValueText()
{
    if(m_x->getAttribute(ATTR_VISIBLE).toBool()) {
        return QString("[(%1,%2),%3 x %4]")
               .arg(m_x->getValue().toInt())
               .arg(m_y->getValue().toInt())
               .arg(m_width->getValue().toInt())
               .arg(m_height->getValue().toInt());
    } else {
        return QString("%1 x %2").arg(m_width->getValue().toInt()).arg(m_height->getValue().toInt());
    }
}

QIcon QRectProperty::getValueIcon()
{
    return QIcon();
}

QVariant QRectProperty::getValue()
{
    QVariant v = QAbstractProperty::getValue();
    if(!m_x->getAttribute(ATTR_VISIBLE).toBool()) {
        QRect rect = v.toRect();
        rect.setRect(0, 0, rect.width(), rect.height());
        v.setValue<QRect>(rect);
    }
    return v;
}

void QRectProperty::makeValue()
{
    QRect re;
    re.setX(m_x->getValue().toInt());
    re.setY(m_y->getValue().toInt());
    re.setWidth(m_width->getValue().toInt());
    re.setHeight(m_height->getValue().toInt());
    m_value.setValue<QRect>(re);
}

void QRectProperty::writeValue()
{

}


