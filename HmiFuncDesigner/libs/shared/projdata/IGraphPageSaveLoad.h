#ifndef IGRAPHPAGESAVELOAD_H
#define IGRAPHPAGESAVELOAD_H

#include "../xmlobject.h"
#include "sharedlibglobal.h"

class SHAREDLIB_EXPORT IGraphPageSaveLoad
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
