#include "qpushbuttonhost.h"

#include <QPushButton>

QPushButtonHost::QPushButtonHost(QAbstractHost *parent):
    QAbstractButtonHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QPushButtonHost::get_show_name()
{
    return tr("Push Button");
}

QString QPushButtonHost::get_show_group()
{
    return tr("Buttons");
}

QString QPushButtonHost::get_show_icon()
{
    return ":/images/pushbutton.png";
}

void QPushButtonHost::create_object()
{
    m_object=new QPushButton();
    m_object->setObjectName("pushbutton");
}

void QPushButtonHost::init_property()
{
    QAbstractButtonHost::init_property();

    set_property_value("geometry",QRect(0,0,100,30));
    set_property_value("text","button");
}
