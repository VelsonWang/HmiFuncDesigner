#include "qframehost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include <QFrame>

QFrameHost::QFrameHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame", true);
    setProperty("accept_drop", true);
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
    m_object = new QFrame();
    m_object->setObjectName("frame");
}

void QFrameHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty *pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "frameShape");
        pObj->setAttribute("show_name", tr("边框形状")); // tr("FrameShape")
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("无边框"); // tr("NoFrame")
        item.m_value = QFrame::NoFrame;
        items.append(item);

        item.m_text = tr("盒子"); // tr("Box")
        item.m_value = QFrame::Box;
        items.append(item);

        item.m_text = tr("面板"); // tr("Panel")
        item.m_value = QFrame::Panel;
        items.append(item);

        item.m_text = tr("风格面板"); // tr("StyledPanel")
        item.m_value = QFrame::StyledPanel;
        items.append(item);

        item.m_text = tr("水平线"); // HLine
        item.m_value = QFrame::HLine;
        items.append(item);

        item.m_text = tr("垂直线"); // tr("VLine")
        item.m_value = QFrame::VLine;
        items.append(item);

        item.m_text = tr("窗口面板"); // tr("WinPanel")
        item.m_value = QFrame::WinPanel;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "frameShadow");
        pObj->setAttribute("show_name", tr("边框阴影")); // tr("FrameShadow")
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("简单"); // tr("Plain")
        item.m_value = QFrame::Plain;
        items.append(item);

        item.m_text = tr("凸起"); // tr("Raised")
        item.m_value = QFrame::Raised;
        items.append(item);

        item.m_text = tr("凹陷"); // tr("Sunken")
        item.m_value = QFrame::Sunken;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        insertProperty(pObj);
    }
}

void QFrameHost::setFrameShape(int frameShape)
{
    setPropertyValue("frameShape", frameShape);
}

int QFrameHost::frameShape()
{
    return getPropertyValue("frameShape").toInt();
}

void QFrameHost::setFrameShadow(int frameShadow)
{
    setPropertyValue("frameShadow", frameShadow);
}

int QFrameHost::frameShadow()
{
    return getPropertyValue("frameShape").toInt();
}

