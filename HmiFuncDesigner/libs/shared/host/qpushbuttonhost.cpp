#include "qpushbuttonhost.h"

#include <QPushButton>

QPushButtonHost::QPushButtonHost(QAbstractHost *parent):
    QAbstractButtonHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QPushButtonHost::getShowName()
{
    return tr("Push Button");
}

QString QPushButtonHost::getShowGroup()
{
    return tr("Buttons");
}

QString QPushButtonHost::getShowIcon()
{
    return ":/images/pushbutton.png";
}

void QPushButtonHost::createObject()
{
    m_object=new QPushButton();
    m_object->setObjectName("pushbutton");
}

void QPushButtonHost::initProperty()
{
    QAbstractButtonHost::initProperty();

    setPropertyValue("geometry",QRect(0,0,100,30));
    setPropertyValue("text","button");
}
