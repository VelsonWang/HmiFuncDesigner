#ifndef QABSTRACTEDITORPLUGIN_H
#define QABSTRACTEDITORPLUGIN_H

#include "corelibglobal.h"

#include "../shared/qabstractplugin.h"

#include <QStringList>

#define RESOURCE_EDIOTR_PLUGIN_TYPE  "Plugin.Resource.Editor"

class QAbstractResourceEditor;

class CORELIB_EXPORT QAbstractEditorPlugin : public QAbstractPlugin
{
    Q_OBJECT
public:
    virtual void initPlugin(); // 初始化插件
    virtual QString getPluginType();

    virtual QAbstractResourceEditor* create(const QString &name) = 0;
    virtual QStringList supports() = 0;
};

#endif // QABSTRACTEDITORPLUGIN_H
