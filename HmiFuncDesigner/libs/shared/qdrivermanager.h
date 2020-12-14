#ifndef QDRIVERMANAGER_H
#define QDRIVERMANAGER_H

#include "sharedlibglobal.h"

#include <QObject>
#include <QMap>

class QAbstractDriver;

class SHAREDLIB_EXPORT QDriverManager : public QObject
{
    Q_OBJECT
public:
    explicit QDriverManager(QObject *parent = 0);
    ~QDriverManager();

    void        load(const QString &path);
    void        save(const QString &path);
    void        clear();

    QList<QAbstractDriver*>     get_drivers();
    QAbstractDriver*            get_driver(const QString &uuid);

    void                        insert_driver(int index,QAbstractDriver* driver);
    void                        remove_driver(const QString &uuid);
signals:
    void    insert_driver_signal(QAbstractDriver* driver,int index);
    void    remove_driver_signal(QAbstractDriver* driver);
public slots:
protected:
    QList<QAbstractDriver*>         m_drivers;
    QMap<QString,QAbstractDriver*>  m_uuid_to_driver;
};

#endif // QDRIVERMANAGER_H
