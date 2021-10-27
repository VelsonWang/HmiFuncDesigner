#include "qnumbereditor.h"
#include "../../shared/property/qabstractproperty.h"

QNumberEditor::QNumberEditor(QAbstractProperty *property, QUndoStack*, QWidget *parent) :
    QSpinBox(parent),
    property(property)
{
    setRange(-9999, 9999);
    if(property) {
        setValue(property->get_value().toInt());
    }
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QNumberEditor::onValueChanged(int value)
{
    if(property) {
        property->notifyEditValue(value);
    }
}

void QNumberEditor::onPropertyChanged()
{
    if(property) {
        setValue(property->get_value().toInt());
    }
}
