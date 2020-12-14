#include "qnumbereditor.h"

#include "../../shared/property/qabstractproperty.h"

QNumberEditor::QNumberEditor(QAbstractProperty *property,QUndoStack* , QWidget *parent):
    QSpinBox(parent),
    m_property(property)
{
    setRange(-9999,9999);
    setValue(property->get_value().toInt());
    connect(this,SIGNAL(valueChanged(int)),this,SLOT(value_changed(int)));
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(m_property,SIGNAL(refresh()),this,SLOT(property_changed()));
}

void QNumberEditor::value_changed(int value)
{
    m_property->emit_edit_value(value);
}

void QNumberEditor::property_changed()
{
    setValue(m_property->get_value().toInt());
}
