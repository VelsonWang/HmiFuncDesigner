#ifndef QALIGNMENTPROPERTY_H
#define QALIGNMENTPROPERTY_H

#include "qabstractproperty.h"

class QEnumProperty;

class QAlignmentProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QAlignmentProperty(QAbstractProperty* parent = 0);

    QString get_value_text();
    QIcon get_value_icon();

    void set_value(const QVariant &value);

protected:
    void child_value_changed(const QVariant &old, const QVariant &now);
    void make_value();
    void write_value();
	
protected:
    QEnumProperty *m_horizonta;
    QEnumProperty *m_vertical;
};

#endif // QALIGNMENTPROPERTY_H

