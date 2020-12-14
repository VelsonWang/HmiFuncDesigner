#include "quseraddundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qusermanager.h"

QUserAddUndoCommand::QUserAddUndoCommand(tagUserInfo *user, enUserAddType type,int index, QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_type(type),
    m_index(index)
{
    if(user!=NULL)
    {
        m_user_info.m_name=user->m_name;
        m_user_info.m_level=user->m_level;
        m_user_info.m_password=user->m_password;
        m_user_info.m_information=user->m_information;
        m_user_info.m_uuid=user->m_uuid;
    }
}

int QUserAddUndoCommand::id()const
{
    return USER_ADD_UNDO_COMMAND_ID;
}

void QUserAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type==UAT_ADD)
    {
        add();
    }
    else if(m_type==UAT_REMOVE)
    {
        remove();
    }
    else if(m_type==UAT_EMPTY)
    {
        QSoftCore::getCore()->getProjectCore()->get_user_manager()->emit_refresh(NULL);
    }

}

void QUserAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type==UAT_ADD)
    {
        remove();
    }
    else if(m_type==UAT_REMOVE)
    {
        add();
    }
    else if(m_type==UAT_EMPTY)
    {
        QSoftCore::getCore()->getProjectCore()->get_user_manager()->emit_refresh(NULL);
    }

}

void QUserAddUndoCommand::add()
{
    QUserManager *manager=QSoftCore::getCore()->getProjectCore()->get_user_manager();
    manager->insert_user(m_user_info,m_index);
}

void QUserAddUndoCommand::remove()
{
    QUserManager *manager=QSoftCore::getCore()->getProjectCore()->get_user_manager();
    manager->remove_user(m_user_info.m_uuid);
}
