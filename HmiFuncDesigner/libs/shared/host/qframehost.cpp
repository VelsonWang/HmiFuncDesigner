#include "qframehost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QFrame>

QFrameHost::QFrameHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame",true);
    setProperty("accept_drop",true);
}

QString QFrameHost::get_show_name()
{
    return tr("Frame");
}

QString QFrameHost::get_show_group()
{
    return tr("Containners");
}


QString QFrameHost::get_show_icon()
{
    return ":/images/frame.png";
}

void QFrameHost::create_object()
{
    m_object=new QFrame();
    m_object->setObjectName("frame");
}

void QFrameHost::init_property()
{
    QWidgetHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","frameShape");
        pro->set_attribute("show_name",tr("FrameShape"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("NoFrame");
        item.m_value=QFrame::NoFrame;
        items.append(item);

        item.m_text=tr("Box");
        item.m_value=QFrame::Box;
        items.append(item);

        item.m_text=tr("Panel");
        item.m_value=QFrame::Panel;
        items.append(item);

        item.m_text=tr("StyledPanel");
        item.m_value=QFrame::StyledPanel;
        items.append(item);

        item.m_text=tr("HLine");
        item.m_value=QFrame::HLine;
        items.append(item);

        item.m_text=tr("VLine");
        item.m_value=QFrame::VLine;
        items.append(item);

        item.m_text=tr("WinPanel");
        item.m_value=QFrame::WinPanel;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","frameShadow");
        pro->set_attribute("show_name",tr("FrameShadow"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("Plain");
        item.m_value=QFrame::Plain;
        items.append(item);

        item.m_text=tr("Raised");
        item.m_value=QFrame::Raised;
        items.append(item);

        item.m_text=tr("Sunken");
        item.m_value=QFrame::Sunken;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        insert_property(pro);
    }
}

void QFrameHost::setFrameShape(int frameShape)
{
    set_property_value("frameShape",frameShape);
}

int QFrameHost::frameShape()
{
    return get_property_value("frameShape").toInt();
}

void QFrameHost::setFrameShadow(int frameShadow)
{
    set_property_value("frameShadow",frameShadow);
}

int QFrameHost::frameShadow()
{
    return get_property_value("frameShape").toInt();
}
