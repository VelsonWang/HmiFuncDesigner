#ifndef QUSERUNDOCOMMAND_H
#define QUSERUNDOCOMMAND_H

#include "qbaseundocommand.h"

#include <QVariant>

#define USER_PROPERTY_UNDO_COMMAND_ID   1000

class CORELIB_EXPORT QUserPropertyUndoCommand: public QBaseUndoCommand
{
public:
    QUserPropertyUndoCommand(QString property,const QString &uuid,const QVariant &old,
                             const QVariant &now,QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;
protected:
    QString     m_property_name;
    QString     m_user_uuid;
    QVariant    m_old_value;
    QVariant    m_new_value;
};

#endif // QUSERUNDOCOMMAND_H
