#ifndef QSWITCHBUTTONHOST_H
#define QSWITCHBUTTONHOST_H

#include "qframehost.h"

class QSwitchButtonHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QSwitchButtonHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

protected:
    void initProperty() override;

protected:
    void createObject() override;
};

#endif // QSWITCHBUTTONHOST_H
