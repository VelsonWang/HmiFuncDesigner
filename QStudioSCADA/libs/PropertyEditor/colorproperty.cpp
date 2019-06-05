#include "colorproperty.h"

ColorProperty::ColorProperty(const QString &pname)
    : Property(pname)
{
}

QVariant ColorProperty::data(int column, int role)
{

    if (column == ColumnData && (Qt::DisplayRole == role)) {
        return ColorPropertyEditor::getColorString(value.value<QColor>());
    }
    else if(Qt::EditRole == role)
        return QVariant();
    else if(column == ColumnData && (Qt::DecorationRole == role))
        return ColorPropertyEditor::getColorPixmap(value.value<QColor>());
    else
        return Property::data(column, role);
}

QWidget* ColorProperty::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &options,
                                     const QAbstractItemDelegate *delegate)
{

    Q_UNUSED(options)
    Q_UNUSED(delegate)

    ColorPropertyEditor *tmpWidget = new ColorPropertyEditor(parent);
    tmpWidget->setColor(value.value<QColor>());
    return tmpWidget;
}

bool ColorProperty::setEditorData(QWidget* editor)
{
    ColorPropertyEditor* tmpWidget = qobject_cast<ColorPropertyEditor*>(editor);
    if(tmpWidget)
        tmpWidget->setColor(value.value<QColor>());
    else
        return false;

    return true;
}

QVariant ColorProperty::getEditorData(QWidget* editor) const
{
    ColorPropertyEditor* tmpWidget = qobject_cast<ColorPropertyEditor*>(editor);
    if(tmpWidget)
        return tmpWidget->getColor();

    return QVariant();
}

