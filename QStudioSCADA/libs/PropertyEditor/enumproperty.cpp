#include "enumproperty.h"
#include <QtDebug>

EnumProperty::EnumProperty(const QString &name)
    : Property(name)
{
    value = 0;
    value.convert(QVariant::Int);
}

QVariant EnumProperty::data (int column, int role)
{
    if(enumerationLiterals.empty())
        return QVariant();

    int tmpIndex = value.toInt();

    if(tmpIndex < 0 || tmpIndex >= enumerationLiterals.count())
        tmpIndex = 0;

    if(column == ColumnData && Qt::DisplayRole == role)
        return enumerationLiterals.at(tmpIndex);
    else if(column == ColumnData && Qt::EditRole == role)
        return tmpIndex;
    else
        return Property::data(column, role);
}

QWidget* EnumProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    QComboBox* tmpEditor = new QComboBox(parent);
    tmpEditor->clear();
    tmpEditor->addItems(enumerationLiterals);
    tmpEditor->setCurrentIndex(value.toInt());
    setPropertyEditor(tmpEditor);

    return tmpEditor;
}

QVariant EnumProperty::getEditorData(QWidget* editor) const
{
    QComboBox* tmpEditor = qobject_cast<QComboBox*>(editor);
    if(tmpEditor)
        return tmpEditor->currentText();

    return QVariant(0);
}

void EnumProperty::setLiterals(const QStringList& literals)
{
    enumerationLiterals = literals;
}

QStringList EnumProperty::getLiterals()
{
    return enumerationLiterals;
}

void EnumProperty::setValue(const QVariant& pvalue)
{
    int tmpIndex = enumerationLiterals.indexOf(pvalue.toString());

    if(tmpIndex < 0 || tmpIndex >= enumerationLiterals.count())
        tmpIndex = 0;

    value.setValue(tmpIndex);
}

QVariant EnumProperty::getValue() const
{
    return enumerationLiterals.at(value.toInt());
}
