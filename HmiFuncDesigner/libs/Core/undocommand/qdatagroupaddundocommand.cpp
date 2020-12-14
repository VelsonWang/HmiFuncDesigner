#include "qdatagroupaddundocommand.h"

#include "qdataaddundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qdatamanager.h"

QDataGroupAddUndoCommand::QDataGroupAddUndoCommand(const QData &data, int index, enDataGroupAddType type, QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_type(type),
    m_index(index)
{
    m_data.copy(data);
}

void QDataGroupAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type==DGAT_ADD)
    {
        add();
    }
    else if(m_type==DGAT_REMOVE)
    {
        remove();
    }
}

void QDataGroupAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type==DGAT_ADD)
    {
        remove();
    }
    else if(m_type==DGAT_REMOVE)
    {
        add();
    }
}

int QDataGroupAddUndoCommand::id()const
{
    return DATA_GROUP_ADD_UNDO_COMMAND_ID;
}

void QDataGroupAddUndoCommand::add()
{
    QSoftCore::getCore()->getProjectCore()->get_data_manager()->insert_group(m_data,m_index);
}

void QDataGroupAddUndoCommand::remove()
{
    QSoftCore::getCore()->getProjectCore()->get_data_manager()->remove_group(m_data.get_uuid());
}
