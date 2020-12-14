#include "qfloatproperty.h"

QFloatProperty::QFloatProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    set_property("type","Float");
}

QString QFloatProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QFloatProperty::get_value_icon()
{
    return QIcon();
}
