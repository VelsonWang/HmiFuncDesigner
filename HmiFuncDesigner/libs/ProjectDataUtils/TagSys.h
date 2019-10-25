#ifndef TAGSYS_H
#define TAGSYS_H

#include <QString>
#include <QStringList>
#include <QList>
#include "Tag.h"

class ProjectDataSQLiteDatabase;

class TagSys
{
public:
    explicit TagSys();
    ~TagSys();

    bool load(ProjectDataSQLiteDatabase *pDB);
    bool save(ProjectDataSQLiteDatabase *pDB);

    bool insert(ProjectDataSQLiteDatabase *pDB, TagSysDBItem *pObj);
    bool del(ProjectDataSQLiteDatabase *pDB, TagSysDBItem *pObj);
    bool delAll(ProjectDataSQLiteDatabase *pDB);
    bool update(ProjectDataSQLiteDatabase *pDB, TagSysDBItem *pObj);

    TagSysDBItem *getTagSysDBItemByID(const QString &id);
    TagSysDBItem *getTagSysDBItemByName(const QString &name);
    int getLastInsertId(ProjectDataSQLiteDatabase *pDB);

public:
    QList<TagSysDBItem *> listTagSysDBItem_;
};

#endif // TAGSYS_H
