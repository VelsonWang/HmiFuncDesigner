#include "qpersistantobject.h"
#include "objectsavestate.h"
#include "qpersistantobject_p.h"
#include "newobjectsavestate.h"
#include "existingobjectsavestate.h"

QPersistantObjectPrivate::QPersistantObjectPrivate()
    : id(0), oss(0)
{
}

QPersistantObjectPrivate::~QPersistantObjectPrivate()
{
    delete oss;
}


void QPersistantObjectPrivate::setObjectSaveState(ObjectSaveState* newOss)
{
    delete oss;
    oss = newOss;
}

bool QPersistantObjectPrivate::isSaved() const
{
    return id > 0;
}

bool QPersistantObjectPrivate::save(QPersistantObject* qpo)
{
    Q_ASSERT(oss);
    bool saveSuccessful = oss->save(qpo);
    return saveSuccessful;
}


/* begin public class implementation */

QPersistantObject::QPersistantObject(QObject* parent)
    : QObject(parent), d(new QPersistantObjectPrivate())
{

}

QPersistantObject::~QPersistantObject()
{
    delete d;
}

int QPersistantObject::id() const
{
    return d->id;
}

void QPersistantObject::setId(int id)
{
    d->id = id;
}

bool QPersistantObject::isSaved() const
{
    return d->isSaved();
}

bool QPersistantObject::save()
{
#if 0
    if (!isSaved())
    {
        NewObjectSaveState* noss = new NewObjectSaveState;
        d->setObjectSaveState(noss);
    }

    bool saveOk = d->save(this);
    if (saveOk)
    {
        ExistingObjectSaveState* eoss = new ExistingObjectSaveState;
        d->setObjectSaveState(eoss);
    }
#else
    if (isSaved())
    {
        ExistingObjectSaveState* eoss = new ExistingObjectSaveState;
        d->setObjectSaveState(eoss);
    }
    else {
        NewObjectSaveState* noss = new NewObjectSaveState;
        d->setObjectSaveState(noss);
    }
    bool saveOk = d->save(this);
#endif
    return saveOk;
}


