#ifndef QPERSISTANTOBJECT_P_H
#define QPERSISTANTOBJECT_P_H


class QPersistantObjectPrivate
{
public:
    QPersistantObjectPrivate();
    virtual ~QPersistantObjectPrivate();

    void setObjectSaveState(ObjectSaveState* newOss);
    bool isSaved() const;
    bool save(QPersistantObject* p);


    int id = -1;
    ObjectSaveState* oss;
};

#endif


