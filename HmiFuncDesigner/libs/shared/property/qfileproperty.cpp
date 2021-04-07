#include "qfileproperty.h"

QFileProperty::QFileProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "File");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QFileProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QFileProperty::get_value_icon()
{
    return QIcon();
}


