#ifndef QDATAPROPERTYCHANGED_H
#define QDATAPROPERTYCHANGED_H

#include "qbaseundocommand.h"

#define DATA_PROPERTY_UNDO_COMMAND_ID  1008

class CORELIB_EXPORT QDataPropertyChanged: public QBaseUndoCommand
{
public:
    QDataPropertyChanged(QString property,const QString &uuid,const QVariant &old,
                             const QVariant &now,QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;

    bool    mergeWith(const QUndoCommand *other);
protected:
    QString     m_property_name;
    QString     m_data_uuid;
    QVariant    m_old_value;
    QVariant    m_new_value;
};

#endif // QDATAPROPERTYCHANGED_H
