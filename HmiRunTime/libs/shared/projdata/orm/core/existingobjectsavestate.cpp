#include "existingobjectsavestate.h"

#include <QtSql/QtSql>

#include "introspection.h"
#include "qpersistantobject.h"
#include "inflection.h"

ExistingObjectSaveState::~ExistingObjectSaveState()
{

}

bool ExistingObjectSaveState::save(QPersistantObject* o)
{
    QSqlDatabase db = QSqlDatabase::database("qtp-database");
    if (!db.isOpen()) {
        return false;
    }

    QString tableName = tableNameForObject(o);
    QList<QString> pList = propertyListForObject(o);
    QVariantHash propertyValues = propertyValuesForObject(o);

    QString queryString = QString("UPDATE %1 SET ").arg(tableName);

    for (int i = 0; i < pList.count(); i++) {
        queryString += Utils::snakeCase(pList.at(i));
        queryString += " = ";
        queryString += "?, ";
        if (i == pList.count() - 1) {
            queryString.chop(2);
        }
    }

    queryString += " WHERE id = ";
    queryString += QString::number(o->id());

    QSqlQuery query(db);
    query.prepare(queryString);

    Q_FOREACH(const QString& property, pList) {
        query.addBindValue(propertyValues.value(property));
    }

    bool querySuccessful = query.exec();
    if (!querySuccessful) {
        qWarning() << "Query unsuccessful! Object not saved\n"
                   << query.lastQuery() << "\n"
                   << query.lastError().text();
    }

    query.finish();
    return querySuccessful;
}
