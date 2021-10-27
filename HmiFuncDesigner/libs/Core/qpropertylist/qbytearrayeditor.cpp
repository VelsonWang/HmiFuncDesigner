#include "qbytearrayeditor.h"
#include "../../shared/property/qabstractproperty.h"

QByteArrayEditor::QByteArrayEditor(QAbstractProperty *pro, QUndoStack*, QWidget *parent) :
    QLineEdit(parent),
    property(pro)
{
    setText(pro->get_value().toString());
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged()));
    connect(property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QByteArrayEditor::onValueChanged()
{
    if(property) {
        property->notifyEditValue(text());
    }
}

void QByteArrayEditor::onPropertyChanged()
{
    if(property) {
        setText(property->get_value().toString());
    }
}
