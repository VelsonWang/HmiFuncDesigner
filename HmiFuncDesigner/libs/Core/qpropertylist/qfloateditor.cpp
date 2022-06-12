#include "qfloateditor.h"
#include "../../shared/property/qabstractproperty.h"

QFloatEditor::QFloatEditor(QAbstractProperty *property, QUndoStack*, QWidget *parent) :
    QDoubleSpinBox(parent),
    m_property(property)
{
    setRange(-9999999, 9999999);
    if(property) {
        setValue(property->getValue().toDouble());
    }
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QFloatEditor::onValueChanged(double value)
{
    if(m_property) {
        m_property->notifyEditValue(value);
    }
}

void QFloatEditor::onPropertyChanged()
{
    if(m_property) {
        setValue(m_property->getValue().toDouble());
    }
}
