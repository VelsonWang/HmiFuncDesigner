#ifndef TAGIO_H
#define TAGIO_H

#include <QString>
#include <QStringList>
#include <QList>
#include "DBTag.h"
#include "XMLObject.h"

class TagIO
{
public:
    explicit TagIO();
    ~TagIO();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    TagIODBItem *getTagIODBItemByID(const QString &id);
    TagIODBItem *getTagIODBItemByName(const QString &name);

public:
    QList<TagIODBItem *> listTagIODBItem_;
};


class TagIOGroup
{
public:
    explicit TagIOGroup();
    ~TagIOGroup();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    QString getGroupNameByShowName(const QString &szShowName);
    TagIOGroupDBItem *getGroupObjByShowName(const QString &szShowName);

    int getGroupCount();
    int getGroupCountByShowName(const QString &szShowName);

public:
    QList<TagIOGroupDBItem *> listTagIOGroupDBItem_;
};

#endif // TAGIO_H
