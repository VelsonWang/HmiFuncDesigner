#include "qstringproperty.h"

QStringProperty::QStringProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setObjectProperty("type", "String");
    setObjectProperty("tr", "false");
    setAttribute(ATTR_RESET_ABLEABLE,false);
}

QString QStringProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QStringProperty::get_value_icon()
{
    return QIcon();
}
