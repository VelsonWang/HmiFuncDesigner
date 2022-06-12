#ifndef QDIALHOST_H
#define QDIALHOST_H

#include "qabstractsliderhost.h"

class QDialHost: public QAbstractSliderHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QDialHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

public slots:
    void setNotchTarget(float notchTarget);
    float notchTarget();

    void setWrapping(bool wrapping);
    bool wrapping();

    void setNotchesVisible(bool notchesVisible);
    bool notchesVisible();

protected:
    void initProperty() override;

protected:
    void createObject() override;

};

#endif // QDIALHOST_H
