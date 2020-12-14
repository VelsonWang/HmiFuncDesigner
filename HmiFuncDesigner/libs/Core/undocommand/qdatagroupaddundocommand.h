#ifndef QDATAGROUPADDUNDOCOMMAND_H
#define QDATAGROUPADDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../../shared/qdata.h"

#define DATA_GROUP_ADD_UNDO_COMMAND_ID  1010

enum enDataGroupAddType
{
    DGAT_ADD,
    DGAT_REMOVE,
    DGAT_EMPTY
};

class CORELIB_EXPORT QDataGroupAddUndoCommand: public QBaseUndoCommand
{
public:
    QDataGroupAddUndoCommand(const QData &data,int index,enDataGroupAddType type,QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;
protected:
    void    add();
    void    remove();
protected:
    enDataGroupAddType      m_type;
    int                     m_index;
    QData                   m_data;
};

#endif // QDATAGROUPADDUNDOCOMMAND_H
