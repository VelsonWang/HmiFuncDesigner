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
    setProperty("sheet_state", v);

    QAbstractProperty* pro;

    pro = QPropertyFactory::create_property("Rect");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "geometry");
        pro->setAttribute("show_name", tr("坐标大小")); // tr("Geometry")
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_NEEDSAVE, true);
        pro->setAttribute(ATTR_CAN_SAME, true);
        m_object->setProperty("geometry", QRect(0, 0, 100, 70));
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Bool");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "enabled");
        pro->setAttribute("show_name", tr("使能")); // tr("Enabled")
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Font");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "font");
        pro->setAttribute("show_name", tr("字体")); // tr("Font")
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Cursor");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "cursor");
        pro->setAttribute("show_name", tr("光标")); // tr("Cursor")
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }


    pro = QPropertyFactory::create_property("Enum");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "focusPolicy");
        pro->setAttribute("show_name", tr("焦点策略")); // tr("FocusPolicy")
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
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

    pro = QPropertyFactory::create_property("String");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "toolTip");
        pro->setAttribute("show_name", tr("工具提示")); // tr("ToolTip")
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }
    pro = QPropertyFactory::create_property("Script");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "mousePress");
        pro->setAttribute("show_name", tr("鼠标按下")); // tr("MousePress")
        pro->setAttribute("group", "Events");
        pro->setAttribute("param", QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips", tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Script");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "mouseMove");
        pro->setAttribute("show_name", tr("鼠标移动")); // tr("MouseMove")
        pro->setAttribute("group", "Events");
        pro->setAttribute("param", QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips", tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Script");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "mouseRelease");
        pro->setAttribute("show_name", tr("鼠标抬起")); // tr("MouseRelease")
        pro->setAttribute("group", "Events");
        pro->setAttribute("param", QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips", tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Script");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "doubleClick");
        pro->setAttribute("show_name", tr("鼠标双击")); // tr("DoubleClick")
        pro->setAttribute("group", "Events");
        pro->setAttribute("param", QStringList()<<"_x"<<"_y"<<"_button");
        pro->setAttribute("method_tips", tr("[_x,_y] is the mouse position.\n[_button] is the mouse button type {0:nobutton 1:leftbutton 2:rightbutton 3:middlebutton}."));
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("StyleSheet");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "background");
        pro->setAttribute("show_name", tr("背景颜色")); // tr("Background")
        pro->setAttribute("group", "Style Sheet");
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("StyleSheet");
    if(pro != Q_NULLPTR) {
        pro->setObjectProperty("name", "border");
        pro->setAttribute("show_name", tr("边框")); // tr("Border")
        pro->setAttribute("group", "Style Sheet");
        insertProperty(pro);
    }
}

void QAbstractWidgetHost::setGeometry(int x, int y, int width, int height)
{
    QRect rect(x, y, width, height);
    setPropertyValue("geometry", rect);
}

void QAbstractWidgetHost::setEnabled(bool enabled)
{
    setPropertyValue("enabled", enabled);
}

void QAbstractWidgetHost::setToolTip(const QString &tooltip)
{
    setPropertyValue("toolTip", tooltip);
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
    setPropertyValue("cursor", v);
}

int QAbstractWidgetHost::cursor()
{
    QCursor c=getPropertyValue("cursor").value<QCursor>();
    return c.shape();
}
