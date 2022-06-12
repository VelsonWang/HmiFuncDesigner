#include "qimageproperty.h"

QImageProperty::QImageProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Image");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QImageProperty::getValueText()
{
    QStringList szListInfo = getValue().toString().split("|");
    if(szListInfo.size() == 2) {
        return szListInfo.at(0); // 图片名称|图片BASE64数据
    }
    return QString();
}

QIcon QImageProperty::getValueIcon()
{
    return QIcon();
}


