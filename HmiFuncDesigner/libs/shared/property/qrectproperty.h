#ifndef QRECTPROPERTY_H
#define QRECTPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;

class QRectProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QRectProperty(QAbstractProperty *parent = 0);

    QString getValueText();
    QIcon getValueIcon();

    void setValue(const QVariant &value);
    QVariant getValue();

protected:
    void child_value_changed(const QVariant &old, const QVariant &now);
    void makeValue();
    void writeValue();

protected:
    QIntProperty *m_x;
    QIntProperty *m_y;
    QIntProperty *m_width;
    QIntProperty *m_height;
};

#endif // QRECTPROPERTY_H

