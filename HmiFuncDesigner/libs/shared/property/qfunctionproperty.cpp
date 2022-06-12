#include "qfunctionproperty.h"

QFunctionProperty::QFunctionProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Function");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QFunctionProperty::getValueText()
{
    QString text = getValue().toString();
    if(text.isEmpty()) {
        return text;
    }
    QStringList szListInfo = text.split("|");
    return QString(tr("已编辑%1个功能")).arg(szListInfo.size());
}

QIcon QFunctionProperty::getValueIcon()
{
    return QIcon();
}


