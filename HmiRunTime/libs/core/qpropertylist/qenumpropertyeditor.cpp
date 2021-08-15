#include "qenumpropertyeditor.h"
#include "../../shared/property/qabstractproperty.h"

QEnumPropertyEditor::QEnumPropertyEditor(QAbstractProperty *property, QUndoStack* , QWidget *parent):
    QComboBox(parent),
    m_property(property)
{
    m_items = m_property->getAttribute("items").value<ComboItems>();
    foreach(tagComboItem item, m_items) {
        this->addItem(QIcon(item.m_icon), item.m_text);
        if(property->get_value() == item.m_value) {
            setCurrentIndex(m_items.indexOf(item));
        }
    }
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onValueChanged(int)));
    connect(m_property, SIGNAL(refresh()), this, SLOT(onPropertyChanged()));
}

void QEnumPropertyEditor::onValueChanged(int index)
{
    tagComboItem item = m_items.value(index);
    if(item.m_value != m_property->get_value()) {
        m_property->notifyEditValue(item.m_value);
    }
}

void QEnumPropertyEditor::onPropertyChanged()
{
    foreach(tagComboItem item, m_items) {
        if(m_property->get_value() == item.m_value) {
            setCurrentIndex(m_items.indexOf(item));
            return;
        }
    }
}
