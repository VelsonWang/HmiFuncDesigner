#ifndef QABSTRACTWIDGETHOST_H
#define QABSTRACTWIDGETHOST_H

#include "qabstracthost.h"

class QAbstractWidgetHost : public QAbstractHost
{
public:
    QAbstractWidgetHost(QAbstractHost *parent = Q_NULLPTR);

public slots:
    void setGeometry(int x,int y,int width,int height);
    int x();
    int y();
    int width();
    int height();

    void setEnabled(bool enabled);
    bool enabled();

    void setToolTip(const QString &tooltip);
    QString toolTip();

    void setCursor(int cursor);
    int cursor();
};

#endif // QABSTRACTWIDGETHOST_H
