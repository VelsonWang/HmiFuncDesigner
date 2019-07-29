#include "pluginmanager.h"
#include <QApplication>
#include <QPluginLoader>
#include <QDir>

QMap<QString,QMap<QString, IPlugin*> > PluginManager::m_plugins;

PluginManager::PluginManager()
{
}

void PluginManager::LoadPlugin(const QString &pluginDir)
{
    ReleasePlugin();

    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cdUp();
    pluginsDir.cd(QLatin1String("plugins"));
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *pluginObj = pluginLoader.instance();
        if (pluginObj)
        {
            IPlugin *plugin= qobject_cast<IPlugin *>(pluginObj);
            if (plugin)
            {
                QString type = plugin->GetPluginType();
                QString name = plugin->GetPluginName();
                if(type != QLatin1String("") && name != QLatin1String(""))
                {
                    QMap<QString, IPlugin*> plugins=m_plugins.value(type);
                    IPlugin *p=plugins.value(name);
                    if(p != NULL)
                    {
                        delete p;
                    }
                    plugins.insert(name, plugin);
                    m_plugins.insert(type, plugins);
                    plugin->Initialize();
                }
                else
                {
                    delete plugin;
                }
            }
        }
    }
}

QMap<QString, IPlugin*> PluginManager::GetPluginByType(const QString &type)
{
    return m_plugins.value(type);
}

IPlugin* PluginManager::GetPlugin(const QString &type, const QString &name)
{
    return m_plugins.value(type).value(name);
}

void PluginManager::ReleasePlugin()
{
    QMapIterator<QString, QMap<QString, IPlugin*> > it(m_plugins);

    while(it.hasNext())
    {
        it.next();
        QMapIterator<QString, IPlugin*>  t(it.value());
        while(t.hasNext())
        {
            t.next();
            delete t.value();
        }
    }
    m_plugins.clear();
}




