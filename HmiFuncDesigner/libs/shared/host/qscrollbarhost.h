#ifndef QSCROLLBARHOST_H
#define QSCROLLBARHOST_H

#include "qabstractsliderhost.h"

class QScrollBarHost: public QAbstractSliderHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QScrollBarHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
protected:
    void initProperty() override;

protected:
    void createObject() override;
    // 控件支持的功能事件
    QStringList supportFuncEvents() override;

};
#endif // QSCROLLBARHOST_H
