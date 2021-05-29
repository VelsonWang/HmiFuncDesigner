#include "qfunctionproperty.h"

QFunctionProperty::QFunctionProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Function");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QFunctionProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QFunctionProperty::get_value_icon()
{
    return QIcon();
}


