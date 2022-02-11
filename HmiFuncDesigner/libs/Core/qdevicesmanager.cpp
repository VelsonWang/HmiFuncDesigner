#include "qdevicesmanager.h"
#include "qabstractupdate.h"
#include <QVariant>

QDevicesManager::QDevicesManager(QObject *parent) :
    QObject(parent)
{
}

QDevicesManager::~QDevicesManager()
{
    clear();
}

void QDevicesManager::addUpdate(QAbstractUpdate *update)
{
    if(update == NULL || m_updates.contains(update)) {
        return;
    }
    connect(update, SIGNAL(newDevice(QObject*)), this, SLOT(newDevice(QObject*)));
    m_updates.append(update);
}

void QDevicesManager::clear()
{
    m_devices.clear();
    m_nameToDevice.clear();
    qDeleteAll(m_updates);
    m_updates.clear();
}

void QDevicesManager::newDevice(QObject *obj)
{
    m_devices.append(obj);
    m_nameToDevice.insert(obj->property("name").toString(), obj);
    connect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(removeDevice(QObject*)));
}

void QDevicesManager::removeDevice(QObject *obj)
{
    m_devices.removeAll(obj);
    m_nameToDevice.remove(obj->property("name").toString());
}

QList<QAbstractUpdate*> QDevicesManager::updates()
{
    return m_updates;
}
