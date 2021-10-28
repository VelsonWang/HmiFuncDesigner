#include "qscriptproperty.h"

QScriptProperty::QScriptProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Script");
}

QString QScriptProperty::getValueText()
{
    QString str = get_value().toString();
    str.replace("\r\n", " ");
    str.replace("\n", " ");
    return str;
}

QIcon QScriptProperty::getValueIcon()
{
    return QIcon();
}

