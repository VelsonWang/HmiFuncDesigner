#include "qdataaddundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qdata.h"
#include "../../shared/qdatamanager.h"

QDataAddUndoCommand::QDataAddUndoCommand(const QString &uuid,
                                         const tagDataInfo &data,
                                         int    index,
                                         enDataAddType type,
                                         QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_group_uuid(uuid),
    m_type(type),
    m_index(index)
{
    m_data.m_name=data.m_name;
    m_data.m_uuid=data.m_uuid;
    m_data.m_hold=data.m_hold;
    m_data.m_type=data.m_type;
    m_data.m_information=data.m_information;
    m_data.m_save_time=data.m_save_time;
    m_data.m_value=data.m_value;
}

int QDataAddUndoCommand::id()const
{
    return DATA_ADD_UNDO_COMMAND_ID;
}

void QDataAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type==DAT_ADD)
    {
        add();
    }
    else if(m_type==DAT_REMOVE)
    {
        remove();
    }
}

void QDataAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type==DAT_ADD)
    {
        remove();
    }
    else if(m_type==DAT_REMOVE)
    {
        add();
    }
}

void QDataAddUndoCommand::add()
{
    QData *data=QSoftCore::getCore()->getProjectCore()->get_data_manager()->get_data_group(m_group_uuid);
    if(data!=NULL)
    {
        data->insert_data(m_data,m_index);
    }
}

void QDataAddUndoCommand::remove()
{
    QData *data=QSoftCore::getCore()->getProjectCore()->get_data_manager()->get_data_group(m_group_uuid);
    if(data!=NULL)
    {
        data->remove_data(m_data.m_uuid);
    }
}
