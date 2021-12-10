#include "qnumbereditor.h"
#include "../../shared/property/qabstractproperty.h"

QNumberEditor::QNumberEditor(QAbstractProperty *property, QUndoStack*, QWidget *parent) :
    QSpinBox(parent),
    m_property(property)
{
    setRange(-9999, 9999);
    if(property) {
        setValue(property->getValue().toInt());
    }
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QNumberEditor::onValueChanged(int value)
{
    if(m_property) {
        m_property->notifyEditValue(value);
    }
}

void QNumberEditor::onPropertyChanged()
{
    if(m_property) {
        setValue(m_property->getValue().toInt());
    }
}
