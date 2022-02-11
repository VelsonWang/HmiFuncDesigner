#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include <QtSql/QtSql>

#include "introspection.h"
#include "inflection.h"


class ObjectLoader
{
public:
    ObjectLoader() {}
    ~ObjectLoader() {}

    template <typename T>
    static QList<T*> all();
};

template <typename T>
QList<T*> ObjectLoader::all()
{
    const QMetaObject o  = T::staticMetaObject;
    QString queryString = "SELECT ";

    QStringList propertyList;
    for (int i = 0; i < o.propertyCount(); ++i) {
        QMetaProperty property = o.property(i);
        QString propertyName = QString::fromLatin1(property.name());
        if (propertyName == "objectName") {
            continue;
        }
        propertyList.append(propertyName);
        queryString += Utils::snakeCase(propertyName);
        queryString += ", ";
    }

    queryString.chop(2);
    queryString += " FROM ";
    queryString += Utils::tableize(QString(o.className()));

    QSqlDatabase db = QSqlDatabase::database("qtp-database");
    QSqlQuery q(db);

    QList<T*> objectList;

    q.exec(queryString);
    while (q.next())
    {
        QSqlRecord record = q.record();
        T* object = new T;
        Q_FOREACH(const QString& prop, propertyList) {
            object->setProperty(prop.toLatin1().constData(),
                                record.value(Utils::snakeCase(prop)));
        }

        objectList.append(object);
    }


    return objectList;
}

#endif


