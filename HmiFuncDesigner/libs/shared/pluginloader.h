#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "sharedlibglobal.h"

#include <QMap>

class QAbstractPlugin;

class SHAREDLIB_EXPORT PluginLoader
{
public:
    //页面插件操作
    static void                            load_plugin(const QString &xml_file_name);
    static QAbstractPlugin*                get_plugin(const QString &type,const QString &name);
    static QMap<QString,QAbstractPlugin*>  get_plugin_by_type(const QString &type);
    static void                            release_plugin();
protected:
    static QMap<QString,QMap<QString,QAbstractPlugin*> >        m_plugins;
};

#endif // QPLUGINLOADER_H
