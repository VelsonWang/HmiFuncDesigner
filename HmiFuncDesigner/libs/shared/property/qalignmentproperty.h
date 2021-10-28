#ifndef QALIGNMENTPROPERTY_H
#define QALIGNMENTPROPERTY_H

#include "qabstractproperty.h"

class QEnumProperty;

class QAlignmentProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QAlignmentProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();

    void set_value(const QVariant &value);

protected:
    void child_value_changed(const QVariant &old, const QVariant &now);
    void makeValue();
    void writeValue();
	
protected:
    QEnumProperty *m_horizonta;
    QEnumProperty *m_vertical;
};

#endif // QALIGNMENTPROPERTY_H

