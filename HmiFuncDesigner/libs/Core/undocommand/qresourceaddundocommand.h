#ifndef QRESOURCEADDUNDOCOMMAND_H
#define QRESOURCEADDUNDOCOMMAND_H

#include "qbaseundocommand.h"

#define RESOURCE_ADD_UNDO_COMMAND_ID  1013

enum enResourceAddType
{
    RAT_ADD,
    RAT_REMOVE,
    RAT_EMPTY
};

class CORELIB_EXPORT QResourceAddUndoCommand: public QBaseUndoCommand
{
public:
    QResourceAddUndoCommand(const QByteArray &data,const QString &name,enResourceAddType type,QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;
protected:
    void    add();
    void    remove();
protected:
    enResourceAddType   m_type;
    QString             m_name;
    QByteArray          m_data;
};

#endif // QRESOURCEADDUNDOCOMMAND_H
