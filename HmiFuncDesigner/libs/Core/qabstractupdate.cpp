#include "qabstractupdate.h"

#include <QVariant>

QAbstractUpdate::QAbstractUpdate(QObject *parent) :
    QObject(parent)
{

}

QAbstractUpdate::~QAbstractUpdate()
{
    clear();
}

QList<QObject*> QAbstractUpdate::devices()
{
    return m_devices;
}

void QAbstractUpdate::addDevice(QObject *device)
{
    if(device == NULL || m_devices.contains(device)) {
        return;
    }
    m_devices.append(device);
    m_nameToDevice.insert(device->property("name").toString(), device);
    emit newDevice(device);
}

void QAbstractUpdate::delDevice(QObject *device)
{
    m_devices.removeAll(device);
    m_nameToDevice.remove(device->property("name").toString());
    m_sendDevices.removeAll(device);
    delete device;
}

void QAbstractUpdate::clear()
{
    m_nameToDevice.clear();
    qDeleteAll(m_devices);
    m_devices.clear();
}

QObject* QAbstractUpdate::device(const QString &name)
{
    return m_nameToDevice.value(name);
}
