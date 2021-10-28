#include "qintproperty.h"

QIntProperty::QIntProperty(QAbstractProperty *parent) : 
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Number");
}

QString QIntProperty::getValueText()
{
    return get_value().toString();
}

QIcon QIntProperty::getValueIcon()
{
    return QIcon();
}


