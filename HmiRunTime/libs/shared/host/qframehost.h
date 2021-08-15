#ifndef QFRAMEHOST_H
#define QFRAMEHOST_H

#include "qwidgethost.h"

class QFrameHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QFrameHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
public slots:
    void    setFrameShape(int frameShape);
    int     frameShape();

    void    setFrameShadow(int frameShadow);
    int     frameShadow();
protected:
    void    initProperty();
protected:
    void    createObject();
};

#endif // QFRAMEHOST_H
