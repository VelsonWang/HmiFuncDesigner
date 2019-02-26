#include "doubleproperty.h"
#include <QDoubleSpinBox>

const double DoubleProperty::standardPrecision = 5;

DoubleProperty::DoubleProperty(const QString &name)
    : Property(name), precision(standardPrecision)
{
    value.setValue(0);
    value.convert(QVariant::Double);
}

QWidget* DoubleProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);
    QDoubleSpinBox* tmpEditor = new QDoubleSpinBox(parent);
    tmpEditor->setMinimum(min);
    tmpEditor->setMaximum(max);
    tmpEditor->setDecimals(precision);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setValue(value.toDouble());

    return tmpEditor;
}

QVariant DoubleProperty::getEditorData(QWidget* editor) const
{
    QDoubleSpinBox* tmpEditor = qobject_cast<QDoubleSpinBox*>(editor);
    if(tmpEditor)
        return tmpEditor->value();

    return QVariant(0);
}

void DoubleProperty::setSettings(double minimum, double maxiumum, int precision)
{
    min = minimum;
    max = maxiumum;
    this->precision = precision;
}


