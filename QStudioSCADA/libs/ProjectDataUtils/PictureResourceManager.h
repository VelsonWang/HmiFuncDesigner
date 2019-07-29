#ifndef PICTURERESOURCEMANAGER_H
#define PICTURERESOURCEMANAGER_H

#include <QString>

class ProjectDataSQLiteDatabase;

class PictureResourceManager
{
public:
    PictureResourceManager();

    bool add(ProjectDataSQLiteDatabase *pDB, const QString &name);
    bool del(ProjectDataSQLiteDatabase *pDB, const QString &name);

private:
    QString szTableName_ = "t_picture_resource_info";
};

#endif // PICTURERESOURCEMANAGER_H
