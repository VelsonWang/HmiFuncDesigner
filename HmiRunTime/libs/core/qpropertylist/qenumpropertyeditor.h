#ifndef QENUMPROPERTYEDITOR_H
#define QENUMPROPERTYEDITOR_H

#include "../../shared/qcommonstruct.h"

#include <QComboBox>
#include <QUndoStack>

class QAbstractProperty;

class QEnumPropertyEditor : public QComboBox
{
    Q_OBJECT
public:
    Q_INVOKABLE QEnumPropertyEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent = 0);
    
signals:
    
protected slots:
    void onValueChanged(int index);
    void onPropertyChanged();

protected:
    QAbstractProperty* m_property;
    ComboItems m_items;
};

#endif // QENUMPROPERTYEDITOR_H
