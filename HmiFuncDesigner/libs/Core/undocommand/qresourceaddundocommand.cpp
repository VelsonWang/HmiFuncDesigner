#include "qresourceaddundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/resource/qresourcemanager.h"
#include "../../shared/qprojectcore.h"

QResourceAddUndoCommand::QResourceAddUndoCommand(const QByteArray &data,
                                                 const QString &name,
                                                 enResourceAddType type,
                                                 QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_type(type),
    m_name(name),
    m_data(data)
{
}

void QResourceAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type==RAT_ADD)
    {
        add();
    }
    else if(m_type==RAT_REMOVE)
    {
        remove();
    }
}

void QResourceAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type==RAT_ADD)
    {
        remove();
    }
    else if(m_type==RAT_REMOVE)
    {
        add();
    }
}

void QResourceAddUndoCommand::add()
{
    QSoftCore::getCore()->getProjectCore()->get_resource_manager()->addResource(m_name,m_data);
}

void QResourceAddUndoCommand::remove()
{
    QSoftCore::getCore()->getProjectCore()->get_resource_manager()->removeResource(m_name);
}

int QResourceAddUndoCommand::id()const
{
    return RESOURCE_ADD_UNDO_COMMAND_ID;
}
