#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QStringList>
#include <QVariant>
#include <QObject>


class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(const QString &dbname = "xpen",
                      const QString &user = "root",
                      const QString &pwd = "725431",
                      const QString &hostname = "127.0.0.1",
                      int port = 0,
                      QObject *parent = nullptr);
    virtual ~Database();

    virtual bool openDatabase() = 0;
    virtual bool closeDatabase() = 0;
    virtual bool isOpenDatabase() = 0;
    virtual bool createDatabase() = 0;

    //when table is not exist then create it
    virtual bool createTables() = 0;

    virtual int createTable(const QString &table,
                            QStringList fieldList,
                            QStringList typeList,
                            const QString &index="") = 0;

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

    virtual bool insertOrUpdateRecord(const QString &table,
                                      const QStringList &keyList,
                                      const QStringList &valueList) = 0;

    void excSQL(const QString& sql, QList<QStringList>& result);
    void excSQL(const QString& sql );

    int getLastInsertId(const QString &tableName);
    int getMaxId(const QString &tableName);
    int getRowCount(const QString &tableName, const QString &expr);

    QSqlQuery createQuery();

    // 声明事务开始
    bool beginTransaction();
    // 回滚
    bool rollbackTransaction();
    // 事务提交
    bool commitTransaction();
    // 是否存在表
    bool isExistTable(const QString &szTableName);

public:
    QString name_;
    QString user_;
    QString pwd_;
    QString hostName_;
    int port_;
    QSqlDatabase db_;
};

#endif // Database_H
