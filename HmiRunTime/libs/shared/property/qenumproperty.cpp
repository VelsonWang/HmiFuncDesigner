#include "qenumproperty.h"

#include "../qcommonstruct.h"

QEnumProperty::QEnumProperty(QAbstractProperty *parent) : 
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Enum");
}

QIcon QEnumProperty::get_value_icon()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item,items) {
        if(item.m_value == get_value()) {
            return QIcon(item.m_icon);
        }
    }
    return QIcon();
}


QString QEnumProperty::get_value_text()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, items) {
        if(item.m_value == get_value()) {
            return item.m_text;
        }
    }
    return "";
}


