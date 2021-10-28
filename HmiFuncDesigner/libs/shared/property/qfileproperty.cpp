#include "qfileproperty.h"

QFileProperty::QFileProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "File");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QFileProperty::getValueText()
{
    //return get_value().toString();
    return tr("文件数据");
}

QIcon QFileProperty::getValueIcon()
{
    return QIcon();
}


