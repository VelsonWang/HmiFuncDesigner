#ifndef TAGTMP_H
#define TAGTMP_H

#include <QString>
#include <QStringList>
#include <QList>
#include "DBTag.h"
#include "XMLObject.h"
#include "projdata_global.h"

class PROJDATASHARED_EXPORT TagTmp
{
public:
    explicit TagTmp();
    ~TagTmp();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    TagTmpDBItem *getTagTmpDBItemByID(const QString &id);

public:
    QList<TagTmpDBItem *> listTagTmpDBItem_;
};

#endif // TAGTMP_H
