#include "qintproperty.h"

QIntProperty::QIntProperty(QAbstractProperty *parent) : 
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Number");
}

QString QIntProperty::getValueText()
{
    return getValue().toString();
}

QIcon QIntProperty::getValueIcon()
{
    return QIcon();
}


