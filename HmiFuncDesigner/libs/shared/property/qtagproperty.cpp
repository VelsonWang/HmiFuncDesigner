#include "qtagproperty.h"
#include "../qcommonstruct.h"

QTagProperty::QTagProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Tag");
}

QIcon QTagProperty::getValueIcon()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, items) {
        if(item.m_value == getValue()) {
            return QIcon(item.m_icon);
        }
    }
    return QIcon();
}

QString QTagProperty::getValueText()
{
    ComboItems items = this->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, items) {
        if(item.m_value == getValue()) {
            return item.m_text;
        }
    }
    return getValue().toString();
}


