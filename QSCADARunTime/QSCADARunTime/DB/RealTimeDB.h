#ifndef REALTIMEDB_H
#define REALTIMEDB_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QTimer>
#include "DBTagObject.h"





class RealTimeDB : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeDB(QObject *parent = 0);
    ~RealTimeDB();


signals:

public slots:

public:
    //       map<id, DBTagObject*>
    static QMap<qint32, DBTagObject* > rtdb;
    static QMap<QString, qint32> varNameMapId;
    static QVariant GetData(qint32 id);
    static QString GetDataString(qint32 id);
    static void SetData(qint32 id, QVariant dat);
    static void SetTypeAndData(qint32 id, TTagDataType type, QVariant dat);
    static void SetDataString(qint32 id, QString dat);
    static void SetDataStringInner(qint32 id, QString dat);
    static int getIdByTagName(const QString name);
    static void debug();
    static void debugShowNameMapId();

};

#endif // REALTIMEDB_H
