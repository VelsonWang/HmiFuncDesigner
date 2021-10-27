#ifndef QNUMBEREDITOR_H
#define QNUMBEREDITOR_H

#include <QSpinBox>
#include <QUndoStack>

class QAbstractProperty;

class QNumberEditor : public QSpinBox
{
    Q_OBJECT
public:
    Q_INVOKABLE QNumberEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);

signals:

protected slots:
    void onValueChanged(int value);
    void onPropertyChanged();

protected:
    QAbstractProperty *property;
};

#endif // QNUMBEREDITOR_H
