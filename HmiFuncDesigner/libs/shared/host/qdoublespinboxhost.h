#ifndef QDOUBLESPINBOXHOST_H
#define QDOUBLESPINBOXHOST_H

#include "qabstractspinboxhost.h"

class QDoubleSpinBoxHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QDoubleSpinBoxHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setValue(float value);
    float   value();

    void    setSingleStep(float singleStep);
    float   singleStep();

    void    setDecimals(int decimals);
    int     decimals();
protected slots:
    void    valueChanged(const QString& value);
protected:
    void init_property();
protected:
    void    create_object();
};
#endif // QDOUBLESPINBOXHOST_H
