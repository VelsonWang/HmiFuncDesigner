#ifndef QFORMHOST_H
#define QFORMHOST_H

#include "../sharedlibglobal.h"

#include "qabstractwidgethost.h"

#define FORM_TITLE "form"

class SHAREDLIB_EXPORT QFormHost : public QAbstractWidgetHost
{
    Q_OBJECT
public:
    QFormHost(QAbstractHost *parent = 0);

protected:
    void initProperty() override;

protected:
    void createObject() override;
    // 控件支持的功能事件
    QStringList supportFuncEvents() override;

};

#endif // QFORMHOST_H
