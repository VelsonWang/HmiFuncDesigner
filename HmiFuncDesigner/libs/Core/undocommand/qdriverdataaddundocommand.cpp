#include "qdriverdataaddundocommand.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qdrivermanager.h"
#include "../../shared/driver/qabstractdriver.h"

QDriverDataAddUndoCommand::QDriverDataAddUndoCommand(const QString &uuid,
                                                     const tagDriverDataInfo &data,
                                                     int index,
                                                     enDriverDataAddType type,
                                                     QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_driver_uuid(uuid),
    m_type(type),
    m_index(index)
{
    m_data.m_data_address=data.m_data_address;
    m_data.m_data_uuid=data.m_data_uuid;
    m_data.m_information=data.m_information;
    m_data.m_level=data.m_level;
    m_data.m_name=data.m_name;
    m_data.m_period=data.m_period;
    m_data.m_temp_data=data.m_temp_data;
    m_data.m_uuid=data.m_uuid;
    m_data.m_scale=data.m_scale;
    m_data.m_type=data.m_type;
}

void QDriverDataAddUndoCommand::redo()
{
    QBaseUndoCommand::redo();

    if(m_type==DDAT_ADD)
    {
        add();
    }
    else if(m_type==DDAT_REMOVE)
    {
        remove();
    }
}

void QDriverDataAddUndoCommand::undo()
{
    QBaseUndoCommand::undo();

    if(m_type==DDAT_ADD)
    {
        remove();
    }
    else if(m_type==DDAT_REMOVE)
    {
        add();
    }
}

int QDriverDataAddUndoCommand::id()const
{
    return DRIVER_DATA_ADD_UNDO_COMMAND_ID;
}

void QDriverDataAddUndoCommand::add()
{
    QAbstractDriver *driver=QSoftCore::getCore()->getProjectCore()->get_driver_manager()
            ->get_driver(m_driver_uuid);

    if(driver!=NULL)
    {
        driver->insert_data(m_data,m_index);
    }
}

void QDriverDataAddUndoCommand::remove()
{
    QAbstractDriver *driver=QSoftCore::getCore()->getProjectCore()->get_driver_manager()
            ->get_driver(m_driver_uuid);

    if(driver!=NULL)
    {
        driver->remove_data(m_data.m_uuid);
    }
}
