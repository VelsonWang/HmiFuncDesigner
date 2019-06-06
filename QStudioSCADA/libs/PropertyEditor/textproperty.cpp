#include "textproperty.h"
#include <QLineEdit>

TextProperty::TextProperty(const QString &name)
    : Property(name),
      text(QString("")),
      readOnly_(false)
{
    value.setValue(QString(""));
    value.convert(QVariant::String);
}

QWidget *TextProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate)
{
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    if(readOnly_) {
        return nullptr;
    } else {
        QLineEdit *tmpEditor = new QLineEdit(parent);
        tmpEditor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        tmpEditor->setText(value.toString());
        tmpEditor->setReadOnly(readOnly_);
        setPropertyEditor(tmpEditor);
        return tmpEditor;
    }
}

QVariant TextProperty::getEditorData(QWidget *editor) const
{
    if(readOnly_) {
        return QVariant();
    } else {
        QLineEdit *tmpEditor = qobject_cast<QLineEdit*>(editor);
        if (tmpEditor) {
            return tmpEditor->text();
        }
        return QVariant(QString("text"));
    }
}

/**
 * @brief TextProperty::setReadOnly
 * @details 设置属性为只读
 * @param readOnly是否为只读
 */
void TextProperty::setReadOnly(bool readOnly) {
    readOnly_ = readOnly;
}


QVariant TextProperty::data (int column, int role) {
    if(column == ColumnData && (Qt::DisplayRole == role || Qt::EditRole == role))
        return value;
    return Property::data(column, role);
}


Qt::ItemFlags TextProperty::flags(int column) {
    if(column == ColumnData) {
        if(readOnly_) {
            return Qt::ItemIsSelectable;
        } else {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        }
    } else {
        return Property::flags(column);
    }
}

