#include "fileproperty.h"

FileProperty::FileProperty(const QString &pname)
    : Property(pname)
{
}

QVariant FileProperty::data(int column, int role) {

    if (column == ColumnData && (Qt::DisplayRole == role)) {
        return value;
    }
    else if(Qt::EditRole == role)
        return QVariant();
    else
        return Property::data(column, role);
}

QWidget* FileProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate) {

    Q_UNUSED(options)
    Q_UNUSED(delegate)

    FilePropertyEditor *tmpWidget = new FilePropertyEditor(parent);
    tmpWidget->setFile(value.toString());
    return tmpWidget;
}

bool FileProperty::setEditorData(QWidget* editor)
{
    FilePropertyEditor* tmpWidget = qobject_cast<FilePropertyEditor*>(editor);
    if(tmpWidget)
        tmpWidget->setFile(value.toString());
    else
        return false;

    return true;
}

QVariant FileProperty::getEditorData(QWidget* editor) const
{
    FilePropertyEditor* tmpWidget = qobject_cast<FilePropertyEditor*>(editor);
    if(tmpWidget)
        return tmpWidget->getFile();

    return QVariant();
}

