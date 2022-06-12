#include "qpageaddundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qpagemanager.h"
#include "../../shared/host/qabstracthost.h"

QPageAddUndoCommand::QPageAddUndoCommand(QAbstractHost *page,
                                         int index,
                                         enPageAddType type,
                                         QUndoCommand *parent) :
    QBaseUndoCommand(parent),
    m_page(page),
    m_type(type),
    m_index(index)

{
}

QPageAddUndoCommand::~QPageAddUndoCommand()
{
    if(m_type == PAT_ADD) {
        m_page->deleteLater();
    }
}

int QPageAddUndoCommand::id() const
{
    return PAGE_ADD_UNDO_COMMAND_ID;
}

void QPageAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type == PAT_ADD) {
        add();
    } else if(m_type == PAT_REMOVE) {
        remove();
    }
}

void QPageAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type == PAT_ADD) {
        remove();
    } else if(m_type == PAT_REMOVE) {
        add();
    }
}

void QPageAddUndoCommand::add()
{
    QPageManager *manager = QSoftCore::getCore()->getProjectCore()->getPageManager();
    manager->insert_page(m_page, m_index);
}

void QPageAddUndoCommand::remove()
{
    QPageManager *manager = QSoftCore::getCore()->getProjectCore()->getPageManager();
    manager->remove_page(m_page);
}
