#ifndef TAGIO_H
#define TAGIO_H

#include <QString>
#include <QStringList>
#include <QList>
#include "Tag.h"

class ProjectDataSQLiteDatabase;

class TagIO
{
public:
    explicit TagIO();
    ~TagIO();

    bool load(ProjectDataSQLiteDatabase *pDB);
    bool save(ProjectDataSQLiteDatabase *pDB);

    bool insert(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);
    bool insert(ProjectDataSQLiteDatabase *pDB, QList<TagIODBItem *> &pObjs);
    bool del(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);
    bool del(ProjectDataSQLiteDatabase *pDB, const QString &id);
    bool delAll(ProjectDataSQLiteDatabase *pDB);
    bool update(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);

    TagIODBItem *getTagIODBItemByID(const QString &id);
    TagIODBItem *getTagIODBItemByName(const QString &name);
    int getLastInsertId(ProjectDataSQLiteDatabase *pDB);

    bool saveTagIODBItem(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);

public:
    QList<TagIODBItem *> listTagIODBItem_;

};


class TagIOGroup
{
public:
    explicit TagIOGroup();
    ~TagIOGroup();

    bool load(ProjectDataSQLiteDatabase *pDB);
    bool save(ProjectDataSQLiteDatabase *pDB);

    bool insert(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj);
    bool del(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj);
    bool update(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj);

    int getLastInsertId(ProjectDataSQLiteDatabase *pDB);

    bool saveTagTmpDBItem(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj);

    QString getGroupNameByShowName(ProjectDataSQLiteDatabase *pDB, const QString &name);
    TagIOGroupDBItem *getGroupObjByShowName(ProjectDataSQLiteDatabase *pDB, const QString &name);

    int getGroupCount(ProjectDataSQLiteDatabase *pDB);
    int getGroupCountByShowName(ProjectDataSQLiteDatabase *pDB, const QString &name);

public:
    QList<TagIOGroupDBItem *> listTagIOGroupDBItem_;

};

#endif // TAGIO_H
