#include "qbytearrayeditor.h"
#include "../../shared/property/qabstractproperty.h"

QByteArrayEditor::QByteArrayEditor(QAbstractProperty *pro, QUndoStack* , QWidget *parent):
    QLineEdit(parent),
    m_property(pro)
{
    setText(pro->get_value().toString());
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(onValueChanged()));
    connect(m_property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QByteArrayEditor::onValueChanged()
{
    m_property->notifyEditValue(text());
}

void QByteArrayEditor::onPropertyChanged()
{
    setText(m_property->get_value().toString());
}
