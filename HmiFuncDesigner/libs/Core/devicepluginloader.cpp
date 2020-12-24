#include "devicepluginloader.h"
#include "../../Devices/IDevicePlugin/IDevicePlugin.h"
#include <QApplication>
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
#ifdef BUILD_DEBUG
    pluginName += "d";
#endif
    QDir pluginsDir(QApplication::applicationDirPath());
    pluginsDir.cdUp();
    pluginsDir.cd("deviceplugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
#ifdef Q_OS_WIN
        QString szFileEndWith = QString(".dll");
#endif
#ifdef Q_OS_LINUX
        QString szFileEndWith = QString(".so");
#endif
        if(fileName.indexOf(pluginName) != -1 && fileName.endsWith(szFileEndWith)) {
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
    }
    return Q_NULLPTR;
}
