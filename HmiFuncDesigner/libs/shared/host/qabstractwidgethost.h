#ifndef QABSTRACTWIDGETHOST_H
#define QABSTRACTWIDGETHOST_H

#include "../sharedlibglobal.h"
#include "qabstracthost.h"
#include "../qstylesheetinfo.h"

class SHAREDLIB_EXPORT QAbstractWidgetHost : public QAbstractHost
{
    Q_OBJECT
public:
    QAbstractWidgetHost(QAbstractHost *parent = 0);

protected:
    void    initProperty();
public slots:
    void   setGeometry(int x,int y,int width,int height);
    int    x();
    int    y();
    int    width();
    int    height();

    void   setEnabled(bool enabled);
    bool   enabled();

    void   setToolTip(const QString &tooltip);
    QString toolTip();

    void   setCursor(int cursor);
    int    cursor();
};

#endif // QABSTRACTWIDGETHOST_H
