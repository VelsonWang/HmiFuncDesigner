#ifndef QABSTRACTUPDATE_H
#define QABSTRACTUPDATE_H

#include "corelibglobal.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QStringList>

class CORELIB_EXPORT QAbstractUpdate : public QObject
{
    Q_OBJECT
public:
    QAbstractUpdate(QObject* parent=0);
    ~QAbstractUpdate();
    QList<QObject*> devices();
    QObject*    device(const QString &name);

    virtual void    addFiles(const QStringList &files,const QString& basePath)=0;
    virtual void    clearFiles()=0;
    virtual void    update(const QString &name)=0;
    virtual QString name()=0;
    void    clear();
protected:
    void    addDevice(QObject* device);
    void    delDevice(QObject* device);
signals:
    void    newDevice(QObject* device);
    void    upateFailed();
    void    updateSuccess();
    void    sendIndex(int index);
public slots:
protected:
    QList<QObject*>     m_devices;
    QMap<QString,QObject*>      m_nameToDevice;
    QList<QObject*>     m_sendDevices;
};

#endif // QABSTRACTUPDATE_H
