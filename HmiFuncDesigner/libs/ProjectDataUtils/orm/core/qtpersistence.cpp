#include "qtpersistence.h"
#include "databaseconfiguration_p.h"

#include <QtCore/QUrl>

bool QtPersistence::useDatabase(const QUrl& databaseUrl)
{
    DatabaseConfiguration* db = DatabaseConfiguration::self();
    bool configOk = db->configureDatabase(databaseUrl);
    return configOk;
}


