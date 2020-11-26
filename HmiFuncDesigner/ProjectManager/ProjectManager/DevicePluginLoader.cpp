#include "DevicePluginLoader.h"
#include "IDevicePlugin.h"
#include "Helper.h"
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QPluginLoader>

DevicePluginLoader::DevicePluginLoader(QObject* parent)
    : QObject(parent)
{

}

DevicePluginLoader::~DevicePluginLoader()
{

}

///
/// \brief DevicePluginLoader::getPluginObject
/// \param szPluginName 插件名称
/// \return 插件对象
///
IDevicePlugin * DevicePluginLoader::getPluginObject(const QString &szPluginName)
{
    QString pluginName = szPluginName;
    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cdUp();
    pluginsDir.cd("deviceplugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if(fileName.indexOf(pluginName) == -1) continue;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            IDevicePlugin *pDevPluginObj = qobject_cast<IDevicePlugin *>(plugin);
            if (pDevPluginObj == Q_NULLPTR) {
                QMessageBox::critical(Q_NULLPTR, QObject::tr("系统错误"), QObject::tr("插件加载失败！\n") + fileName);
            }
            return pDevPluginObj;
        }
    }
    return Q_NULLPTR;
}
