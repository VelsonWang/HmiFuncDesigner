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
    explicit TagIO(const QString &szTableName="");
    ~TagIO();

    bool load(ProjectDataSQLiteDatabase *pDB);
    bool save(ProjectDataSQLiteDatabase *pDB);

    bool insert(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);
    bool insert(ProjectDataSQLiteDatabase *pDB, QList<TagIODBItem *> &pObjs);
    bool del(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);
    bool delAll(ProjectDataSQLiteDatabase *pDB);
    bool update(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);

    TagIODBItem *getTagTmpDBItemByID(const QString &id);
    TagIODBItem *getTagTmpDBItemByName(const QString &name);
    int getLastInsertId(ProjectDataSQLiteDatabase *pDB);

    bool saveTagTmpDBItem(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj);

    QString getGroupNameByShowName(ProjectDataSQLiteDatabase *pDB, const QString &name);

public:
    QList<TagIODBItem *> listTagIODBItem_;

};

#endif // TAGIO_H
