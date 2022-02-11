#ifndef QSIZEPROPERTY_H
#define QSIZEPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;

class QSizeProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QSizeProperty(QAbstractProperty *parent = 0);

    QString getValueText();
    QIcon getValueIcon();

    void setValue(const QVariant &value);

protected:
    void child_value_changed(const QVariant &old, const QVariant &now);
    void makeValue();
    void writeValue();

protected:
    QIntProperty *m_width;
    QIntProperty *m_height;

};

#endif // QSIZEPROPERTY_H

