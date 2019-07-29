#include "emptyproperty.h"

EmptyProperty::EmptyProperty(const QString& name)
    : Property(name)
{
}

QVariant EmptyProperty::data (int column, int role)
{
    if(column == ColumnData && (Qt::DisplayRole == role || Qt::EditRole == role))
        return QVariant();
    else if(role == Qt::BackgroundRole)
        return QBrush(QColor(217,217,217));
    else if(role == Qt::FontRole)
    { QFont tmpFont; tmpFont.setBold(true); return tmpFont; }
    else
        return Property::data(column, role);
}

QWidget* EmptyProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(parent);
    Q_UNUSED(delegate);
    setPropertyEditor(nullptr);

    return NULL;
}

QVariant EmptyProperty::getEditorData(QWidget* editor) const
{
    Q_UNUSED(editor);

    return QVariant();
}

Qt::ItemFlags EmptyProperty::flags(int column)
{
    Q_UNUSED(column);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
