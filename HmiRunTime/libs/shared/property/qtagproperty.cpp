#include "qtagproperty.h"
#include "../qcommonstruct.h"

QTagProperty::QTagProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Tag");
}

QIcon QTagProperty::get_value_icon()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, items) {
        if(item.m_value == get_value()) {
            return QIcon(item.m_icon);
        }
    }
    return QIcon();
}

QString QTagProperty::get_value_text()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, items) {
        if(item.m_value == get_value()) {
            return item.m_text;
        }
    }
    return get_value().toString();
}


