﻿#include "Database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QThread>
#include <QCoreApplication>
#include "ulog.h"

static const char *g_pszConnectionPrefix = "_database_";
static DatabaseWrapper *g_pGlobalDatabaseObj = Q_NULLPTR;


DatabaseWrapper::DatabaseWrapper(QObject *parent)
    : QObject(parent),
      m_llConnectionId(0)
{
}

void DatabaseWrapper::onThreadFinished()
{
    QThread *pThreadObj = qobject_cast<QThread*>(sender());
    if (!pThreadObj)
        return;

    // 清理线程数据库连接
    QMutexLocker locker(&m_mutex);
    disconnect(pThreadObj, SIGNAL(finished()), this, SLOT(onThreadFinished()));
    const QString szConnectionName = m_mapCopies.value(pThreadObj).connectionName();
    m_mapCopies.remove(pThreadObj);
    if (szConnectionName.startsWith(QLatin1String(g_pszConnectionPrefix)))
        QSqlDatabase::removeDatabase(szConnectionName);
}

static void releaseDatabase()
{
    if(g_pGlobalDatabaseObj)
    {
        delete g_pGlobalDatabaseObj;
        g_pGlobalDatabaseObj = Q_NULLPTR;
    }
}


//------------------------------------------------------------------------------

Database::Database(const QString &dbname,
                   const QString &user,
                   const QString &pwd,
                   const QString &hostname,
                   int port,
                   QObject *parent)
    : QObject(parent)
{
    name_ = dbname;
    user_ = user;
    pwd_ = pwd;
    hostName_ = hostname;
    port_ = port;
}

Database::~Database() {}


