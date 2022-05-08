#ifndef QVALUESTICKHOST_H
#define QVALUESTICKHOST_H

#include "qframehost.h"

class QValueStickHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QValueStickHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

protected:
    void initProperty() override;

protected:
    void createObject() override;

};

#endif // QVALUESTICKHOST_H
