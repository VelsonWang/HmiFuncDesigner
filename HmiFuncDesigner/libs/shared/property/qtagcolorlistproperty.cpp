#include "qtagcolorlistproperty.h"

QTagColorListProperty::QTagColorListProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "TagColorList");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QTagColorListProperty::getValueText()
{
    return get_value().toString();
}

QIcon QTagColorListProperty::getValueIcon()
{
    return QIcon();
}


