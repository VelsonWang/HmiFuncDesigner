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
    void initProperty() override;

protected:
    void createObject() override;
    // 控件支持的功能事件
    QStringList supportFuncEvents() override;
};

#endif // QJARSHAPEHOST_H
