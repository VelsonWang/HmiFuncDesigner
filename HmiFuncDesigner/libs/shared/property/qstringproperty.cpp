#include "qstringproperty.h"

QStringProperty::QStringProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    set_property("type","String");
    set_property("tr","false");
    set_attribute(ATTR_RESET_ABLEABLE,false);
}

QString QStringProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QStringProperty::get_value_icon()
{
    return QIcon();
}
