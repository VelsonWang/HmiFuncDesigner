#ifndef IGRAPHPAGESAVELOAD_H
#define IGRAPHPAGESAVELOAD_H

#include "XMLObject.h"
#include "projdata_global.h"

class PROJDATASHARED_EXPORT IGraphPageSaveLoad
{
public:
    virtual ~IGraphPageSaveLoad() {}

public:
    virtual bool openFromXml(XMLObject *pXmlObj) = 0;
    virtual bool saveToXml(XMLObject *pXmlObj) = 0;
    virtual void getAllElementIDName(QStringList &szIDList) = 0;
    virtual void getAllGraphPageName(QStringList &szList) = 0;
};

#endif // IGRAPHPAGESAVELOAD_H
