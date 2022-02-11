#ifndef NEWOBJECTSAVESTATE_H
#define NEWOBJECTSAVESTATE_H

#include "objectsavestate.h"

class NewObjectSaveState : public ObjectSaveState
{
public:
    virtual ~NewObjectSaveState();

    virtual bool save(QPersistantObject* o);
};

#endif

