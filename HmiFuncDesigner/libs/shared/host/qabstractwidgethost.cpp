#include "qabstractwidgethost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QDynamicPropertyChangeEvent>

QAbstractWidgetHost::QAbstractWidgetHost(QAbstractHost *parent):
    QAbstractHost(parent)
{
}

void QAbstractWidgetHost::initProperty()
{
    QAbstractHost::initProperty();

    QWidget* wid=(QWidget*)m_object;
    wid->setMouseTracking(true);

    SheetItems items=property("sheet_state").value<SheetItems>();

    tagSheetItem item;

    item.m_item_id=SSID_Active;
    items.insert(item.m_item_id,item);
    item.m_item_id=SSID_Enabled;
    items.insert(item.m_item_id,item);
    item.m_item_id=SSID_Disabled;
    items.insert(item.m_item_id,item);
    item.m_item_id=SSID_Hover;
    items.insert(item.m_item_id,item);
    item.m_item_id=SSID_Pressed;
    items.insert(item.m_item_id,item);

    QVariant v;
    v.setValue<SheetItems>(items);
    setProperty("sheet_state",v);


    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("Rect");
    if(pro!=NULL)
    {
        pro->setProperty("name","geometry");
        pro->setAttribute("show_name",tr("Geometry"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_NEEDSAVE,true);
        pro->setAttribute(ATTR_CAN_SAME,true);
        m_object->setProperty("geometry",QRect(0,0,100,70));
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->setProperty("name","enabled");
        pro->setAttribute("show_name",tr("Enabled"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Font");
    if(pro!=NULL)
    {
        pro->setProperty("name","font");
        pro->setAttribute("show_name",tr("Font"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Cursor");
    if(pro!=NULL)
    {
        pro->setProperty("name","cursor");
        pro->setAttribute("show_name",tr("Cursor"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }


    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->setProperty("name","focusPolicy");
        pro->setAttribute("show_name",tr("FocusPolicy"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;
        item.m_text="TabFocus";
        item.m_value=Qt::TabFocus;
        items.append(item);
        item.m_text="ClickFocus";
        item.m_value=Qt::ClickFocus;
        items.append(item);
        item.m_text="StrongFocus";
        item.m_value=Qt::StrongFocus;
        items.append(item);
        item.m_text="WheelFocus";
        item.m_value=Qt::WheelFocus;
        items.append(item);
        item.m_text="NoFocus";
        item.m_value=Qt::NoFocus;
        items.append(item);
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("String");
    if(pro!=NULL)
    {
        pro->setProperty("name","toolTip");
        pro->setAttribute("show_name",tr("ToolTip"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }
    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->setProperty("name","mousePress");
        pro->setAttribute("show_name",tr("MousePress"));
        pro->setAttribute("group","Events");
        pro->setAttribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->setProperty("name","mouseMove");
        pro->setAttribute("show_name",tr("MouseMove"));
        pro->setAttribute("group","Events");
        pro->setAttribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->setProperty("name","mouseRelease");
        pro->setAttribute("show_name",tr("MouseRelease"));
        pro->setAttribute("group","Events");
        pro->setAttribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->setProperty("name","doubleClick");
        pro->setAttribute("show_name",tr("DoubleClick"));
        pro->setAttribute("group","Events");
        pro->setAttribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("StyleSheet");
    if(pro!=NULL)
    {
        pro->setProperty("name","background");
        pro->setAttribute("show_name",tr("Background"));
        pro->setAttribute("group","Style Sheet");

        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("StyleSheet");
    if(pro!=NULL)
    {
        pro->setProperty("name","border");
        pro->setAttribute("show_name",tr("Border"));
        pro->setAttribute("group","Style Sheet");

        insertProperty(pro);
    }
}

void QAbstractWidgetHost::setGeometry(int x, int y, int width, int height)
{
    QRect rect(x,y,width,height);
    setPropertyValue("geometry",rect);
}

void QAbstractWidgetHost::setEnabled(bool enabled)
{
    setPropertyValue("enabled",enabled);
}

void QAbstractWidgetHost::setToolTip(const QString &tooltip)
{
    setPropertyValue("toolTip",tooltip);
}

int QAbstractWidgetHost::x()
{
    return getPropertyValue("geometry").toRect().x();
}

int QAbstractWidgetHost::y()
{
    return getPropertyValue("geometry").toRect().y();
}

int QAbstractWidgetHost::width()
{
    return getPropertyValue("geometry").toRect().width();
}

int QAbstractWidgetHost::height()
{
    return getPropertyValue("geometry").toRect().height();
}

bool QAbstractWidgetHost::enabled()
{
    return getPropertyValue("enabled").toBool();
}

QString QAbstractWidgetHost::toolTip()
{
    return getPropertyValue("toolTip").toString();
}

void QAbstractWidgetHost::setCursor(int cursor)
{
    QCursor c((Qt::CursorShape)cursor);
    QVariant v;
    v.setValue<QCursor>(c);
    setPropertyValue("cursor",v);
}

int QAbstractWidgetHost::cursor()
{
    QCursor c=getPropertyValue("cursor").value<QCursor>();
    return c.shape();
}
