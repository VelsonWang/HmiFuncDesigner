#include "qimageproperty.h"

QImageProperty::QImageProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Image");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QImageProperty::get_value_text()
{
    QStringList szListInfo = get_value().toString().split("|");
    if(szListInfo.size() == 2) {
        return szListInfo.at(0); // 图片名称|图片BASE64数据
    }
    return QString();
}

QIcon QImageProperty::get_value_icon()
{
    return QIcon();
}


