#include "qcolorproperty.h"

QColorProperty::QColorProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Color");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QColorProperty::getValueText()
{
    return getValue().toString();
}

QIcon QColorProperty::getValueIcon()
{
    return QIcon();
}


