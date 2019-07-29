#include "tagtextlistproperty.h"


TagTextListProperty::TagTextListProperty(const QString &pname)
    : Property(pname)
{
    value.setValue(QStringList());
    value.convert(QVariant::StringList);
}

QVariant TagTextListProperty::data(int column, int role)
{
    if(Qt::EditRole == role)
        return QVariant();
    else
        return Property::data(column, role);
}

QWidget* TagTextListProperty::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &options,
                                           const QAbstractItemDelegate *delegate)
{
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    TagTextListPropertyEditor *tmpWidget = new TagTextListPropertyEditor(parent);
    tmpWidget->setValueTextList(value.toStringList());
    setPropertyEditor(tmpWidget);

    return tmpWidget;
}

bool TagTextListProperty::setEditorData(QWidget* editor)
{
    TagTextListPropertyEditor* tmpWidget = qobject_cast<TagTextListPropertyEditor *>(editor);
    if(tmpWidget)
        tmpWidget->setValueTextList(value.toStringList());
    else
        return false;

    return true;
}

QVariant TagTextListProperty::getEditorData(QWidget* editor) const
{
    TagTextListPropertyEditor* tmpWidget = qobject_cast<TagTextListPropertyEditor *>(editor);
    if(tmpWidget)
        return tmpWidget->getValueTextList();

    return QVariant();
}

