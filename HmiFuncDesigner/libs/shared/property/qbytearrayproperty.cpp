#include "qbytearrayproperty.h"

QByteArrayProperty::QByteArrayProperty(QAbstractProperty *parent) : 
    QAbstractProperty(parent)
{
    setObjectProperty("type", "ByteArray");
}

QString QByteArrayProperty::getValueText()
{
    return get_value().toString();
}

QIcon QByteArrayProperty::getValueIcon()
{
    return QIcon();
}

