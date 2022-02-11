#include "qscriptproperty.h"

QScriptProperty::QScriptProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Script");
}

QString QScriptProperty::get_value_text()
{
    QString str = get_value().toString();
    str.replace("\r\n", " ");
    str.replace("\n", " ");
    return str;
}

QIcon QScriptProperty::get_value_icon()
{
    return QIcon();
}

