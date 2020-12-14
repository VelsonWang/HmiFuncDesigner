#include "qfloateditor.h"

#include "../../shared/property/qabstractproperty.h"

QFloatEditor::QFloatEditor(QAbstractProperty *property,QUndoStack* , QWidget *parent):
    QDoubleSpinBox(parent),
    m_property(property)
{
    setRange(-9999999,9999999);
    setValue(property->get_value().toDouble());
    connect(this,SIGNAL(valueChanged(double)),this,SLOT(value_changed(double)));
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(m_property,SIGNAL(refresh()),this,SLOT(property_changed()));
}

void QFloatEditor::value_changed(double value)
{
    m_property->emit_edit_value(value);
}

void QFloatEditor::property_changed()
{
    setValue(m_property->get_value().toDouble());
}
