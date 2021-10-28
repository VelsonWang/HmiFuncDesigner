#include "qtagtextlistproperty.h"

QTagTextListProperty::QTagTextListProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "TagTextList");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QTagTextListProperty::getValueText()
{
    return getValue().toString();
}

QIcon QTagTextListProperty::getValueIcon()
{
    return QIcon();
}


