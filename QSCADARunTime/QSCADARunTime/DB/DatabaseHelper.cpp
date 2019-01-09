#include "DatabaseHelper.h"
#include "log4qt/logger.h"


DatabaseHelper::DatabaseHelper(const QString &dbname,
                               const QString &user,
                               const QString &pwd,
                               DBType type,
                               QObject *parent)
    : QObject(parent)
{
    name_ = dbname;
    user_ = user;
    pwd_ = pwd;
    type_ = type;
}

DatabaseHelper::~DatabaseHelper()
{
    closeDatabase();
}

bool DatabaseHelper::openDatabase()
{
    try
    {
        if(type_ == DB_MYSQL)
        {
			QSqlDatabase db;
			db = QSqlDatabase::addDatabase("QMYSQL");
            if(!db.open(user_, pwd_))
            {
                Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
                log->error(QString("Open Database Failed!"));
				return false;
			}
			db.exec("set names 'utf-8'");
        }
        else if(type_ == DB_SQLITE)
        {
			QSqlDatabase db;
			db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(name_);
            if(!db.open())
            {
                Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
                log->error(QString("Open Database Failed!"));
				return false;
			}
        }
        else
        {

		}
    }
    catch(...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("Open Database Failed Exception!"));
        return false;
	}
	return true;
}

bool DatabaseHelper::closeDatabase()
{
    return false;
}

bool DatabaseHelper::isOpenDatabase()
{
	QSqlQuery query;
	bool ret = false;
    try
    {
        if(type_ == DB_MYSQL)
        {
			ret = query.exec("select now()");
        }
        else if(type_ == DB_SQLITE)
        {
			ret = query.exec("select CURRENT_TIMESTAMP");
        }
        else
        {
            
		}
        if(!ret)
        {
			ret = openDatabase();
		}
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("Open Database: %1 Failed Exception!").arg(name_));
		return false;
	}
	return ret;
}

bool DatabaseHelper::createDatabase()
{
	QSqlQuery query;
	try
	{
        query.exec(QString("show databases like '%1'").arg(name_));
		if(!query.next())
		{
            if(!query.exec(QString("create database %1").arg(name_)))
			{
                Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
                log->error(QString("Create Database: %1 Failed!").arg(name_));
				return false;
			}
		}
        query.exec(QString("use %1").arg(name_));
	}
	catch (...)
	{
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("Create Database: %1 Failed Exception!").arg(name_));
		return false;
	}
	return true;
}

int DatabaseHelper::createTable(QString table,
                                QStringList fieldList,
                                QStringList typeList,
                                QString index)
{
    QString key = QString();
    QSqlQuery query;
    int pos = 0, ret = 0;
    int count = fieldList.count();
    
    if(table.isEmpty() || count == 0 || count != typeList.count())
	{
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("create table: %1 failed!").arg(table));
        return 0;
	}

    if(type_ == DB_MYSQL)
    {
        query.exec(QString("show tables like '%1'").arg(table));
    }
    else if(type_ == DB_SQLITE)
    {
        query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));
    }

    if(!query.next() || (type_ == DB_SQLITE && query.value(0).toInt() == 0))
    {
        for(int i=0; i<fieldList.count(); ++i)
        {
            key += fieldList.at(i) + " " + typeList.at(i) + ",";
        }

        if(index.isEmpty())
        {
            key.chop(1);
        }
        else
        {
            key += QString("index(%1)").arg(index);
        }

        if(!query.exec(QString("create table %1(%2)").arg(table,key)))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("create table: %1 failed! deatil: %2!").arg(table).arg(query.lastError().text()));
            return 0;
        }
        return 1;
    }
    else 
    {
        QStringList oldFieldList,oldTypeList;
        if(type_ == DB_MYSQL)
        {
            query.exec(QString("desc %1").arg(table));
        }
        else if(type_ == DB_SQLITE)
        {
            query.exec(QString("pragma table_info (%1)").arg(table));
        }
        while(query.next())
        {
            if(type_ == DB_MYSQL)
            {
                oldFieldList << query.record().value("Field").toString();
                oldTypeList  << query.record().value("Type").toString();
            }
            else if(type_ == DB_SQLITE)
            {
                oldFieldList << query.record().value("name").toString();
                oldTypeList  << query.record().value("Type").toString();
            }
        }
        for(int i=0;i<count;i++)
        {
            pos = oldFieldList.indexOf(fieldList.at(i));
            if(pos == -1) 
            {
                if(!query.exec(QString("alter table %1 add column %2 %3").arg(table,fieldList.at(i),typeList.at(i))))
                {
                    Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
                    log->error(QString("alter table: %1 failed! deatil: Failed or Exception").arg(table));
                    return 0;
                }
                ret = 2;
            } 
            else if(type_ == DB_MYSQL && !typeList.at(i).contains("key", Qt::CaseInsensitive) &&
                    !typeList.at(i).contains(oldTypeList.at(pos), Qt::CaseInsensitive))
			{
				if(!query.exec(QString("alter table %1 change %2 %3 %4").arg(table,oldFieldList.at(pos),fieldList.at(i),typeList.at(i))))
				{
                    Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
                    log->error(QString("alter table: %1 failed! deatil: Failed or Exception").arg(table));
					return 0;
				}
				ret = 3;
			}
        }
        return ret;
    }
    return 4;
}

