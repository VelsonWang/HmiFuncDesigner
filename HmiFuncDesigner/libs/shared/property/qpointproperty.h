#ifndef QPOINTPROPERTY_H
#define QPOINTPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;

class QPointProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QPointProperty(QAbstractProperty *parent = 0);

    QString get_value_text();
    QIcon get_value_icon();

    void set_value(const QVariant &value);

protected:
    void child_value_changed(const QVariant &old, const QVariant &now);
    void make_value();
    void write_value();
	
protected:
    QIntProperty *m_x;
    QIntProperty *m_y;
};

#endif // QPOINTPROPERTY_H

