#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QtSql/QtSql>



typedef enum EDBType
{
    DB_MYSQL = 1,
    DB_SQLITE
}DBType;

class DatabaseHelper : public QObject
{
    Q_OBJECT
public:
    DatabaseHelper(const QString &dbname = ":memory:",
                   const QString &user = "root",
                   const QString &pwd = "725431",
                   DBType type = DB_SQLITE,
                   QObject *parent = 0);
    ~DatabaseHelper();
	bool openDatabase();
	bool closeDatabase();
	bool isOpenDatabase();
	bool createDatabase();
    //when table is not exist then create it
    bool createTable();

    int createTable(QString table,
                    QStringList fieldList,
                    QStringList typeList,
                    QString index="");

    //one key one value
    bool getRecord(const QString &table,
                   const QStringList &keyList,
                   QStringList &valueList,
                   const QString &expr="");

    //one key multi value
    bool getRecord(const QString &table,
                   const QString &key,
                   QStringList &valueList,
                   const QString &expr="");

    //only one key only one value
    bool getRecord(const QString &table,
                   const QString &key,
                   QString &value,
                   const QString &expr="");

	//one key multi value
    bool getRecord(const QString &table,
                   const QStringList &keyList,
                   QList<QStringList> &valueListList,
                   const QString &expr="");

    //set multi key 
    bool setRecord(const QString &table,
                   const QStringList &keyList,
                   const QStringList &valueList,
                   const QString &expr="");

    //set one key to one value
    bool setRecord(const QString &table,
                   const QString &key,
                   const QString &value,
                   const QString &expr="");

    //insert one record
    bool insertRecord(const QString &table,
                      const QStringList &keyList,
                      const QStringList &valueList,
                      const QString &expr="");

    bool insertRecord(const QString &table,
                      const QString &key,
                      const QStringList &valueList,
                      const QString &expr="");

    bool copyRecord(const QString &tableFrom,
                    const QString &tableTo,
                    const QString &expr="");

    //delete from table where expr
    bool deleteRecord(const QString &table,
                      const QString &expr="");

    bool insertOrUpdateRecord(const QString &table,
                              const QStringList &keyList,
                              const QStringList &valueList);

    bool isContain(const QString& table);

    void excSQL(const QString& sql, QList<QStringList>& result);
    void excSQL(const QString& sql );

private:
    QString name_;
    QString user_;
    QString pwd_;

public:
    DBType type_;
};

#endif // DATABASEHELPER_H
