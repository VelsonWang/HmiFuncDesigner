#include "qdialhost.h"

#include <QDial>

QDialHost::QDialHost(QAbstractHost *parent):
    QAbstractSliderHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QDialHost::getShowName()
{
    return tr("调节器");//tr("Dial");
}

QString QDialHost::getShowGroup()
{
    return tr("输入部件"); //tr("Input Widgets");
}

QString QDialHost::getShowIcon()
{
    return ":/images/dial.png";
}

void QDialHost::createObject()
{
    m_object = new QDial();
    m_object->setObjectName("dial");
}

void QDialHost::initProperty()
{
    QAbstractSliderHost::initProperty();
    setPropertyValue("geometry", QRect(0, 0, 60, 60));
}

void QDialHost::setWrapping(bool wrapping)
{
    setPropertyValue("wrapping", wrapping);
}

bool QDialHost::wrapping()
{
    return getPropertyValue("wrapping").toBool();
}

void QDialHost::setNotchesVisible(bool notchesVisible)
{
    setPropertyValue("notchesVisible", notchesVisible);
}

bool QDialHost::notchesVisible()
{
    return getPropertyValue("notchesVisible").toBool();
}

void QDialHost::setNotchTarget(float notchTarget)
{
    setPropertyValue("notchTarget", notchTarget);
}

float QDialHost::notchTarget()
{
    return getPropertyValue("notchTarget").toBool();
}
