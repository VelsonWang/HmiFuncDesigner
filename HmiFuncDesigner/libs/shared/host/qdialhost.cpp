#include "qdialhost.h"

#include <QDial>

QDialHost::QDialHost(QAbstractHost *parent):
    QAbstractSliderHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QDialHost::get_show_name()
{
    return tr("Dial");
}

QString QDialHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QDialHost::get_show_icon()
{
    return ":/images/dial.png";
}

void QDialHost::create_object()
{
    m_object=new QDial();
    m_object->setObjectName("dial");
}

void QDialHost::init_property()
{
    QAbstractSliderHost::init_property();
    set_property_value("geometry",QRect(0,0,60,60));
}

void QDialHost::setWrapping(bool wrapping)
{
    set_property_value("wrapping",wrapping);
}

bool QDialHost::wrapping()
{
    return get_property_value("wrapping").toBool();
}

void QDialHost::setNotchesVisible(bool notchesVisible)
{
    set_property_value("notchesVisible",notchesVisible);
}

bool QDialHost::notchesVisible()
{
    return get_property_value("notchesVisible").toBool();
}

void QDialHost::setNotchTarget(float notchTarget)
{
    set_property_value("notchTarget",notchTarget);
}

float QDialHost::notchTarget()
{
    return get_property_value("notchTarget").toBool();
}
