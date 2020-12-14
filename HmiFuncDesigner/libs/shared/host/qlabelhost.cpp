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

QString QLabelHost::get_show_name()
{
    return tr("Label");
}

QString QLabelHost::get_show_group()
{
    return tr("Display Widgets");
}

QString QLabelHost::get_show_icon()
{
    return ":/images/label.png";
}

void QLabelHost::create_object()
{
    m_object=new QLabel();
    m_object->setObjectName("label");
}

void QLabelHost::init_property()
{
    QFrameHost::init_property();

    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("String");
    if(pro!=NULL)
    {
        pro->set_property("name","text");
        pro->set_attribute("show_name",tr("Text"));
        pro->set_attribute("group","Attributes");
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","wordWrap");
        pro->set_attribute("show_name",tr("WordWrap"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Alignment");
    if(pro!=NULL)
    {
        pro->set_property("name","alignment");
        pro->set_attribute("show_name",tr("Alignment"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("StyleSheet");
    if(pro!=NULL)
    {
        pro->set_property("name","text_sheet");
        pro->set_attribute("show_name",tr("Text"));
        pro->set_attribute("group","Style Sheet");

        insert_property(pro);
    }

    set_property_value("geometry",QRect(0,0,80,20));
    set_property_value("text","label");
}

void QLabelHost::setText(const QString &text)
{
    set_property_value("text",text);
}

QString QLabelHost::text()
{
    return get_property_value("text").toString();
}

void QLabelHost::setWordWrap(bool wordWrap)
{
    set_property_value("wordWrap",wordWrap);
}

bool QLabelHost::wordWrap()
{
    return get_property_value("wordWrap").toBool();
}

void QLabelHost::setAlignment(int alignment)
{
    set_property_value("alignment",(Qt::AlignmentFlag)alignment);
}

int QLabelHost::alignment()
{
    return get_property_value("alignment").toInt();
}
