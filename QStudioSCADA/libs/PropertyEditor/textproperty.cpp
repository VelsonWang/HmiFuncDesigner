#include "textproperty.h"
#include <QLineEdit>

TextProperty::TextProperty(const QString &name)
    : Property(name)
{
    value.setValue(QString(""));
    value.convert(QVariant::String);
}

QWidget *TextProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate)
{

    Q_UNUSED(options)
    Q_UNUSED(delegate)

    QLineEdit *tmpEditor = new QLineEdit(parent);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tmpEditor->setText(value.toString());

    return tmpEditor;
}

QVariant TextProperty::getEditorData(QWidget *editor) const
{
    QLineEdit *tmpEditor = qobject_cast<QLineEdit*>(editor);

    if (tmpEditor) {
        return tmpEditor->text();
    }

    return QVariant(QString("text"));
}

