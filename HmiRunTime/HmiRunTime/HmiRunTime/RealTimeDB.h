#ifndef REALTIMEDB_H
#define REALTIMEDB_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QTimer>
#include <QMutex>
#include "RunTimeTag.h"

class RealTimeDB : public QObject
{
    Q_OBJECT
public:
    static RealTimeDB *instance()
    {
        static RealTimeDB rtdb;
        return &rtdb;
    }

    void setTagData(quint64 id, QByteArray val, bool syncToVendor = false);
    QByteArray tagData(quint64 id);

    quint64 tagId(const QString &name);

private:
    explicit RealTimeDB(QObject *parent = nullptr);
    ~RealTimeDB();

signals:

public slots:

public:
    QHash<quint64, RunTimeTag * > rtdb;

private:
    QMutex m_mutex;

};

#endif // REALTIMEDB_H
