#ifndef IGRAPHPAGESAVELOAD_H
#define IGRAPHPAGESAVELOAD_H

#include "XMLObject.h"

class IGraphPageSaveLoad
{
public:
    virtual ~IGraphPageSaveLoad() {}

public:
    virtual bool openFromXml(XMLObject *pXmlObj) = 0;
    virtual bool saveToXml(XMLObject *pXmlObj) = 0;

};

#endif // IGRAPHPAGESAVELOAD_H
