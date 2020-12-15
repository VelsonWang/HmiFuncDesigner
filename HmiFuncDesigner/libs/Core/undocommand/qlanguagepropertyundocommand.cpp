#include "qlanguagepropertyundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qlanguagemanager.h"
#include "../../shared/qlanguage.h"

QLanguagePropertyUndoCommand::QLanguagePropertyUndoCommand(QString property, const QString &uuid, const QVariant &old, const QVariant &now, QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_property_name(property),
    m_language_uuid(uuid),
    m_old_value(old),
    m_new_value(now)
{
}

int QLanguagePropertyUndoCommand::id()const
{
    return LANGUAGE_PROPERTY_UNDO_COMMAND_ID;
}

void QLanguagePropertyUndoCommand::redo()
{
    QLanguageManager *manager=QSoftCore::getCore()->getProjectCore()->getLanguageManager();
    QLanguage *temp=manager->get_language(m_language_uuid);
    if(temp!=NULL)
    {
        if(m_property_name=="languageName")
        {
            temp->set_language_name(m_new_value.toString());
        }
        if(m_property_name=="information")
        {
            temp->set_information(m_new_value.toString());
        }
        manager->emit_refresh(temp);
    }
    QBaseUndoCommand::redo();
}

void QLanguagePropertyUndoCommand::undo()
{
    QLanguageManager *manager=QSoftCore::getCore()->getProjectCore()->getLanguageManager();
    QLanguage *temp=manager->get_language(m_language_uuid);
    if(temp!=NULL)
    {
        if(m_property_name=="languageName")
        {
            temp->set_language_name(m_old_value.toString());
        }
        if(m_property_name=="information")
        {
            temp->set_information(m_old_value.toString());
        }
        manager->emit_refresh(temp);
    }
    QBaseUndoCommand::undo();
}
