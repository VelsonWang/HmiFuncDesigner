#ifndef PICTURERESOURCEMANAGER_H
#define PICTURERESOURCEMANAGER_H

#include <QString>

class ProjectDataSQLiteDatabase;

class PictureResourceManager
{
public:
    PictureResourceManager();

    bool add(ProjectDataSQLiteDatabase *pDB, const QString &name, const QString &path);
    bool del(ProjectDataSQLiteDatabase *pDB, const QString &name);

};

#endif // PICTURERESOURCEMANAGER_H
