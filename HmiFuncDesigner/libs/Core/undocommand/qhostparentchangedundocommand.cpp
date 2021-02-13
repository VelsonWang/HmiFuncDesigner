#include "qhostparentchangedundocommand.h"

#include "../qsoftcore.h"
#include "../../shared/qprojectcore.h"
#include "../../shared/host/qabstractbuttonhost.h"

QHostParentChangedUndoCommand::QHostParentChangedUndoCommand(QAbstractHost *host,
                                                             QAbstractHost *old_parent,
                                                             int old_index,
                                                             QAbstractHost *new_parent,
                                                             int new_index,
                                                             QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_host(host),
    m_old_parent(old_parent),
    m_old_parent_index(old_index),
    m_new_parent(new_parent),
    m_new_parent_index(new_index)
{
}

int QHostParentChangedUndoCommand::id() const
{
    return HOST_PARENT_CHANGED_UNDO_COMMAND;
}

void QHostParentChangedUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    m_host->setParent(m_new_parent, m_new_parent_index);
}

void QHostParentChangedUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    m_host->setParent(m_old_parent, m_old_parent_index);
}
