#ifndef QTANKHOST_H
#define QTANKHOST_H

#include "qframehost.h"

class QTankHost : public QFrameHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QTankHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

protected:
    void initProperty() override;

protected:
    void createObject() override;

};

#endif
