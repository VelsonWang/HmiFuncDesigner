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

QString QFrameHost::getShowName()
{
    return tr("Frame");
}

QString QFrameHost::getShowGroup()
{
    return tr("Containners");
}


QString QFrameHost::getShowIcon()
{
    return ":/images/frame.png";
}

void QFrameHost::createObject()
{
    m_object=new QFrame();
    m_object->setObjectName("frame");
}

void QFrameHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->setProperty("name","frameShape");
        pro->setAttribute("show_name",tr("FrameShape"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
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
        pro->setAttribute("items",v);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->setProperty("name","frameShadow");
        pro->setAttribute("show_name",tr("FrameShadow"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
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
        pro->setAttribute("items",v);
        insertProperty(pro);
    }
}

void QFrameHost::setFrameShape(int frameShape)
{
    setPropertyValue("frameShape",frameShape);
}

int QFrameHost::frameShape()
{
    return getPropertyValue("frameShape").toInt();
}

void QFrameHost::setFrameShadow(int frameShadow)
{
    setPropertyValue("frameShadow",frameShadow);
}

int QFrameHost::frameShadow()
{
    return getPropertyValue("frameShape").toInt();
}
