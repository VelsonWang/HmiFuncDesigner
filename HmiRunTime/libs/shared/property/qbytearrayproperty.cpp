#include "qbytearrayproperty.h"

QByteArrayProperty::QByteArrayProperty(QAbstractProperty *parent) : 
    QAbstractProperty(parent)
{
    setObjectProperty("type", "ByteArray");
}

QString QByteArrayProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QByteArrayProperty::get_value_icon()
{
    return QIcon();
}

