#include "tagcolorlistproperty.h"


TagColorListProperty::TagColorListProperty(const QString &pname)
    : Property(pname) {
    value.setValue(QStringList());
    value.convert(QVariant::StringList);
}

QVariant TagColorListProperty::data(int column, int role) {
    if(Qt::EditRole == role)
        return QVariant();
    else
        return Property::data(column, role);
}

QWidget* TagColorListProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate) {
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    TagColorListPropertyEditor *tmpWidget = new TagColorListPropertyEditor(parent);
    tmpWidget->setValueColorList(value.toStringList());
    setPropertyEditor(tmpWidget);

    return tmpWidget;
}

bool TagColorListProperty::setEditorData(QWidget* editor) {
    TagColorListPropertyEditor* tmpWidget = qobject_cast<TagColorListPropertyEditor *>(editor);
    if(tmpWidget)
        tmpWidget->setValueColorList(value.toStringList());
    else
        return false;

    return true;
}

QVariant TagColorListProperty::getEditorData(QWidget* editor) const {
    TagColorListPropertyEditor* tmpWidget = qobject_cast<TagColorListPropertyEditor *>(editor);
    if(tmpWidget)
        return tmpWidget->getValueColorList();

    return QVariant();
}

