#ifndef QNUMBEREDITOR_H
#define QNUMBEREDITOR_H

#include <QSpinBox>
#include <QUndoStack>

class QAbstractProperty;

class QNumberEditor : public QSpinBox
{
    Q_OBJECT
public:
    Q_INVOKABLE QNumberEditor(QAbstractProperty* property,QUndoStack* stack,QWidget *parent = 0);
    
signals:
    
protected slots:
    void    value_changed(int value);
    void    property_changed();
protected:
    QAbstractProperty   *m_property;
};

#endif // QNUMBEREDITOR_H
