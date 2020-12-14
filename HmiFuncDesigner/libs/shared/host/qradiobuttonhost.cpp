#include "qradiobuttonhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"


#include <QRadioButton>

QRadioButtonHost::QRadioButtonHost(QAbstractHost *parent):
    QAbstractButtonHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QRadioButtonHost::get_show_name()
{
    return tr("Radio Button");
}

QString QRadioButtonHost::get_show_group()
{
    return tr("Buttons");
}

QString QRadioButtonHost::get_show_icon()
{
    return ":/images/radiobutton.png";
}

void QRadioButtonHost::create_object()
{
    m_object=new QRadioButton();
    m_object->setObjectName("radiobutton");
}

void QRadioButtonHost::init_property()
{
    QAbstractButtonHost::init_property();

    SheetItems items=property("sheet_state").value<SheetItems>();

    tagSheetItem item;

    item.m_item_id=SSID_On;
    items.insert(item.m_item_id,item);
    item.m_item_id=SSID_Off;
    items.insert(item.m_item_id,item);

    QVariant v;
    v.setValue<SheetItems>(items);
    setProperty("sheet_state",v);

    set_property_value("geometry",QRect(0,0,100,20));
    set_property_value("text","radio button");
    remove_property("checkable");
}
