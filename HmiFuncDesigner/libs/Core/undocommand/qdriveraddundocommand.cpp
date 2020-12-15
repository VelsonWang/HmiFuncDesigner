#include "qdriveraddundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qdrivermanager.h"
#include "../../shared/driver/qabstractdriver.h"
#include "../../shared/qprojectcore.h"

QDriverAddUndoCommand::QDriverAddUndoCommand(QAbstractDriver *driver,
                                             int index,
                                             enDriverAddType type,
                                             QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_driver(driver),
    m_type(type),
    m_index(index)
{
}

QDriverAddUndoCommand::~QDriverAddUndoCommand()
{
    if(m_type==DRAT_ADD)
    {
        m_driver->deleteLater();
    }
}

int QDriverAddUndoCommand::id() const
{
    return DRIVER_ADD_UNDO_COMMAND_ID;
}

void QDriverAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();

    if(m_type==DRAT_ADD)
    {
        add();
    }
    else if(m_type==DRAT_REMOVE)
    {
        remove();
    }
}

void QDriverAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();

    if(m_type==DRAT_ADD)
    {
        remove();
    }
    else if(m_type==DRAT_REMOVE)
    {
        add();
    }
}

void QDriverAddUndoCommand::add()
{
    QDriverManager *manager=QSoftCore::getCore()->getProjectCore()->get_driver_manager();

    manager->insert_driver(m_index,m_driver);
}

void QDriverAddUndoCommand::remove()
{
    QDriverManager *manager=QSoftCore::getCore()->getProjectCore()->get_driver_manager();
    manager->remove_driver(m_driver->getUuid());
}
