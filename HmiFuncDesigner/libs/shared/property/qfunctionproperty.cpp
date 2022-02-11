#include "qfunctionproperty.h"

QFunctionProperty::QFunctionProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Function");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QFunctionProperty::getValueText()
{
    return getValue().toString();
}

QIcon QFunctionProperty::getValueIcon()
{
    return QIcon();
}


