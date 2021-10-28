#include "qfloatproperty.h"

QFloatProperty::QFloatProperty(QAbstractProperty *parent) : 
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Float");
}

QString QFloatProperty::getValueText()
{
    return get_value().toString();
}

QIcon QFloatProperty::getValueIcon()
{
    return QIcon();
}

