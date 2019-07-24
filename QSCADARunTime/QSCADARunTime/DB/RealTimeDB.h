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
    explicit RealTimeDB(QObject *parent = nullptr);
    ~RealTimeDB();


signals:

public slots:

public:
    //       map<id, DBTagObject*>
    static QMap<QString, DBTagObject* > rtdb;
    static QMap<QString, QString> varNameMapId;
    static QVariant GetData(const QString &id);
    static QString GetDataString(const QString &id);
    static void SetData(const QString &id, QVariant dat);
    static void SetTypeAndData(const QString &id, TTagDataType type, QVariant dat);
    static void SetDataString(const QString &id, const QString &dat);
    static void SetDataStringInner(const QString &id, const QString &dat);
    static QString getIdByTagName(const QString &name);
    static void debug();
    static void debugShowNameMapId();

};

#endif // REALTIMEDB_H
