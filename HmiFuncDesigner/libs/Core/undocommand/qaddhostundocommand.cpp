#include "qaddhostundocommand.h"

#include "../../shared/host/qabstracthost.h"

QAddHostUndoCommand::QAddHostUndoCommand(QAbstractHost *parent_host,
                                         const QList<QAbstractHost *> &hosts,
                                         const QList<int> &indexs,
                                         enAddHostType type,
                                         QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_parent_host(parent_host),
    m_hosts(hosts),
    m_indexs(indexs),
    m_type(type)
{
}

QAddHostUndoCommand::~QAddHostUndoCommand()
{
    if(m_type==AHT_ADD)
    {
        qDeleteAll(m_hosts);
    }
}

int QAddHostUndoCommand::id()const
{
    return HOST_ADD_UNDO_COMMAND;
}

void QAddHostUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    if(m_type==AHT_ADD)
    {
        add();
    }
    else if(m_type==AHT_REMOVE)
    {
        remove();
    }
}

void QAddHostUndoCommand::undo()
{
    QBaseUndoCommand::undo();
    if(m_type==AHT_ADD)
    {
        remove();
    }
    else if(m_type==AHT_REMOVE)
    {
        add();
    }
}

void QAddHostUndoCommand::add()
{
    if(m_parent_host!=Q_NULLPTR)
    {
        m_parent_host->insertChildren(m_indexs,m_hosts);
    }
}

void QAddHostUndoCommand::remove()
{
    if(m_parent_host!=Q_NULLPTR)
    {
        m_parent_host->removeChildren(m_hosts);
    }
}
