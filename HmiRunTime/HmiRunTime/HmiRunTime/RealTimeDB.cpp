#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMutexLocker>
#include <QHashIterator>
#include "RealTimeDB.h"
#include "Public/PublicFunction.h"
#include <QDebug>


RealTimeDB::RealTimeDB(QObject *parent) : QObject(parent)
{

}


RealTimeDB::~RealTimeDB()
{
    qDeleteAll(rtdb);
    rtdb.clear();
}

void RealTimeDB::setTagData(quint64 id, QByteArray val, bool syncToVendor)
{
    if(rtdb.count(id) > 0) {
        RunTimeTag* pObj = rtdb.value(id);
        if(pObj) {
            pObj->dataFromVendor.clear();
            pObj->dataFromVendor.append(val);
            if(syncToVendor) {
                pObj->dataToVendor.clear();
                pObj->dataToVendor.append(val);
            }
        }
    }
}

QByteArray RealTimeDB::tagData(quint64 id)
{
    if(rtdb.count(id) > 0) {
        RunTimeTag* pObj = rtdb.value(id);
        if(pObj) {
            return pObj->dataFromVendor;
        }
    }
    return QByteArray();
}

quint64 RealTimeDB::tagId(const QString &name)
{
    QHashIterator<quint64, RunTimeTag * > iter(rtdb);
    while (iter.hasNext()) {
        iter.next();
        RunTimeTag *pObj = iter.value();
        if(pObj) {
            if(pObj->name == name) {
                return iter.key();
            }
        }
    }
    return 0;
}


void RealTimeDB::debug()
{
    qDebug() << "RealTimeDB:----------------------------------------";
    QHashIterator<quint64, RunTimeTag * > iter(rtdb);
    while (iter.hasNext()) {
        iter.next();
        RunTimeTag *pObj = iter.value();
        if(pObj) {
            qDebug() << pObj->id << "=" << pObj->toString();
        }
    }
    qDebug() << "---------------------------------------------------";
}
