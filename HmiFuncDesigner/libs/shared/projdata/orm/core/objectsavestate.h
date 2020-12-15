#ifndef OBJECTSAVESTATE_H
#define OBJECTSAVESTATE_H

class QPersistantObject;

class ObjectSaveState
{
public:
    virtual ~ObjectSaveState() {}

    virtual bool save(QPersistantObject* o) = 0;

};


#endif




