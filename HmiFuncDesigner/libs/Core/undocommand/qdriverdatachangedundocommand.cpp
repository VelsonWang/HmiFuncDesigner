#include "qdriverdatachangedundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qdrivermanager.h"
#include "../../shared/driver/qabstractdriver.h"

QDriverDataChangedUndoCommand::QDriverDataChangedUndoCommand(const QString &driver_uuid,
                                                             const QString &data_uuid,
                                                             const QString &key,
                                                             const QVariant &old_value,
                                                             const QVariant &new_value,
                                                             QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_driver_uuid(driver_uuid),
    m_data_uuid(data_uuid),
    m_key(key),
    m_old_value(old_value),
    m_new_value(new_value)
{
}

int QDriverDataChangedUndoCommand::id()const
{
    return DRIVER_DATA_CHANGED_UNDO_COMMAND_ID;
}

void QDriverDataChangedUndoCommand::redo()
{
    QBaseUndoCommand::redo();

    QAbstractDriver* driver=QSoftCore::getCore()->getProjectCore()->get_driver_manager()
            ->get_driver(m_driver_uuid);

    if(driver!=NULL)
    {
        driver->changed_data(m_data_uuid,m_key,m_new_value);
    }
}

void QDriverDataChangedUndoCommand::undo()
{
    QBaseUndoCommand::undo();

    QAbstractDriver* driver=QSoftCore::getCore()->getProjectCore()->get_driver_manager()
            ->get_driver(m_driver_uuid);

    if(driver!=NULL)
    {
        driver->changed_data(m_data_uuid,m_key,m_old_value);
    }
}

bool QDriverDataChangedUndoCommand::mergeWith(const QUndoCommand *other)
{
    QDriverDataChangedUndoCommand *cmd=(QDriverDataChangedUndoCommand*)other;

    if(cmd->m_driver_uuid==m_driver_uuid && cmd->m_data_uuid==m_data_uuid
            && cmd->m_key==m_key)
    {
        m_new_value=cmd->m_new_value;
        return true;
    }
    else
    {
        return false;
    }
}