bool Database::getRecord(const QString &table,
                         const QStringList &keyList,
                         QStringList &valueList,
                         const QString &expr)
{
    QSqlQuery query(db_);
    QString key,sql;

    if (keyList.isEmpty())
        return false;

    key = keyList.join(",");

    if (expr.isEmpty())
        sql = QString("select %1 from %2").arg(key).arg(table);
    else
        sql = QString("select %1 from %2 where %3").arg(key).arg(table).arg(expr);

    try {
        if(!query.exec(sql)) {
            LogError(QString("get record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
        if(query.next()) {
            for(int i = 0; i < keyList.count(); ++i)
                valueList << query.record().value(keyList.at(i)).toString();
        }
    } catch (...) {
        LogError(QString("get record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}

bool Database::getRecord(const QString &table,
                         const QString &key,
                         QStringList &valueList,
                         const QString &expr)
{
    QSqlQuery query(db_);
    QString sql;

    if (key.isEmpty()) return false;

    if (expr.isEmpty())
        sql = QString("select %1 from %2").arg(key).arg(table);
    else
        sql = QString("select %1 from %2 where %3").arg(key).arg(table).arg(expr);

    try {
        if(!query.exec(sql)) {
            LogError(QString("get record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
        //return the first one value
        while(query.next())
            valueList << query.record().value(key).toString();
    } catch (...) {
        LogError(QString("get record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}

bool Database::getRecord(const QString &table,
                         const QString &key,
                         QString &value,
                         const QString &expr)
{
    QSqlQuery query(db_);
    QString sql;

    if (key.isEmpty()) return false;

    if (expr.isEmpty())
        sql = QString("select %1 from %2").arg(key).arg(table);
    else
        sql = QString("select %1 from %2 where %3").arg(key).arg(table).arg(expr);

    try {
        if (!query.exec(sql)) {
            LogError(QString("get record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }

        if(query.next())
            value = query.record().value(key).toString();
    } catch (...) {
        LogError(QString("get record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}

bool Database::getRecord(const QString &table,
                         const QStringList &keyList,
                         QList<QStringList> &valueListList,
                         const QString &expr)
{
    QSqlQuery query(db_);
    QString key,sql;
    QStringList valueList;

    if (keyList.isEmpty())
        return false;

    key = keyList.join(",");

    if (expr.isEmpty())
        sql = QString("select %1 from %2").arg(key).arg(table);
    else
        sql = QString("select %1 from %2 where %3").arg(key).arg(table).arg(expr);

    try {
        if (!query.exec(sql)) {
            LogError(QString("get record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
        // return all record
        while (query.next()) {
            valueList.clear();
            for (int i = 0; i < keyList.count(); ++i)
                valueList << query.record().value(keyList.at(i)).toString();
            if(keyList.count() == valueList.count())
                valueListList << valueList;
        }
    } catch (...) {
        LogError(QString("get record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}


bool Database::setRecord(const QString &table,
                         const QString &key,
                         const QString &value,
                         const QString &expr)
{
    QSqlQuery query(db_);
    QString sql;

    if(expr.isEmpty())
        sql = QString("update %1 set %2='%3'").arg(table).arg(key).arg(value);
    else
        sql = QString("update %1 set %2='%3' where %4").arg(table).arg(key).arg(value).arg(expr);
    try {
        if(!query.exec(sql)) {
            LogError(QString("set record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
    } catch (...) {
        LogError(QString("set record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}

bool Database::setRecord(const QString &table,
                         const QStringList &keyList,
                         const QStringList &valueList,
                         const QString &expr)
{
    int i,count;
    QSqlQuery query(db_);
    QString key,sql;

    count = keyList.count();
    if(!count || count != valueList.count()) {
        LogError(QString("set record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for (i = 0; i < count; ++i)
        key += keyList.at(i) + "='" + valueList.at(i) + "',";
    key.chop(1);

    if(expr.isEmpty())
        sql = QString("update %1 set %2").arg(table).arg(key);
    else
        sql = QString("update %1 set %2 where %3").arg(table).arg(key).arg(expr);
    try {
        if(!query.exec(sql)) {
            LogError(QString("set record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
    } catch (...) {
        LogError(QString("set record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}

bool Database::insertRecord(const QString &table,
                            const QStringList &keyList,
                            const QStringList &valueList,
                            const QString &expr)
{
    int i,count;
    QSqlQuery query(db_);
    QString key,value,sql;

    count = keyList.count();
    if(!count || count != valueList.count()) {
        LogError(QString("insert record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for (i = 0; i < count; ++i) {
        key += keyList.at(i) + ",";
        value += "'" + valueList.at(i) + "',";
    }
    key.chop(1);
    value.chop(1);

    if(expr.isEmpty())
        sql = QString("insert into %1(%2) values(%3)").arg(table).arg(key).arg(value);
    else
        sql = QString("insert into %1(%2) values(%3) where %4").arg(table).arg(key).arg(value).arg(expr);
    try {
        if(!query.exec(sql)) {
            LogError(QString("insert record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
    } catch (...) {
        LogError(QString("insert record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}

bool Database::insertRecord(const QString &table,
                            const QString &key,
                            const QStringList &valueList,
                            const QString &expr)
{
    QSqlQuery query(db_);
    QString value,sql;
    if(key.isEmpty() || !valueList.count()) {
        LogError(QString("insert record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for (int i = 0; i < valueList.count(); ++i) {
        value += "(" + valueList.at(i) + "),";
    }
    value.chop(1);

    if(expr.isEmpty())
        sql = QString("insert into %1(%2) values %3 ").arg(table).arg(key).arg(value);
    else
        sql = QString("insert into %1(%2) values %3 where %4").arg(table).arg(key).arg(value).arg(expr);

    try {
        if(!query.exec(sql)) {
            LogError(QString("insert record: %1 failed! %2 ,error: %3!")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
    } catch (...) {
        LogError(QString("insert record: %1 failed! Exception: %2")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}


bool Database::copyRecord(const QString &tableFrom,
                          const QString &tableTo,
                          const QString &expr)
{
    QSqlQuery query(db_);
    QString sql;
    if(tableFrom.isEmpty() || tableTo.isEmpty()) {
        LogError(QString("copy record: form %1 to %2 failed!").arg(tableFrom).arg(tableTo));
        return false;
    }

    if(expr.isEmpty())
        sql = QString("insert into %1 select * from %2 ").arg(tableTo).arg(tableFrom);
    else
        sql = QString("insert into %1 select * from %2 where %3").arg(tableTo).arg(tableFrom).arg(expr);

    try {
        if (!query.exec(sql)) {
            LogError(QString("copy record: form %1 to %2 failed! %3 ,error: %4!")
                     .arg(tableFrom)
                     .arg(tableTo)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
    } catch (...) {
        LogError(QString("copy record: form %1 to %2 failed! Exception: %3")
                 .arg(tableFrom)
                 .arg(tableTo)
                 .arg(sql));
        return false;
    }
    return true;
}

bool Database::deleteRecord(const QString &table, const QString &expr) {
    QSqlQuery query(db_);
    QString sql;

    if (expr.isEmpty())
        sql = QString("delete from %1").arg(table);
    else
        sql = QString("delete from %1 where %2").arg(table).arg(expr);

    try {
        if (!query.exec(sql)) {
            LogError(QString("delete record from %1 failed! %2, error: %3")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
    } catch (...) {
        LogError(QString("delete record to %1 failed! %2, Exception")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}

void Database::excSQL(const QString &sql) {
    QList<QStringList> result;
    excSQL(sql, result);
}

void Database::excSQL(const QString &sql, QList<QStringList> &result) {
    QSqlQuery query(db_);

    result.clear();

    try {
        LogInfo(QString("DB sql:%1").arg(sql));

        if (!query.exec(sql)) {
            LogError(QString("sql execute fail:(%1) sql:%2")
                     .arg(query.lastError().text())
                     .arg(sql));
        }

        while (query.next()) {
            int nField = query.record().count();
            QStringList valueList;
            for (int i = 0; i < nField; ++i)
                valueList << query.record().value(i).toString();

            result.append(valueList);
        }

    } catch (...) {
        LogError(QString("sql execute exception:(%1) sql:%2")
                 .arg(query.lastError().text())
                 .arg(sql));
    }
}

int Database::getLastInsertId(const QString &tableName) {
    int id = 0;
    QSqlQuery query(db_);
    QSqlRecord rec;

    QString szSql = QString("SELECT DISTINCT last_insert_rowid() as id FROM %1").arg(tableName);
    bool ret = query.exec(szSql);
    if(!ret) {
        LogError(QString("getLastInsertId: %1 failed! %2 ,error: %3!")
                 .arg(tableName)
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    while (query.next()) {
        rec = query.record();
        id = rec.value("id").toInt();
    }

    return id;
}

int Database::getMaxId(const QString &tableName) {
    int id = 0;
    QSqlQuery query(db_);
    QSqlRecord rec;

    QString szSql = QString("select MAX(id) as id from %1").arg(tableName);
    bool ret = query.exec(szSql);
    if (!ret) {
        LogError(QString("getMaxId: %1 failed! %2 ,error: %3!")
                 .arg(tableName)
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    while (query.next()) {
        rec = query.record();
        id = rec.value("id").toInt();
    }

    return id;
}

int Database::getRowCount(const QString &tableName, const QString &expr) {
    int iCount = 0;
    QString szSql = "";
    QSqlQuery query(db_);
    QSqlRecord rec;

    if (expr.isEmpty()) {
        szSql = QString("SELECT COUNT(*) as nums FROM %1;").arg(tableName);
    } else {
        szSql = QString("SELECT COUNT(*) as nums FROM %1 WHERE %2")
                .arg(tableName)
                .arg(expr);
    }

    bool ret = query.exec(szSql);
    if (!ret) {
        LogError(QString("getRowCount: %1 failed! %2 ,error: %3!")
                 .arg(tableName)
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    while (query.next()) {
        rec = query.record();
        iCount = rec.value("nums").toInt();
    }

    return iCount;
}


QSqlQuery Database::createQuery()
{
    return QSqlQuery(db_);
}

/**
 * @brief Database::beginTransaction
 * @details 声明事务开始
 * @return true-成功, false-失败
 */
bool Database::beginTransaction()
{
    return db_.driver()->beginTransaction();
}

/**
 * @brief Database::rollbackTransaction
 * @details 回滚
 * @return true-成功, false-失败
 */
bool Database::rollbackTransaction()
{
    return db_.driver()->rollbackTransaction();
}

/**
 * @brief Database::commitTransaction
 * @details 事务提交
 * @return true-成功, false-失败
 */
bool Database::commitTransaction()
{
    return db_.driver()->commitTransaction();
}


/**
 * @brief Database::isExistTable
 * @details 是否存在表
 * @param szTableName
 * @return true-否存, false-不否存
 */
bool Database::isExistTable(const QString &szTableName)
{
    static QStringList szListTable = db_.tables();
    return (szListTable.indexOf(szTableName) >= 0);
}


/**
 * @brief Database::database
 * @details 获取线程的数据库对象
 * @return 线程数据库对象
 */
QSqlDatabase Database::database()
{
    if (!g_pGlobalDatabaseObj)
        return QSqlDatabase();

    // 如果在主线程运行，直接返回数据库连接
    QThread *pThreadObj = QThread::currentThread();
    if (pThreadObj == g_pGlobalDatabaseObj->thread()) {
        //LogInfo(QString("[main thread id: %3] get database object").arg(QString::number((ulong)pThreadObj->currentThreadId())));
        return g_pGlobalDatabaseObj->m_reference;
    }

    // 如果运行线程已有数据库连接，直接返回数据库连接
    QMutexLocker locker(&g_pGlobalDatabaseObj->m_mutex);
    if (g_pGlobalDatabaseObj->m_mapCopies.contains(pThreadObj)) {
        //LogInfo(QString("[copy thread id: %3] get database object").arg(QString::number((ulong)pThreadObj->currentThreadId())));
        return g_pGlobalDatabaseObj->m_mapCopies[pThreadObj];
    }

    // 为线程创建新的数据库连接
    QObject::connect(pThreadObj, SIGNAL(finished()), g_pGlobalDatabaseObj, SLOT(onThreadFinished()));
    QSqlDatabase db = QSqlDatabase::cloneDatabase(g_pGlobalDatabaseObj->m_reference,
                                                  QLatin1String(g_pszConnectionPrefix) + QString::number(g_pGlobalDatabaseObj->m_llConnectionId++));

    const QString driverName = db.driverName();
    if (driverName == QLatin1String("QMYSQL") ||  driverName == QLatin1String("QMYSQL3"))
    {
        db.setHostName(g_pGlobalDatabaseObj->m_szHostName);
        db.setPort(g_pGlobalDatabaseObj->m_iPort);
        if(!db.open(g_pGlobalDatabaseObj->m_szUser, g_pGlobalDatabaseObj->m_szPwd))
        {
            LogError(QString("Open Database Failed!"));
            return QSqlDatabase();
        }
        QSqlQuery query(db);
        query.exec(QString("use %1").arg(g_pGlobalDatabaseObj->m_szName));
        db.exec("set names 'utf-8'");
    }
    else
    {
        db.open();
    }

    g_pGlobalDatabaseObj->m_mapCopies.insert(pThreadObj, db);
    LogInfo(QString("[new thread ID: %3] get database object").arg(QString::number((ulong)pThreadObj->currentThreadId())));

    return db;
}


/**
 * @brief Database::setDatabase
 * @details 设置数据库对象
 * @param database 数据库对象
 * @param szDBName 数据库名称
 * @param szUser 数据库用户名
 * @param szPwd 数据库密码
 * @param szHostName 数据库IP地址
 * @param iPort 数据库端口
 */
void Database::setDatabase(QSqlDatabase database,
                           const QString &szDBName,
                           const QString &szUser,
                           const QString &szPwd,
                           const QString &szHostName,
                           int iPort)
{
    if (!g_pGlobalDatabaseObj) {
        g_pGlobalDatabaseObj = new DatabaseWrapper();
        qAddPostRoutine(releaseDatabase);
    }
    g_pGlobalDatabaseObj->m_reference = database;
    g_pGlobalDatabaseObj->m_szName = szDBName;
    g_pGlobalDatabaseObj->m_szUser = szUser;
    g_pGlobalDatabaseObj->m_szPwd = szPwd;
    g_pGlobalDatabaseObj->m_szHostName = szHostName;
    g_pGlobalDatabaseObj->m_iPort = iPort;
    LogInfo(QString("set database object thread id: %3").arg(QString::number((ulong)g_pGlobalDatabaseObj->thread()->currentThreadId())));
}

