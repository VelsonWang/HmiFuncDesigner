#include "qdatapropertychanged.h"

#include "../qsoftcore.h"

#include "../../shared/qprojectcore.h"
#include "../../shared/qdatamanager.h"
#include "../../shared/qdata.h"

QDataPropertyChanged::QDataPropertyChanged(QString property, const QString &uuid, const QVariant &old, const QVariant &now, QUndoCommand *parent):
    QBaseUndoCommand(parent),
    m_property_name(property),
    m_data_uuid(uuid),
    m_old_value(old),
    m_new_value(now)
{
}

int QDataPropertyChanged::id()const
{
    return DATA_PROPERTY_UNDO_COMMAND_ID;
}

void QDataPropertyChanged::redo()
{
    QBaseUndoCommand::redo();

    tagDataInfo *data=QSoftCore::getCore()->getProjectCore()->get_data_manager()->get_data(m_data_uuid);
    if(data!=NULL)
    {
        if(m_property_name=="name")
        {
            data->m_name=m_new_value.toString();
        }
        else if(m_property_name=="value")
        {
            data->m_value=m_new_value;
        }
        else if(m_property_name=="hold")
        {
            data->m_hold=m_new_value.toBool();
        }
        else if(m_property_name=="save")
        {
            data->m_save_time=m_new_value.toInt();
        }
        else if(m_property_name=="information")
        {
            data->m_information=m_new_value.toString();
        }
        data->m_data->emit_refresh(data);
    }
}

void QDataPropertyChanged::undo()
{
    QBaseUndoCommand::undo();

    tagDataInfo *data=QSoftCore::getCore()->getProjectCore()->get_data_manager()->get_data(m_data_uuid);
    if(data!=NULL)
    {
        if(m_property_name=="name")
        {
            data->m_name=m_old_value.toString();
        }
        else if(m_property_name=="value")
        {
            data->m_value=m_old_value;
        }
        else if(m_property_name=="hold")
        {
            data->m_hold=m_old_value.toBool();
        }
        else if(m_property_name=="save")
        {
            data->m_save_time=m_old_value.toInt();
        }
        else if(m_property_name=="information")
        {
            data->m_information=m_old_value.toString();
        }
        data->m_data->emit_refresh(data);
    }
}

bool QDataPropertyChanged::mergeWith(const QUndoCommand *other)
{
    QDataPropertyChanged* cmd=(QDataPropertyChanged*)other;
    if(cmd->m_data_uuid==m_data_uuid && cmd->m_property_name==m_property_name)
    {
        m_new_value=cmd->m_new_value;
        return true;
    }
    return false;
}
