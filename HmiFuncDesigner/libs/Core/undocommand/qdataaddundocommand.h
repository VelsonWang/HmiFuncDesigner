#ifndef QDATAADDUNDOCOMMAND_H
#define QDATAADDUNDOCOMMAND_H

#include "qbaseundocommand.h"
#include "../../shared/qdata.h"

#define DATA_ADD_UNDO_COMMAND_ID  1009

enum enDataAddType
{
    DAT_ADD,
    DAT_REMOVE,
    DAT_EMPTY
};

class CORELIB_EXPORT QDataAddUndoCommand: public QBaseUndoCommand
{
public:
    QDataAddUndoCommand(const QString &uuid,const tagDataInfo &data,int index,enDataAddType type,QUndoCommand* parent=0);

    void    redo();
    void    undo();

    int     id()const;
protected:
    void    add();
    void    remove();
protected:
    tagDataInfo     m_data;
    QString         m_group_uuid;
    enDataAddType   m_type;
    int             m_index;
};

#endif // QDATAADDUNDOCOMMAND_H