bool DatabaseHelper::createTable()
{
	QString autoincrement;
    QStringList keyList,valueList;
    int ok1;

    if(type_ == DB_MYSQL)
		autoincrement = "int not null auto_increment primary key";
	else
		autoincrement = "integer not null primary key autoincrement";

    // test table
    keyList.clear();
    valueList.clear();
    keyList << "id"
            << "name";
    valueList << autoincrement
              << "varchar(20)";
    ok1 = createTable("test", keyList, valueList);

    return ok1;
}

bool DatabaseHelper::getRecord(const QString &table,
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

    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("get record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
        if(query.next())
        {
            for(i = 0; i < count; ++i)
                valueList << query.record().value(keyList.at(i)).toString();
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("get record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
    }
	return true;
}

bool DatabaseHelper::getRecord(const QString &table,
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

    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("get record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
        //return the first one value
        while(query.next())
            valueList << query.record().value(key).toString();
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("get record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
    }
	return true;
}

bool DatabaseHelper::getRecord(const QString &table,
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

    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("get record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }

        if(query.next())
            value = query.record().value(key).toString();
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("get record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
    }
	return true;
}

bool DatabaseHelper::getRecord(const QString &table,
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

	try
	{
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("get record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
        //return all record
        count = keyList.count();
        while(query.next())
        {
            valueList.clear();
            for(i = 0; i < count; ++i)
                valueList << query.record().value(keyList.at(i)).toString();

            if(count == valueList.count())
                valueListList << valueList;
        }
	}
	catch (...)
	{
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("get record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
	}
	return true;
}


bool DatabaseHelper::setRecord(const QString &table,
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
    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("set record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("set record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
    }
    return true;
}

bool DatabaseHelper::setRecord(const QString &table,
                               const QStringList &keyList,
                               const QStringList &valueList,
                               const QString &expr)
{
	int i,count;
    QSqlQuery query;
    QString key,sql;

    count = keyList.count();
    if(!count || count != valueList.count())
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("set record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(i = 0; i < count; ++i)
        key += keyList.at(i) + "='" + valueList.at(i) + "',";
    key.chop(1);

    if(expr.isEmpty())
        sql = QString("update %1 set %2").arg(table).arg(key);
    else
        sql = QString("update %1 set %2 where %3").arg(table).arg(key).arg(expr);
    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("set record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("set record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
    }
    return true;
}

bool DatabaseHelper::insertRecord(const QString &table,
                                  const QStringList &keyList,
                                  const QStringList &valueList,
                                  const QString &expr)
{
	int i,count;
    QSqlQuery query;
    QString key,value,sql;

    count = keyList.count();
    if(!count || count != valueList.count())
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("insert record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(i = 0; i < count; ++i)
    {
		key += keyList.at(i) + ",";
        value += "'" + valueList.at(i) + "',";
    }
    key.chop(1);
    value.chop(1);

    if(expr.isEmpty())
        sql = QString("insert into %1(%2) values(%3)").arg(table).arg(key).arg(value);
    else
        sql = QString("insert into %1(%2) values(%3) where %4").arg(table).arg(key).arg(value).arg(expr);
    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("insert record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("insert record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
    }
    return true;
}

bool DatabaseHelper::insertRecord(const QString &table,
                                  const QString &key,
                                  const QStringList &valueList,
                                  const QString &expr)
{
    QSqlQuery query;
    QString value,sql;
    if(key.isEmpty() || !valueList.count())
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("insert record: %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(int i = 0; i < valueList.count(); ++i)
    {
        value += "(" + valueList.at(i) + "),";
    }
    value.chop(1);

    if(expr.isEmpty())
        sql = QString("insert into %1(%2) values %3 ").arg(table).arg(key).arg(value);
    else
        sql = QString("insert into %1(%2) values %3 where %4").arg(table).arg(key).arg(value).arg(expr);

    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("insert record: %1 failed! %2 ,error: %3!")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("insert record: %1 failed! Exception: %2")
                   .arg(table)
                   .arg(sql));
        return false;
    }
    return true;
}


bool DatabaseHelper::copyRecord(const QString &tableFrom,
                                const QString &tableTo,
                                const QString &expr)
{
    QSqlQuery query;
    QString sql;
    if(tableFrom.isEmpty() || tableTo.isEmpty())
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("copy record: form %1 to %2 failed!").arg(tableFrom).arg(tableTo));
        return false;
    }

    if(expr.isEmpty())
        sql = QString("insert into %1 select * from %2 ").arg(tableTo).arg(tableFrom);
    else
        sql = QString("insert into %1 select * from %2 where %3").arg(tableTo).arg(tableFrom).arg(expr);

    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("copy record: form %1 to %2 failed! %3 ,error: %4!")
                       .arg(tableFrom)
                       .arg(tableTo)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("copy record: form %1 to %2 failed! Exception: %3")
                   .arg(tableFrom)
                   .arg(tableTo)
                   .arg(sql));
        return false;
    }
    return true;
}

bool DatabaseHelper::insertOrUpdateRecord(const QString &table,
                                          const QStringList &keyList,
                                          const QStringList &valueList)
{
    QSqlQuery query;
    QString key,value,update,sql;
    int count = keyList.count();

    if(!count || count != valueList.count())
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("insert record to %1 failed! keylist.count != valuelist.count").arg(table));
        return false;
    }

    for(int i = 0; i < count; ++i)
    {
        key += keyList.at(i) + ",";
        value += "'" + valueList.at(i) + "',";
    }
    key.chop(1);
    value.chop(1);
    if(type_ == DB_MYSQL)
	{
		for(int i = 0; i < count; ++i)
			update += keyList.at(i) + "='" + valueList.at(i) + "',";
		update.chop(1);
        sql = QString("insert into %1(%2) value(%3) ON DUPLICATE KEY UPDATE %4")
                .arg(table)
                .arg(key)
                .arg(value)
                .arg(update);
	}
	else
        sql = QString("replace into %1(%2) value(%3)")
                .arg(table)
                .arg(key)
                .arg(value);
  
    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("insert record to %1 failed! %2, error: %3")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("insert record to %1 failed! %2, Exception")
                   .arg(table)
                   .arg(sql));
        return false;
    }
    return true;
}

