#include "ViewElementPluginManager.h"
#include "Helper.h"
#include "../IHmiRunTimeViewPlugin/IHmiRunTimeViewPlugin.h"
#include <QApplication>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>


ViewElementPluginManager::ViewElementPluginManager()
{
}

ViewElementPluginManager::~ViewElementPluginManager()
{
    releasePlugin();
}

void ViewElementPluginManager::loadAllPlugin()
{
    releasePlugin();

    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cd(QLatin1String("ViewElements"));

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
            IHmiRunTimeViewPlugin *plugin = qobject_cast<IHmiRunTimeViewPlugin *>(pluginObj);
            if (plugin) {
                QFileInfo fileInfo(szFileName);
                m_plugins.insert(fileInfo.baseName(), plugin);
            }
        }
    }
}

IHmiRunTimeViewPlugin* ViewElementPluginManager::loadPlugin(const QString &name)
{
    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cd(QLatin1String("ViewElements"));
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
                IHmiRunTimeViewPlugin *plugin = qobject_cast<IHmiRunTimeViewPlugin *>(pluginObj);
                if (plugin) {
                    return plugin;
                }
            }
        }
    }
    return Q_NULLPTR;
}


IHmiRunTimeViewPlugin* ViewElementPluginManager::getPlugin(const QString &name)
{
    IHmiRunTimeViewPlugin *pDevPlugin = Q_NULLPTR;
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

void ViewElementPluginManager::releasePlugin()
{
    QMapIterator<QString, IHmiRunTimeViewPlugin*> iter(m_plugins);
    while(iter.hasNext()) {
        iter.next();
        delete iter.value();
    }
    m_plugins.clear();
}


///
/// \brief ViewElementPluginManager::getPluginByElementName
/// \details 获取匹配元素名称的插件对象
/// \param szName 元素名称
/// \return 插件对象
///
IHmiRunTimeViewPlugin* ViewElementPluginManager::getPluginByElementName(const QString &szName)
{
    if(szName.trimmed().isEmpty()) return Q_NULLPTR;

    QMapIterator<QString, IHmiRunTimeViewPlugin*> iter(m_plugins);
    while(iter.hasNext()) {
        iter.next();
        IHmiRunTimeViewPlugin* pPluginObj = iter.value();
        if(pPluginObj != Q_NULLPTR) {
            if(pPluginObj->getElementName() == szName) return pPluginObj;
        }
    }

    return Q_NULLPTR;
}


///
/// \brief ViewElementPluginManager::getPluginByElementID
/// \details 获取匹配元素ID的插件对象
/// \param iID 元素ID
/// \return 插件对象
///
IHmiRunTimeViewPlugin* ViewElementPluginManager::getPluginByElementID(int &iID)
{
    if(iID < 2) return Q_NULLPTR;

    QMapIterator<QString, IHmiRunTimeViewPlugin*> iter(m_plugins);
    while(iter.hasNext()) {
        iter.next();
        IHmiRunTimeViewPlugin* pPluginObj = iter.value();
        if(pPluginObj != Q_NULLPTR) {
            if(pPluginObj->getElementID() == iID) return pPluginObj;
        }
    }

    return Q_NULLPTR;
}

//
///
/// \brief ViewElementPluginManager::getPluginByElementIDString
/// \details 获取匹配元素ID String的插件对象
/// \param szIDString 元素ID String
/// \return 插件对象
///
IHmiRunTimeViewPlugin* ViewElementPluginManager::getPluginByElementIDString(const QString &szIDString)
{
    if(szIDString.trimmed().isEmpty()) return Q_NULLPTR;

    QMapIterator<QString, IHmiRunTimeViewPlugin*> iter(m_plugins);
    while(iter.hasNext()) {
        iter.next();
        IHmiRunTimeViewPlugin* pPluginObj = iter.value();
        if(pPluginObj != Q_NULLPTR) {
            if(pPluginObj->getElementIDString() == szIDString) return pPluginObj;
        }
    }

    return Q_NULLPTR;
}


