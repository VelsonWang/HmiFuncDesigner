#ifndef ILOADER_H
#define ILOADER_H

#include <QDebug>

class XMLObject;

#define PROPERTY_TITLE "Property"
#define ATTR_EDITABLE "Editable"
#define ATTR_NEEDSAVE "Need_Save"
#define ATTR_VISIBLE  "Visible"
#define ATTR_CAN_SAME "Can_Same"
#define ATTR_RESET_ABLEABLE "Can_Reset"

class ILoader
{
public:
    virtual void fromObject(XMLObject* xml) = 0;
};

#endif // ILOADER_H
