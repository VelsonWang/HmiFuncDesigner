#include "qtagtextlistproperty.h"

QTagTextListProperty::QTagTextListProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "TagTextList");
    setAttribute(ATTR_RESET_ABLEABLE, false);
}

QString QTagTextListProperty::get_value_text()
{
    return get_value().toString();
}

QIcon QTagTextListProperty::get_value_icon()
{
    return QIcon();
}


