#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QMap>
class IVendorPlugin;


class VendorPluginManager
{
public:
    static VendorPluginManager *getInstance() {
        static VendorPluginManager instance;
        return &instance;
    }

private:
    VendorPluginManager();
    ~VendorPluginManager();

public:
    void loadAllPlugin();
    IVendorPlugin* loadPlugin(const QString &name);
    IVendorPlugin* getPlugin(const QString &name);
    void releasePlugin();

public:
    //------name-------plugin
    QMap<QString, IVendorPlugin*> m_plugins;

};

#endif // PLUGINMANAGER_H
