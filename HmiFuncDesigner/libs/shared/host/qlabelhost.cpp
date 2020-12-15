#include "qlabelhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QLabel>

QLabelHost::QLabelHost(QAbstractHost *parent):
    QFrameHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QLabelHost::getShowName()
{
    return tr("Label");
}

QString QLabelHost::getShowGroup()
{
    return tr("Display Widgets");
}

QString QLabelHost::getShowIcon()
{
    return ":/images/label.png";
}

void QLabelHost::createObject()
{
    m_object=new QLabel();
    m_object->setObjectName("label");
}

void QLabelHost::initProperty()
{
    QFrameHost::initProperty();

    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("String");
    if(pro!=NULL)
    {
        pro->setProperty("name","text");
        pro->setAttribute("show_name",tr("Text"));
        pro->setAttribute("group","Attributes");
        insertProperty(pro,1);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->setProperty("name","wordWrap");
        pro->setAttribute("show_name",tr("WordWrap"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Alignment");
    if(pro!=NULL)
    {
        pro->setProperty("name","alignment");
        pro->setAttribute("show_name",tr("Alignment"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("StyleSheet");
    if(pro!=NULL)
    {
        pro->setProperty("name","text_sheet");
        pro->setAttribute("show_name",tr("Text"));
        pro->setAttribute("group","Style Sheet");

        insertProperty(pro);
    }

    setPropertyValue("geometry",QRect(0,0,80,20));
    setPropertyValue("text","label");
}

void QLabelHost::setText(const QString &text)
{
    setPropertyValue("text",text);
}

QString QLabelHost::text()
{
    return getPropertyValue("text").toString();
}

void QLabelHost::setWordWrap(bool wordWrap)
{
    setPropertyValue("wordWrap",wordWrap);
}

bool QLabelHost::wordWrap()
{
    return getPropertyValue("wordWrap").toBool();
}

void QLabelHost::setAlignment(int alignment)
{
    setPropertyValue("alignment",(Qt::AlignmentFlag)alignment);
}

int QLabelHost::alignment()
{
    return getPropertyValue("alignment").toInt();
}
