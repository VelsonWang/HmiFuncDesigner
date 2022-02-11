#include "qfileproperty.h"

QFileProperty::QFileProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "File");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QFileProperty::get_value_text()
{
    //return get_value().toString();
    return tr("文件数据");
}

QIcon QFileProperty::get_value_icon()
{
    return QIcon();
}


