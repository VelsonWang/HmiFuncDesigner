#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QMutex>
#include <QMutexLocker>
#include "IDrawApplicationPlugin.h"

class PluginManager
{
public:
    static PluginManager *getInstance() {
        QMutexLocker locker(&mutex_);
        if(instance_ == nullptr) {
            instance_ = new PluginManager();
        }
        return instance_;
    }

private:
    PluginManager();

public:
    void loadPlugin();
    IDrawApplicationPlugin* getPlugin(const QString &type,const QString &name);
    QMap<QString, IDrawApplicationPlugin*> getPluginByType(const QString &type);
    void releasePlugin();

public:
    //-------type---------name-------plugin
    QMap<QString, QMap<QString, IDrawApplicationPlugin*> > plugins_;

private:
    static QMutex mutex_;
    static PluginManager *instance_;
};

#endif // PLUGINMANAGER_H
