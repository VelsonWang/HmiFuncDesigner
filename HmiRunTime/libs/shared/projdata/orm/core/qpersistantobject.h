#ifndef QPERSISTANTOBJECT_H
#define QPERSISTANTOBJECT_H

#include <QtCore/QObject>

class QPersistantObjectPrivate;

class QPersistantObject: public QObject
{
Q_OBJECT
Q_PROPERTY(int id READ id WRITE setId)
public:
    QPersistantObject (QObject* parent = 0);
    virtual ~QPersistantObject();

    int id() const;
    void setId(int);

    bool save();
    bool isSaved() const;

private:
    QPersistantObjectPrivate* const d;
};


#endif