bool DatabaseHelper::deleteRecord(const QString &table, const QString &expr)
{
    QSqlQuery query;
    QString sql;
    if(expr.isEmpty())
        sql = QString("delete from %1").arg(table);
    else
        sql = QString("delete from %1 where %2").arg(table).arg(expr);
    try
    {
        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("delete record from %1 failed! %2, error: %3")
                       .arg(table)
                       .arg(sql)
                       .arg(query.lastError().text()));
            return false;
        }
    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("delete record to %1 failed! %2, Exception")
                   .arg(table)
                   .arg(sql));
        return false;
    }
    return true;
}


bool DatabaseHelper::isContain(const QString& table)
{
    bool ret = false;
    QSqlQuery query;

    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));

    if (!query.next() || query.value(0).toInt() == 0)
        ret = false;
    else
        ret = true;

    return ret;
}


void DatabaseHelper::excSQL(const QString& sql)
{
    QList<QStringList> result;
    excSQL(sql, result);
}

void DatabaseHelper::excSQL(const QString& sql, QList<QStringList>& result)
{
    QSqlQuery query;

    result.clear();

    try
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->info(QString("DB sql:%1").arg(sql));

        if(!query.exec(sql))
        {
            Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
            log->error(QString("sql execute fail:(%1) sql:%2")
                       .arg(query.lastError().text())
                       .arg(sql));
        }

        while(query.next())
        {
            int nField = query.record().count();
            QStringList valueList;
            for( int i = 0; i < nField; ++i)
                valueList << query.record().value(i).toString();

            result.append(valueList);
        }


    }
    catch (...)
    {
        Log4Qt::Logger *log = Log4Qt::Logger::logger("DatabaseHelper");
        log->error(QString("sql execute exception:(%1) sql:%2")
                   .arg(query.lastError().text())
                   .arg(sql));
    }

}

