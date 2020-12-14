#ifndef QFILEGROUPADDUNDOCOMMAND_H
#define QFILEGROUPADDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../qfilemanager.h"

#define FILE_GROUP_ADD_UNDO_COMMAND_ID  1011

enum enFileGroupAddType
{
    FGAT_ADD,
    FGAT_REMOVE,
    FGAT_EMPTY
};

class CORELIB_EXPORT QFileGroupAddUndoCommand: public QBaseUndoCommand
{
public:
    QFileGroupAddUndoCommand(const tagFileGroupInfo &data,int index,enFileGroupAddType type,QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;
protected:
    void    add();
    void    remove();
protected:
    enFileGroupAddType      m_type;
    int                     m_index;
    tagFileGroupInfo        m_data;
};

#endif // QFILEGROUPADDUNDOCOMMAND_H
