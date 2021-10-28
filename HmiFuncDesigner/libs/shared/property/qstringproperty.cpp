#include "qstringproperty.h"

QStringProperty::QStringProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "String");
    setObjectProperty("tr", "false");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QStringProperty::getValueText()
{
    return getValue().toString();
}

QIcon QStringProperty::getValueIcon()
{
    return QIcon();
}


