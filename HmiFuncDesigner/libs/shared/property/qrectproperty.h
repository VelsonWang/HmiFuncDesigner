#ifndef QRECTPROPERTY_H
#define QRECTPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;

class QRectProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QRectProperty(QAbstractProperty *parent=0);

    QString         get_value_text();
    QIcon           get_value_icon();

    void            set_value(const QVariant &value);
    QVariant        get_value();

protected:
    void            child_value_changed(const QVariant &old, const QVariant &now);
    void            make_value();
    void            write_value();
protected:
    QIntProperty    *m_x;
    QIntProperty    *m_y;
    QIntProperty    *m_width;
    QIntProperty    *m_height;
};

#endif // QRECTPROPERTY_H
