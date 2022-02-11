#include "qpropertychangedundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/host/qabstracthost.h"
#include "../../shared/property/qabstractproperty.h"
#include "../../shared/qprojectcore.h"

QPropertyChangedUndoCommand::QPropertyChangedUndoCommand(const QString &host_uuid,
        const QString &property_name,
        const QVariant &old_value,
        const QVariant &new_value,
        QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_host_uuid(host_uuid),
    m_property_name(property_name),
    m_old_value(old_value),
    m_new_value(new_value)
{
}

int QPropertyChangedUndoCommand::id() const
{
    return PROPERTY_UNDO_COMMAND;
}

bool QPropertyChangedUndoCommand::mergeWith(const QUndoCommand *other)
{
    QPropertyChangedUndoCommand* cmd = (QPropertyChangedUndoCommand*)other;

    if(childCount() != cmd->childCount() || m_property_name != cmd->m_property_name) {
        return false;
    }
    int c = childCount();
    QPropertyChangedUndoCommand *p1, *p2;
    for(int i = 0; i < c; i++) {
        p1 = (QPropertyChangedUndoCommand*)child(i);
        p2 = (QPropertyChangedUndoCommand*)cmd->child(i);
        if(p1->m_host_uuid != p2->m_host_uuid || p1->m_property_name != p2->m_property_name) {
            return false;
        }
    }

    for(int i = 0; i < c; i++) {
        p1 = (QPropertyChangedUndoCommand*)child(i);
        p2 = (QPropertyChangedUndoCommand*)cmd->child(i);
        p1->m_new_value = p2->m_new_value;
    }
    m_new_value = cmd->m_new_value;
    return true;
}

void QPropertyChangedUndoCommand::redo()
{
    QBaseUndoCommand::redo();
    QAbstractHost* h = QSoftCore::getCore()->getProjectCore()->getHostByUuid(m_host_uuid);
    if(h != NULL) {
        h->setPropertyValue(m_property_name, m_new_value);
    }
}

void QPropertyChangedUndoCommand::undo()
{
    QBaseUndoCommand::undo();

    QAbstractHost* h = QSoftCore::getCore()->getProjectCore()->getHostByUuid(m_host_uuid);
    if(h != NULL) {
        h->setPropertyValue(m_property_name, m_old_value);
    }
}
