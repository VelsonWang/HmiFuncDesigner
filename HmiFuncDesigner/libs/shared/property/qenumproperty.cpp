#include "qenumproperty.h"
#include "../qcommonstruct.h"

QEnumProperty::QEnumProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Enum");
}

QIcon QEnumProperty::getValueIcon()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, items) {
        if(item.m_value == get_value()) {
            return QIcon(item.m_icon);
        }
    }
    return QIcon();
}


QString QEnumProperty::getValueText()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, items) {
        if(item.m_value == get_value()) {
            return item.m_text;
        }
    }
    return "";
}


