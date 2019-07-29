#include "SQLiteDatabase.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "ulog.h"

SQLiteDatabase::SQLiteDatabase(const QString &dbname,
                               const QString &user,
                               const QString &pwd,
                               const QString &hostname,
                               int port,
                               QObject *parent)
    : Database(dbname, user, pwd, hostname, port, parent)
{
    try {
        if(QSqlDatabase::contains("sqlite_connection")) {
            db_ = QSqlDatabase::database("sqlite_connection");
        } else {
            db_ = QSqlDatabase::addDatabase("QSQLITE", "sqlite_connection");
        }
    } catch(...) {
        LogError(QString("Add Database Failed Exception!"));
    }
}

SQLiteDatabase::~SQLiteDatabase() { closeDatabase(); }

bool SQLiteDatabase::openDatabase()
{
    closeDatabase();
    try {
        db_.setDatabaseName(name_);
        if(!db_.open()) {
            LogError(QString("Open Database Failed!"));
            return false;
        }
    } catch(...) {
        LogError(QString("Open Database Failed Exception!"));
        return false;
	}
	return true;
}

bool SQLiteDatabase::closeDatabase()
{
    if(db_.isOpen()) {
        db_.close();
    }
    return false;
}

bool SQLiteDatabase::isOpenDatabase()
{
    QSqlQuery query(db_);
	bool ret = false;
    try {
        ret = query.exec("select CURRENT_TIMESTAMP");
        if(!ret) ret = openDatabase();
    } catch (...) {
        LogError(QString("Open Database: %1 Failed Exception!").arg(name_));
		return false;
	}
	return ret;
}

bool SQLiteDatabase::createDatabase()
{
	return true;
}

int SQLiteDatabase::createTable(const QString &table,
                                QStringList fieldList,
                                QStringList typeList,
                                const QString &index)
{
    QString key = QString();
    QSqlQuery query(db_);
    int pos = 0, ret = 0;
    int count = fieldList.count();
    
    if(table.isEmpty() || count == 0 || count != typeList.count()) {
        LogError(QString("create table: %1 failed!").arg(table));
        return 0;
	}

    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));

    if(!query.next() || (query.value(0).toInt() == 0)) {
        for(int i=0; i<fieldList.count(); ++i)
            key += fieldList.at(i) + " " + typeList.at(i) + ",";

        if(index.isEmpty()) key.chop(1);
        else key += QString("index(%1)").arg(index);

        if(!query.exec(QString("create table %1(%2)").arg(table,key))) {
            LogError(QString("create table: %1 failed! deatil: %2!").arg(table).arg(query.lastError().text()));
            return 0;
        }
        return 1;
    } else {
        QStringList oldFieldList,oldTypeList;
        query.exec(QString("pragma table_info (%1)").arg(table));

        while(query.next()) {
            oldFieldList << query.record().value("name").toString();
            oldTypeList  << query.record().value("Type").toString();
        }

        for(int i=0;i<count;i++) {
            pos = oldFieldList.indexOf(fieldList.at(i));
            if(pos == -1) {
                if(!query.exec(QString("alter table %1 add column %2 %3")
                               .arg(table,fieldList.at(i),typeList.at(i)))) {
                    LogError(QString("alter table: %1 failed! deatil: Failed or Exception").arg(table));
                    return 0;
                }
                ret = 2;
            } 
        }
        return ret;
    }
    return 4;
}

bool SQLiteDatabase::createTables() { return false; }


bool SQLiteDatabase::insertOrUpdateRecord(const QString &table,
                                          const QStringList &keyList,
                                          const QStringList &valueList)
{
    QSqlQuery query(db_);
    QString key,value,update,sql;
    int count = keyList.count();

    if(!count || count != valueList.count()) {
        LogError(QString("insert record to %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(int i = 0; i < count; ++i) {
        key += keyList.at(i) + ",";
        value += "'" + valueList.at(i) + "',";
    }
    key.chop(1);
    value.chop(1);

    sql = QString("replace into %1(%2) value(%3)")
            .arg(table)
            .arg(key)
            .arg(value);
  
    try {
        if(!query.exec(sql)) {
            LogError(QString("insert record to %1 failed! %2, error: %3")
                     .arg(table)
                     .arg(sql)
                     .arg(query.lastError().text()));
            return false;
        }
    } catch (...) {
        LogError(QString("insert record to %1 failed! %2, Exception")
                 .arg(table)
                 .arg(sql));
        return false;
    }
    return true;
}



bool SQLiteDatabase::isContain(const QString& table)
{
    bool ret = false;
    QSqlQuery query(db_);

    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));

    if (!query.next() || query.value(0).toInt() == 0)
        ret = false;
    else
        ret = true;

    return ret;
}




