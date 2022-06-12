#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "sharedlibglobal.h"

#include <QMap>

class QAbstractPlugin;

class SHAREDLIB_EXPORT PluginLoader
{
public:
    //页面插件操作
    static void loadPlugin(const QString &xml_file_name);
    static QAbstractPlugin* getPlugin(const QString &type, const QString &name);
    static QMap<QString, QAbstractPlugin*> getPluginByType(const QString &type);
    static void releasePlugin();

protected:
    static QMap<QString, QMap<QString, QAbstractPlugin*> > m_plugins;
};

#endif // QPLUGINLOADER_H
