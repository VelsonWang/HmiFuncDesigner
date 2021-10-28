#include "qfloatproperty.h"

QFloatProperty::QFloatProperty(QAbstractProperty *parent) : 
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Float");
}

QString QFloatProperty::getValueText()
{
    return getValue().toString();
}

QIcon QFloatProperty::getValueIcon()
{
    return QIcon();
}

