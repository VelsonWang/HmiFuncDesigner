#include <QCoreApplication>
#include "fakebook.h"
#include <QtSql/QtSql>
#include <QDebug>
#include "objectloader.h"

#if 0
#include "qtpersistencetest.h"
#include "qtpersistence.h"
#include "databaseconfigurationtest.h"
#include "databaseconfiguration_p.h"
#include <QtSql/QSqlDatabase>


qtpTestData testData[] = {
    { QUrl("sqlite://localhost/path/to/dbfile"), "QSQLITE",
      "localhost", 0, QString(), QString(), "/path/to/dbfile", true, true },
    { QUrl("mysql://mysql:mysql@localhost:1234/test1"), "QMYSQL",
      "localhost", 1234, "mysql", "mysql", "test1", true, true },
    { QUrl("postgresql://pgsql:pgsql@localhost:5432"), "QPSQL",
      "localhost", 5432, "pgsql", "pgsql", QString(), true, true },
    { QUrl(), QString(), QString(), 0, QString(),
      QString(), QString(), false, false }
    };



TEST_P(DatabaseConfigurationTest, testUseDatabase)
{
    dbcTestData data = GetParam();
    DatabaseConfiguration* dbc = DatabaseConfiguration::self();
    bool configOk = dbc->configureDatabase(data.url);
    EXPECT_EQ(data.result, configOk);

    //QSqlDatabase db = QSqlDatabase::database("dbc-database", false [> open <]);
    //EXPECT_EQ(data.validity, db.isValid());

    EXPECT_EQ(data.system, dbc->databaseSystem());
    EXPECT_EQ(data.host, dbc->hostName());
    EXPECT_EQ(data.port, dbc->port());
    EXPECT_EQ(data.user, dbc->userName());
    EXPECT_EQ(data.database, dbc->databaseName());
}


#endif

void create_table()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qtp-database");
    //db.setDatabaseName(":memory:");
    db.setDatabaseName("fakebook.db");
    db.open();
    QSqlQuery q(db);
    q.exec ("CREATE TABLE fake_books ("
            " id INTEGER PRIMARY KEY AUTOINCREMENT,"
            " author TEXT,"
            " year_published INTEGER "
            ");");
    q.finish();

}

void removeDatabase()
{
    QSqlDatabase::removeDatabase("qtp-database");
}

void delete_table()
{
    QSqlDatabase db = QSqlDatabase::database("qtp-database");
    db.exec("DELETE FROM fake_books");
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    create_table();

    //-------------<insert>----------------
#if 1
    FakeBook* b = new FakeBook;
    b->setAuthor("Jason");
    b->setYearPublished(1999);
    bool objectSaved = b->save();
    delete b;

    b = new FakeBook;
    b->setAuthor("Lucy");
    b->setYearPublished(2009);
    objectSaved = b->save();
    delete b;

    b = new FakeBook;
    b->setYearPublished(2010);
    b->setAuthor("Lily");
    objectSaved = b->save();
    b->isSaved();
    //qDebug() << "id: " << b->id();
    delete b;
#endif
    //--------------<select>---------------
    {
        QList<FakeBook*> objectList;
        objectList = ObjectLoader::all<FakeBook>();
        qDebug() << "record count: " << objectList.count();

        foreach (FakeBook* b, objectList) {
            qDebug() << "index: " << objectList.indexOf(b)
                     << " id: " << b->id()
                     << " author: " << b->author()
                     << " yearPublished: " << b->yearPublished();
        }
        qDeleteAll(objectList);
        objectList.clear();
    }

    //---------------<update>---------------
    {
        QList<FakeBook*> objectList;
        objectList = ObjectLoader::all<FakeBook>();
        if(objectList.count() > 1) {
            FakeBook* b = objectList.at(0);
            b->setAuthor("Velson xxx1");
            b->setYearPublished(1989);
            bool objectSaved = b->save();

            b = objectList.at(1);
            b->setAuthor("Velson xxx2");
            b->setYearPublished(2019);
            objectSaved = b->save();
        }
        qDeleteAll(objectList);
        objectList.clear();
    }

    //--------------<select>---------------
    {
        QList<FakeBook*> objectList;
        objectList = ObjectLoader::all<FakeBook>();
        qDebug() << "record count: " << objectList.count();

        foreach (FakeBook* b, objectList) {
            qDebug() << "index: " << objectList.indexOf(b)
                     << " id: " << b->id()
                     << " author: " << b->author()
                     << " yearPublished: " << b->yearPublished();
        }
        qDeleteAll(objectList);
        objectList.clear();
    }

    //--------------<select>---------------

    delete_table();
    removeDatabase();

    return 0;
}
