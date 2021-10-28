#ifndef QPOINTPROPERTY_H
#define QPOINTPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;

class QPointProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QPointProperty(QAbstractProperty *parent = 0);

    QString getValueText();
    QIcon getValueIcon();

    void set_value(const QVariant &value);

protected:
    void child_value_changed(const QVariant &old, const QVariant &now);
    void makeValue();
    void writeValue();

protected:
    QIntProperty *m_x;
    QIntProperty *m_y;
};

#endif // QPOINTPROPERTY_H

