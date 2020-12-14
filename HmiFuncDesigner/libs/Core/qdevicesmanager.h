#ifndef QDEVICESMANAGER_H
#define QDEVICESMANAGER_H

#include "corelibglobal.h"

#include <QObject>
#include <QMap>

class QAbstractUpdate;

class CORELIB_EXPORT QDevicesManager : public QObject
{
    Q_OBJECT
public:
    explicit QDevicesManager(QObject *parent = 0);

    ~QDevicesManager();

    void    clear();

    void    addUpdate(QAbstractUpdate *update);
    QList<QAbstractUpdate*> updates();
signals:

protected slots:
    void    newDevice(QObject* obj);
    void    removeDevice(QObject* obj);
protected:
    QList<QObject*>     m_devices;
    QMap<QString,QObject*>  m_nameToDevice;
    QList<QAbstractUpdate*> m_updates;
};

#endif // QDEVICESMANAGER_H
