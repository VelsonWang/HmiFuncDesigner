#include "fontproperty.h"

FontProperty::FontProperty(const QString &pname)
    : Property(pname) {

}

QVariant FontProperty::data(int column, int role) {
    if (column == ColumnData && (Qt::DisplayRole == role)) {
        return FontPropertyEditor::getFontString(value.value<QFont>());
    }
    else if(Qt::EditRole == role)
        return QVariant();
    else
        return Property::data(column, role);
}

QWidget* FontProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate) {
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    FontPropertyEditor *tmpWidget = new FontPropertyEditor(parent);
    tmpWidget->setFont(value.value<QFont>());
    return tmpWidget;
}

bool FontProperty::setEditorData(QWidget* editor) {
    FontPropertyEditor* tmpWidget = qobject_cast<FontPropertyEditor*>(editor);
    if(tmpWidget)
        tmpWidget->setFont(value.value<QFont>());
    else
        return false;
    return true;
}

QVariant FontProperty::getEditorData(QWidget* editor) const {
    FontPropertyEditor* tmpWidget = qobject_cast<FontPropertyEditor*>(editor);
    if(tmpWidget)
        return tmpWidget->getFont();
    return QVariant();
}

