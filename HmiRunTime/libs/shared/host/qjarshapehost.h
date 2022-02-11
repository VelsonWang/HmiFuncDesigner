#ifndef QJARSHAPEHOST_H
#define QJARSHAPEHOST_H

#include "qframehost.h"

class QJarShapeHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QJarShapeHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

protected:
    void initProperty();

protected:
    void createObject();

};

#endif // QJARSHAPEHOST_H
