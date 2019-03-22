#include "Database.h"
#include "Log.h"


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

Database::~Database()
{
}


bool Database::getRecord(const QString &table,
                         const QStringList &keyList,
                         QStringList &valueList,
                         const QString &expr)
{
    int i,count;
    QSqlQuery query;
    QString key,sql;

    if(keyList.isEmpty())
        return false;

    count = keyList.count();
    for(i = 0; i < count; ++i)
        key += keyList.at(i) + ",";
    key.chop(1);

    if(expr.isEmpty())
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
            for(i = 0; i < count; ++i)
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
    QSqlQuery query;
    QString sql;

    if(key.isEmpty())
        return false;

    if(expr.isEmpty())
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
    QSqlQuery query;
    QString sql;

    if(key.isEmpty())
        return false;

    if(expr.isEmpty())
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
    int i,count;
	QSqlQuery query;
    QString key,sql;
	QStringList valueList;

	if(keyList.isEmpty())
		return false;

	count = keyList.count();
	for(i = 0; i < count; ++i)
		key += keyList.at(i) + ",";
	key.chop(1); 

	if(expr.isEmpty())
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
        //return all record
        count = keyList.count();
        while(query.next()) {
            valueList.clear();
            for(i = 0; i < count; ++i)
                valueList << query.record().value(keyList.at(i)).toString();

            if(count == valueList.count())
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
    QSqlQuery query;
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
    QSqlQuery query;
    QString key,sql;

    count = keyList.count();
    if(!count || count != valueList.count()) {
        LogError(QString("set record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(i = 0; i < count; ++i)
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
    QSqlQuery query;
    QString key,value,sql;

    count = keyList.count();
    if(!count || count != valueList.count()) {
        LogError(QString("insert record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(i = 0; i < count; ++i) {
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
    QSqlQuery query;
    QString value,sql;
    if(key.isEmpty() || !valueList.count()) {
        LogError(QString("insert record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(int i = 0; i < valueList.count(); ++i) {
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
    QSqlQuery query;
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
        if(!query.exec(sql)) {
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

bool Database::deleteRecord(const QString &table, const QString &expr)
{
    QSqlQuery query;
    QString sql;

    if(expr.isEmpty())
        sql = QString("delete from %1").arg(table);
    else
        sql = QString("delete from %1 where %2").arg(table).arg(expr);

    try {
        if(!query.exec(sql)) {
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

void Database::excSQL(const QString& sql)
{
    QList<QStringList> result;
    excSQL(sql, result);
}

void Database::excSQL(const QString& sql, QList<QStringList>& result)
{
    QSqlQuery query;

    result.clear();

    try {
        LogInfo(QString("DB sql:%1").arg(sql));

        if(!query.exec(sql)) {
            LogError(QString("sql execute fail:(%1) sql:%2")
                     .arg(query.lastError().text())
                     .arg(sql));
        }

        while(query.next()) {
            int nField = query.record().count();
            QStringList valueList;
            for( int i = 0; i < nField; ++i)
                valueList << query.record().value(i).toString();

            result.append(valueList);
        }


    } catch (...) {
        LogError(QString("sql execute exception:(%1) sql:%2")
                 .arg(query.lastError().text())
                 .arg(sql));
    }

}

