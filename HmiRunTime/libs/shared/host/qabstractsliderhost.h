#ifndef QABSTRACTSLIDERHOST_H
#define QABSTRACTSLIDERHOST_H

#include "qwidgethost.h"

class QAbstractSliderHost :public QWidgetHost
{
    Q_OBJECT
public:
    QAbstractSliderHost(QAbstractHost *parent = 0);
public slots:
    void    setValue(int value);
    int     value();

    void    setSingleStep(int singleStep);
    int     singleStep();

    void    setOrientation(int orientation);
    int     orientation();

    void    setInvertedAppearance(bool invertedAppearance);
    bool    invertedAppearance();
protected slots:
    void    valueChanged(int value);
protected:
    void initProperty();
};

#endif // QABSTRACTSLIDERHOST_H
