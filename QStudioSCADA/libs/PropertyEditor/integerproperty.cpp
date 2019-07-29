#include "integerproperty.h"
#include <QSpinBox>

const int IntegerProperty::standardMin = -1000000;
const int IntegerProperty::standardMax = 1000000;

IntegerProperty::IntegerProperty(const QString &name)
    : Property(name),min(standardMin),max(standardMax)
{
    value.setValue(0);
    value.convert(QVariant::Int);
}

QWidget* IntegerProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    QSpinBox* tmpEditor = new QSpinBox(parent);
    tmpEditor->setMinimum(min);
    tmpEditor->setMaximum(max);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setValue(value.toInt());
    setPropertyEditor(tmpEditor);

    return tmpEditor;
}

QVariant IntegerProperty::getEditorData(QWidget* editor) const
{
    QSpinBox* tmpEditor = qobject_cast<QSpinBox*>(editor);
    if(tmpEditor)
        return tmpEditor->value();

    return QVariant(0);
}

void IntegerProperty::setSettings(int minimum, int maxiumum)
{
    min = minimum;
    max = maxiumum;
}
