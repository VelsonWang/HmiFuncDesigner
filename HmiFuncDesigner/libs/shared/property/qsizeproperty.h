#ifndef QSIZEPROPERTY_H
#define QSIZEPROPERTY_H

#include "qabstractproperty.h"

class QIntProperty;

class QSizeProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QSizeProperty(QAbstractProperty *parent=0);

    QString         get_value_text();
    QIcon           get_value_icon();

    void            set_value(const QVariant &value);

protected:
    void            child_value_changed(const QVariant &old, const QVariant &now);
    void            make_value();
    void            write_value();
protected:
    QIntProperty    *m_width;
    QIntProperty    *m_height;
};
#endif // QSIZEPROPERTY_H
