#include "qtagcolorlistproperty.h"

QTagColorListProperty::QTagColorListProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "TagColorList");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QTagColorListProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QTagColorListProperty::get_value_icon()
{
    return QIcon();
}


