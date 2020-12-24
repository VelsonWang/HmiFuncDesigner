#ifndef DEVICEPLUGINLOADER_H
#define DEVICEPLUGINLOADER_H

#include "corelibglobal.h"
#include <QString>
#include <QObject>

class IDevicePlugin;


class CORELIB_EXPORT DevicePluginLoader : public QObject
{
    Q_OBJECT
public:
    static DevicePluginLoader *getInstance() {
        static DevicePluginLoader instance;
        return &instance;
    }

    IDevicePlugin *getPluginObject(const QString &szPluginName);

private:
    DevicePluginLoader(QObject* parent = Q_NULLPTR);
    ~DevicePluginLoader();

};

#endif // DEVICEPLUGINLOADER_H
