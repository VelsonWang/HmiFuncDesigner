#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "pluginmanager_global.h"
#include "iplugin.h"

class PLUGINMANAGERSHARED_EXPORT PluginManager
{

public:
    PluginManager();

public:
    static void LoadPlugin(const QString &pluginDir);
    static IPlugin* GetPlugin(const QString &type,const QString &name);
    static QMap<QString, IPlugin*> GetPluginByType(const QString &type);
    static void ReleasePlugin();

private:
    //------------type---------name-------plugin
    static QMap<QString, QMap<QString, IPlugin*> > m_plugins;
};

#endif // PLUGINMANAGER_H
