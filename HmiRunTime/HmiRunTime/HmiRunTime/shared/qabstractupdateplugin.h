#ifndef QABSTRACTUPDATEPLUGIN_H
#define QABSTRACTUPDATEPLUGIN_H

#include "qabstractplugin.h"

#include <QStringList>

#define UPDATE_PLUGIN_TYPE  "Plugin.Update"

class QAbstractUpdate;

class SHAREDLIB_EXPORT QAbstractUpdatePlugin : public QAbstractPlugin
{
    Q_OBJECT
public:
    virtual void initPlugin(); // 初始化插件
    virtual QString getPluginType();

    virtual QAbstractUpdate* create(const QString &name);
    virtual QStringList supports();
};

#endif // QABSTRACTUPDATEPLUGIN_H
