#include "qscrollbarhost.h"

#include <QScrollBar>

QScrollBarHost::QScrollBarHost(QAbstractHost *parent):
    QAbstractSliderHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QScrollBarHost::getShowName()
{
    return tr("Scroll Bar");
}

QString QScrollBarHost::getShowGroup()
{
    return tr("Input Widgets");
}

QString QScrollBarHost::getShowIcon()
{
    return ":/images/hscrollbar.png";
}

void QScrollBarHost::createObject()
{
    m_object=new QScrollBar();
    m_object->setObjectName("scrollbar");
}

void QScrollBarHost::initProperty()
{
    QAbstractSliderHost::initProperty();
    setPropertyValue("geometry",QRect(0,0,100,20));
}
