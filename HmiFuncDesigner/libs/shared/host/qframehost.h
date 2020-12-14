#ifndef QFRAMEHOST_H
#define QFRAMEHOST_H

#include "qwidgethost.h"

class QFrameHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QFrameHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setFrameShape(int frameShape);
    int     frameShape();

    void    setFrameShadow(int frameShadow);
    int     frameShadow();
protected:
    void    init_property();
protected:
    void    create_object();
};

#endif // QFRAMEHOST_H
