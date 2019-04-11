#include "boolproperty.h"

BoolProperty::BoolProperty(const QString &name)
    : Property(name)
{
    value.setValue(false);
    value.convert(QVariant::Bool);

    if(trueText.isNull()) trueText = QObject::tr("True");
    if(trueText.isNull()) falseText = QObject::tr("False");
}

QVariant BoolProperty::data (int column, int role)
{
    if(column == ColumnData && (Qt::DisplayRole == role || Qt::EditRole == role))
        return value.toBool() ? trueText : falseText;
    if(column == ColumnData && Qt::CheckStateRole == role)
        return value.toBool() ? Qt::Checked : Qt::Unchecked;
    else
        return Property::data(column, role);
}

bool BoolProperty::setData(const QVariant &data, int role)
{
    if(Qt::CheckStateRole == role)
    {
        value = (Qt::Checked == static_cast<Qt::CheckState>(data.toInt())); return true;
    }

    return false;
}


Qt::ItemFlags BoolProperty::flags(int column)
{
    if(column == ColumnData)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return Property::flags(column);
}

void BoolProperty::setTrueText(const QString &text) {
    trueText = text;
}

void BoolProperty::setFalseText(const QString &text) {
    falseText = text;
}
