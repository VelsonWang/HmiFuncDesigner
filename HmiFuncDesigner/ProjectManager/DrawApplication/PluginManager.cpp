#include "PluginManager.h"
#include "Helper.h"
#include <QApplication>
#include <QPluginLoader>
#include <QDir>
#include <QDebug>

QMutex PluginManager::mutex_;
PluginManager *PluginManager::instance_ = nullptr;

PluginManager::PluginManager()
{
}

void PluginManager::loadPlugin()
{
    releasePlugin();

    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cdUp();
    pluginsDir.cd(QLatin1String("DrawApplicationPlugins"));

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
#ifdef Q_OS_WIN
        if(!fileName.endsWith(".dll"))
            continue;
#endif

#ifdef Q_OS_LINUX
        if(!fileName.endsWith(".so"))
            continue;
#endif

        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *pluginObj = pluginLoader.instance();
        if (pluginObj)
        {
            IDrawApplicationPlugin *plugin = qobject_cast<IDrawApplicationPlugin *>(pluginObj);
            if (plugin)
            {
                QString type = plugin->getPluginTypeName();
                QString name = plugin->getPluginName();
                if(type != QLatin1String("") && name != QLatin1String(""))
                {
                    QMap<QString, IDrawApplicationPlugin*> plugins = plugins_.value(type);
                    IDrawApplicationPlugin *p = plugins.value(name);
                    if(p != NULL)
                    {
                        delete p;
                    }
                    plugins.insert(name, plugin);
                    plugins_.insert(type, plugins);
                    plugin->initialize();
                }
                else
                {
                    delete plugin;
                }
            }
        }
    }
}

QMap<QString, IDrawApplicationPlugin*> PluginManager::getPluginByType(const QString &type)
{
    return plugins_.value(type);
}

IDrawApplicationPlugin* PluginManager::getPlugin(const QString &type, const QString &name)
{
    return plugins_.value(type).value(name);
}

void PluginManager::releasePlugin()
{
    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > it(plugins_);

    while(it.hasNext())
    {
        it.next();
        QMapIterator<QString, IDrawApplicationPlugin*>  t(it.value());
        while(t.hasNext())
        {
            t.next();
            delete t.value();
        }
    }
    plugins_.clear();
}




