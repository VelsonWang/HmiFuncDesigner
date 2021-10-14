#include "VendorPluginManager.h"
#include "Helper.h"
#include "../Vendors/IVendorPlugin/IVendorPlugin.h"
#include <QApplication>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>


VendorPluginManager::VendorPluginManager()
{
}

VendorPluginManager::~VendorPluginManager()
{
    releasePlugin();
}

void VendorPluginManager::loadAllPlugin()
{
    releasePlugin();

    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cd(QLatin1String("Vendors"));

    foreach (QString szFileName, pluginsDir.entryList(QDir::Files)) {
#ifdef Q_OS_WIN
        if(!szFileName.endsWith(".dll"))
            continue;
#endif     
#ifdef Q_OS_LINUX
        if(!szFileName.endsWith(".so"))
            continue;
#endif
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(szFileName));
        QObject *pluginObj = pluginLoader.instance();
        if (pluginObj) {
            IVendorPlugin *plugin = qobject_cast<IVendorPlugin *>(pluginObj);
            if (plugin) {
                QFileInfo fileInfo(szFileName);
                m_plugins.insert(fileInfo.baseName(), plugin);
            }
        }
    }
}

IVendorPlugin* VendorPluginManager::loadPlugin(const QString &name)
{
    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cd(QLatin1String("Vendors"));
    QString szPluginName = name;

    foreach (QString szFileName, pluginsDir.entryList(QDir::Files)) {
#ifdef Q_OS_WIN
        if(!szFileName.endsWith(".dll"))
            continue;
#endif
#ifdef Q_OS_LINUX
        if(!szFileName.endsWith(".so"))
            continue;
#endif

#ifdef BUILD_BY_DEBUG
    #ifdef Q_OS_WIN
        szPluginName = name + "d";
    #endif
#endif
        QFileInfo fileInfo(szFileName);
        if(fileInfo.baseName().toLower() == szPluginName.toLower()) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(szFileName));
            QObject *pluginObj = pluginLoader.instance();
            if (pluginObj) {
                IVendorPlugin *plugin = qobject_cast<IVendorPlugin *>(pluginObj);
                if (plugin) {
                    return plugin;
                }
            }
        }
    }
    return Q_NULLPTR;
}


IVendorPlugin* VendorPluginManager::getPlugin(const QString &name)
{
    IVendorPlugin *pDevPlugin = Q_NULLPTR;
    if(name == "") return Q_NULLPTR;
    if(m_plugins.contains(name)) {
        pDevPlugin = m_plugins.value(name);
    }
    else {
        pDevPlugin = loadPlugin(name);
        if (pDevPlugin) {
            m_plugins.insert(name, pDevPlugin);
        }
    }
    return pDevPlugin;
}

void VendorPluginManager::releasePlugin()
{
    QMapIterator<QString, IVendorPlugin*> iter(m_plugins);
    while(iter.hasNext()) {
        iter.next();
        delete iter.value();
    }
    m_plugins.clear();
}




