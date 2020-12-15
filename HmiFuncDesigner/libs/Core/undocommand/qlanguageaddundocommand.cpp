#include "qlanguageaddundocommand.h"

#include "../../shared/qlanguage.h"
#include "../../shared/qprojectcore.h"
#include "../qsoftcore.h"
#include "../../shared/qlanguagemanager.h"

QLanguageAddUndoCommand::QLanguageAddUndoCommand(QLanguage *l,int index, enLanguageAddType type, QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_type(type),
    m_index(index)
{
    if(l!=NULL)
    {
        m_language=new QLanguage;
        *m_language=*l;
    }
    else
    {
        m_language=NULL;
    }
}

QLanguageAddUndoCommand::~QLanguageAddUndoCommand()
{
    if(m_language!=NULL)
    {
        delete m_language;
        m_language=NULL;
    }
}

void QLanguageAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type==LAT_ADD)
    {
        add();
    }
    else if(m_type==LAT_REMOVE)
    {
        remove();
    }
    else
    {
        QSoftCore::getCore()->getProjectCore()->getLanguageManager()->emit_refresh(NULL);
    }
}

void QLanguageAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type==LAT_ADD)
    {
        remove();
    }
    else if(m_type==LAT_REMOVE)
    {
        add();
    }
    else
    {
        QSoftCore::getCore()->getProjectCore()->getLanguageManager()->emit_refresh(NULL);
    }
}

void QLanguageAddUndoCommand::add()
{
    QLanguageManager *manager=QSoftCore::getCore()->getProjectCore()->getLanguageManager();
    QLanguage *l=new QLanguage;
    *l=*m_language;
    manager->insert_language(l,m_index);
}

void QLanguageAddUndoCommand::remove()
{
    QLanguageManager *manager=QSoftCore::getCore()->getProjectCore()->getLanguageManager();
    manager->remove_language(m_language->getUuid());
}

int QLanguageAddUndoCommand::id()const
{
    return LANGUAGE_ADD_UNDO_COMMAND;
}

