#ifndef TAGSYS_H
#define TAGSYS_H

#include <QString>
#include <QStringList>
#include <QList>
#include "DBTag.h"
#include "XMLObject.h"
#include "projdata_global.h"


class PROJDATASHARED_EXPORT TagSys
{
public:
    explicit TagSys();
    ~TagSys();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

public:
    QList<TagSysDBItem *> listTagSysDBItem_;
};

#endif // TAGSYS_H
