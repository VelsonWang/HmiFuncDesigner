#include "MySQLDatabase.h"
#include "Log.h"

MySQLDatabase::MySQLDatabase(const QString &dbname,
                             const QString &user,
                             const QString &pwd,
                             const QString &hostname,
                             int port,
                             QObject *parent)
    : Database(dbname, user, pwd, hostname, port, parent)
{

}

MySQLDatabase::~MySQLDatabase()
{
    closeDatabase();
}

bool MySQLDatabase::openDatabase()
{
    try {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(hostName_);
        db.setPort(port_);
        if(!db.open(user_, pwd_)) {
            LogError(QString("Open Database Failed!"));
            return false;
        }
        db.exec("set names 'utf-8'");
    } catch(...) {
        LogError(QString("Open Database Failed Exception!"));
        return false;
	}
	return true;
}

bool MySQLDatabase::closeDatabase()
{
    return false;
}

bool MySQLDatabase::isOpenDatabase()
{
	QSqlQuery query;
	bool ret = false;
    try {
        ret = query.exec("select now()");
        if(!ret) ret = openDatabase();
    }
    catch (...)
    {
        LogError(QString("Open Database: %1 Failed Exception!").arg(name_));
		return false;
	}
	return ret;
}

bool MySQLDatabase::createDatabase()
{
	QSqlQuery query;
    try {
        query.exec(QString("show databases like '%1'").arg(name_));
        if(!query.next()) {
            if(!query.exec(QString("create database %1").arg(name_))) {
                LogError(QString("Create Database: %1 Failed!").arg(name_));
				return false;
			}
		}
        query.exec(QString("use %1").arg(name_));
    } catch (...) {
        LogError(QString("Create Database: %1 Failed Exception!").arg(name_));
		return false;
	}
	return true;
}

int MySQLDatabase::createTable(const QString &table,
                               QStringList fieldList,
                               QStringList typeList,
                               const QString &index)
{
    QString key = QString();
    QSqlQuery query;
    int pos = 0, ret = 0;
    int count = fieldList.count();
    
    if(table.isEmpty() || count == 0 || count != typeList.count())
	{
        LogError(QString("create table: %1 failed!").arg(table));
        return 0;
	}

    query.exec(QString("show tables like '%1'").arg(table));

    if(!query.next()) {

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
        QStringList oldFieldList, oldTypeList;
        query.exec(QString("desc %1").arg(table));

        while(query.next()) {
            oldFieldList << query.record().value("Field").toString();
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
            else if(!typeList.at(i).contains("key", Qt::CaseInsensitive) &&
                    !typeList.at(i).contains(oldTypeList.at(pos), Qt::CaseInsensitive)) {
                if(!query.exec(QString("alter table %1 change %2 %3 %4")
                               .arg(table,oldFieldList.at(pos),fieldList.at(i),typeList.at(i)))) {
                    LogError(QString("alter table: %1 failed! deatil: Failed or Exception").arg(table));
					return 0;
				}
				ret = 3;
			}
        }
        return ret;
    }
    return 4;
}

bool MySQLDatabase::createTables()
{
    return false;
}


bool MySQLDatabase::insertOrUpdateRecord(const QString &table,
                                         const QStringList &keyList,
                                         const QStringList &valueList)
{
    QSqlQuery query;
    QString key,value,update,sql;
    int count = keyList.count();

    if(!count || count != valueList.count())
    {
        LogError(QString("insert record to %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(int i = 0; i < count; ++i)
    {
        key += keyList.at(i) + ",";
        value += "'" + valueList.at(i) + "',";
    }

    key.chop(1);
    value.chop(1);

    for(int i = 0; i < count; ++i)
        update += keyList.at(i) + "='" + valueList.at(i) + "',";

    update.chop(1);

    sql = QString("insert into %1(%2) value(%3) ON DUPLICATE KEY UPDATE %4")
            .arg(table)
            .arg(key)
            .arg(value)
            .arg(update);

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



