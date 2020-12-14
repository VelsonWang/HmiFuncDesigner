#include "qabstractwidgethost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QDynamicPropertyChangeEvent>

QAbstractWidgetHost::QAbstractWidgetHost(QAbstractHost *parent):
    QAbstractHost(parent)
{
}

void QAbstractWidgetHost::init_property()
{
    QAbstractHost::init_property();

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
        pro->set_property("name","geometry");
        pro->set_attribute("show_name",tr("Geometry"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        pro->set_attribute(ATTR_CAN_SAME,true);
        m_object->setProperty("geometry",QRect(0,0,100,70));
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","enabled");
        pro->set_attribute("show_name",tr("Enabled"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Font");
    if(pro!=NULL)
    {
        pro->set_property("name","font");
        pro->set_attribute("show_name",tr("Font"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Cursor");
    if(pro!=NULL)
    {
        pro->set_property("name","cursor");
        pro->set_attribute("show_name",tr("Cursor"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }


    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","focusPolicy");
        pro->set_attribute("show_name",tr("FocusPolicy"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
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
        pro->set_attribute("items",v);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("String");
    if(pro!=NULL)
    {
        pro->set_property("name","toolTip");
        pro->set_attribute("show_name",tr("ToolTip"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }
    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","mousePress");
        pro->set_attribute("show_name",tr("MousePress"));
        pro->set_attribute("group","Events");
        pro->set_attribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->set_attribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","mouseMove");
        pro->set_attribute("show_name",tr("MouseMove"));
        pro->set_attribute("group","Events");
        pro->set_attribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->set_attribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","mouseRelease");
        pro->set_attribute("show_name",tr("MouseRelease"));
        pro->set_attribute("group","Events");
        pro->set_attribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->set_attribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","doubleClick");
        pro->set_attribute("show_name",tr("DoubleClick"));
        pro->set_attribute("group","Events");
        pro->set_attribute("param",QStringList()<<"_x"<<"_y"<<"_button");
        pro->set_attribute("method_tips",tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("StyleSheet");
    if(pro!=NULL)
    {
        pro->set_property("name","background");
        pro->set_attribute("show_name",tr("Background"));
        pro->set_attribute("group","Style Sheet");

        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("StyleSheet");
    if(pro!=NULL)
    {
        pro->set_property("name","border");
        pro->set_attribute("show_name",tr("Border"));
        pro->set_attribute("group","Style Sheet");

        insert_property(pro);
    }
}

void QAbstractWidgetHost::setGeometry(int x, int y, int width, int height)
{
    QRect rect(x,y,width,height);
    set_property_value("geometry",rect);
}

void QAbstractWidgetHost::setEnabled(bool enabled)
{
    set_property_value("enabled",enabled);
}

void QAbstractWidgetHost::setToolTip(const QString &tooltip)
{
    set_property_value("toolTip",tooltip);
}

int QAbstractWidgetHost::x()
{
    return get_property_value("geometry").toRect().x();
}

int QAbstractWidgetHost::y()
{
    return get_property_value("geometry").toRect().y();
}

int QAbstractWidgetHost::width()
{
    return get_property_value("geometry").toRect().width();
}

int QAbstractWidgetHost::height()
{
    return get_property_value("geometry").toRect().height();
}

bool QAbstractWidgetHost::enabled()
{
    return get_property_value("enabled").toBool();
}

QString QAbstractWidgetHost::toolTip()
{
    return get_property_value("toolTip").toString();
}

void QAbstractWidgetHost::setCursor(int cursor)
{
    QCursor c((Qt::CursorShape)cursor);
    QVariant v;
    v.setValue<QCursor>(c);
    set_property_value("cursor",v);
}

int QAbstractWidgetHost::cursor()
{
    QCursor c=get_property_value("cursor").value<QCursor>();
    return c.shape();
}
