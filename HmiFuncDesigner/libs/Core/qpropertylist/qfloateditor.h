#ifndef QFLOATEDITOR_H
#define QFLOATEDITOR_H

#include <QDoubleSpinBox>
#include <QUndoStack>

class QAbstractProperty;

class QFloatEditor : public QDoubleSpinBox
{
    Q_OBJECT
public:
    Q_INVOKABLE QFloatEditor(QAbstractProperty* property, QUndoStack* stack, QWidget *parent = 0);

signals:

protected slots:
    void onValueChanged(double value);
    void onPropertyChanged();

protected:
    QAbstractProperty *m_property;
};

#endif // QFLOATEDITOR_H
