#include "qfloateditor.h"
#include "../../shared/property/qabstractproperty.h"

QFloatEditor::QFloatEditor(QAbstractProperty *property, QUndoStack* , QWidget *parent):
    QDoubleSpinBox(parent),
    m_property(property)
{
    setRange(-9999999, 9999999);
    setValue(property->get_value().toDouble());
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(m_property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QFloatEditor::onValueChanged(double value)
{
    m_property->notifyEditValue(value);
}

void QFloatEditor::onPropertyChanged()
{
    setValue(m_property->get_value().toDouble());
}
