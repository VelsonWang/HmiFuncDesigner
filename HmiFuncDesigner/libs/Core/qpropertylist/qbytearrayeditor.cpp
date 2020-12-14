#include "qbytearrayeditor.h"

#include "../../shared/property/qabstractproperty.h"

QByteArrayEditor::QByteArrayEditor(QAbstractProperty *pro,QUndoStack* , QWidget *parent):
    QLineEdit(parent),
    m_property(pro)
{
    setText(pro->get_value().toString());
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(value_changed()));
    connect(m_property,SIGNAL(refresh()),this,SLOT(property_changed()));
}

void QByteArrayEditor::value_changed()
{
    m_property->emit_edit_value(text());
}

void QByteArrayEditor::property_changed()
{
    setText(m_property->get_value().toString());
}
