#ifndef REALTIMEDB_H
#define REALTIMEDB_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QTimer>
#include <QMutex>
#include "DBTagObject.h"
#include "ShareMemory.h"




class RealTimeDB : public QObject
{
    Q_OBJECT
public:
    static RealTimeDB *instance() {
        static RealTimeDB rtdb;
        return &rtdb;
    }

private:
    explicit RealTimeDB(QObject *parent = nullptr);
    ~RealTimeDB();

signals:

public slots:

public:
    bool m_bMemStatus = false;
    //    map<id, DBTagObject*>
    QMap<QString, DBTagObject* > rtdb;
    QMap<QString, QString> varNameMapId;
    TAny GetData(const QString &id);
    QString GetDataString(const QString &id);
    void SetData(const QString &id, TAny dat);
    void SetTypeAndData(const QString &id, TTagDataType type, TAny dat);
    void SetDataString(const QString &id, const QString &dat);
    void SetDataStringInner(const QString &id, const QString &dat);
    QString getIdByTagName(const QString &name);
    void debug();
    void debugShowNameMapId();
    // 从共享内存取一个未使用的DBTagObject对象
    PDBTagObject getEmptyDBTagObject();
    // 添加需要写入PLC或仪器的变量至相应设备节点写队列
    void addNeedWriteTagToDeviceWriteQueue();

private:
    // 从共享内查找指定ID的DBTagObject对象
    PDBTagObject getDBTagObject(const QString &szID);
    QMutex m_mutex;

private:
    Sharememory m_rtdbSharememory;
    PDBTagObject m_pDBTagObjectBaseAddr = Q_NULLPTR;
};

#endif // REALTIMEDB_H
