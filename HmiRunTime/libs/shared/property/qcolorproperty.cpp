#include "qcolorproperty.h"

QColorProperty::QColorProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Color");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QColorProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QColorProperty::get_value_icon()
{
    return QIcon();
}


