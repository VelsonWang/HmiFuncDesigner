#ifndef QWIDGETHOST_H
#define QWIDGETHOST_H

#include "qabstractwidgethost.h"

class QWidgetHost : public QAbstractWidgetHost
{
public:
    Q_INVOKABLE QWidgetHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
public slots:
    void setVisible(bool visible);
    bool visible();

    void setFocus();
    void killFocus();

protected:
    void createObject();

};

#endif // QWIDGETHOST_H
