#ifndef QTPERSISTENCE_H
#define QTPERSISTENCE_H

#include <QtCore/QString>
#include <QtCore/QList>

class QPersistantObject;

class QUrl;

namespace QtPersistence
{
    /**
     * Set up a specific database to use for the persistence layer
     *
     * Urls should be of the form:
     * \code
     * databaseSystem://user:pass@host.name/databaseName
     * \endcode
     *
     * For SQLite, to use an in-memory database, use ":memory:" as the
     * database name.
     */
    bool useDatabase(const QUrl& databaseUrl);

    template<typename T>
    static QList<QPersistantObject*> all()
    {
        return QList<QPersistantObject*>();
    }

};

#endif


