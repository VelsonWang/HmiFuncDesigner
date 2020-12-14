#include "qbytearrayproperty.h"

QByteArrayProperty::QByteArrayProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    set_property("type","ByteArray");
}

QString QByteArrayProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QByteArrayProperty::get_value_icon()
{
    return QIcon();
}
