#include "qscrollbarhost.h"

#include <QScrollBar>

QScrollBarHost::QScrollBarHost(QAbstractHost *parent):
    QAbstractSliderHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QScrollBarHost::get_show_name()
{
    return tr("Scroll Bar");
}

QString QScrollBarHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QScrollBarHost::get_show_icon()
{
    return ":/images/hscrollbar.png";
}

void QScrollBarHost::create_object()
{
    m_object=new QScrollBar();
    m_object->setObjectName("scrollbar");
}

void QScrollBarHost::init_property()
{
    QAbstractSliderHost::init_property();
    set_property_value("geometry",QRect(0,0,100,20));
}
