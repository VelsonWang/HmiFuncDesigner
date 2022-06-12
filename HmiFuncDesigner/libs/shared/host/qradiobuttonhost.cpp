#include "qradiobuttonhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"


#include <QRadioButton>

QRadioButtonHost::QRadioButtonHost(QAbstractHost *parent):
    QAbstractButtonHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QRadioButtonHost::getShowName()
{
    return tr("单选按钮");//tr("Radio Button")
}

QString QRadioButtonHost::getShowGroup()
{
    return tr("按钮");//tr("Buttons")
}

QString QRadioButtonHost::getShowIcon()
{
    return ":/images/radiobutton.png";
}

void QRadioButtonHost::createObject()
{
    m_object = new QRadioButton();
    m_object->setObjectName("radiobutton");
}

void QRadioButtonHost::initProperty()
{
    QAbstractButtonHost::initProperty();

    SheetItems items = property("sheet_state").value<SheetItems>();

    tagSheetItem item;

    item.m_item_id = SSID_On;
    items.insert(item.m_item_id, item);
    item.m_item_id = SSID_Off;
    items.insert(item.m_item_id, item);

    QVariant v;
    v.setValue<SheetItems>(items);
    setProperty("sheet_state", v);

    setPropertyValue("geometry", QRect(0, 0, 100, 20));
    setPropertyValue("text", "radio button");
    removeProperty("checkable");
}

