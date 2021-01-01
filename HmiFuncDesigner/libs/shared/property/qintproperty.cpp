#include "qintproperty.h"

QIntProperty::QIntProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Number");
}

QString QIntProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QIntProperty::get_value_icon()
{
    return QIcon();
}
