#include "quserpropertyundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qusermanager.h"

QUserPropertyUndoCommand::QUserPropertyUndoCommand(QString property, const QString &uuid, const QVariant &old, const QVariant &now, QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_property_name(property),
    m_user_uuid(uuid),
    m_old_value(old),
    m_new_value(now)
{
}

int QUserPropertyUndoCommand::id()const
{
    return USER_PROPERTY_UNDO_COMMAND_ID;
}

void QUserPropertyUndoCommand::redo()
{
    QUserManager *manager=QSoftCore::getCore()->getProjectCore()->get_user_manager();
    tagUserInfo *temp=manager->get_user(m_user_uuid);
    if(temp!=NULL)
    {
        if(m_property_name=="userName")
        {
            temp->m_name=m_new_value.toString();
        }
        if(m_property_name=="level")
        {
            temp->m_level=m_new_value.toInt();
        }
        else if(m_property_name=="password")
        {
            temp->m_password=m_new_value.toString();
        }
        else if(m_property_name=="information")
        {
            temp->m_information=m_new_value.toString();
        }
        manager->emit_refresh(temp);
    }
    QBaseUndoCommand::redo();
}

void QUserPropertyUndoCommand::undo()
{
    QUserManager *manager=QSoftCore::getCore()->getProjectCore()->get_user_manager();
    tagUserInfo *temp=manager->get_user(m_user_uuid);
    if(temp!=NULL)
    {
        if(m_property_name=="userName")
        {
            temp->m_name=m_old_value.toString();
        }
        if(m_property_name=="level")
        {
            temp->m_level=m_old_value.toInt();
        }
        else if(m_property_name=="password")
        {
            temp->m_password=m_old_value.toString();
        }
        else if(m_property_name=="information")
        {
            temp->m_information=m_old_value.toString();
        }
        manager->emit_refresh(temp);
    }
    QBaseUndoCommand::undo();
}
