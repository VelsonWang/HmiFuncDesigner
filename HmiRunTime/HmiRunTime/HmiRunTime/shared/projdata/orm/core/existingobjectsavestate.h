#ifndef EXISTINGOBJECTSAVESTATE_H
#define EXISTINGOBJECTSAVESTATE_H

#include "objectsavestate.h"

class ExistingObjectSaveState : public ObjectSaveState
{
public:
    virtual ~ExistingObjectSaveState();

    virtual bool save(QPersistantObject* o);
};


#endif



