#include "qprojectinformation.h"

QProjectInformation::QProjectInformation(QObject *parent) :
    QObject(parent)
{
}

QProjectInformation::~QProjectInformation()
{
    qDeleteAll(m_propertys);
    m_propertys.clear();
    m_uuid_to_property.clear();
}

tagProperty* QProjectInformation::get_property(const QString &uuid)
{
    return m_uuid_to_property.value(uuid);
}

QList<tagProperty*> QProjectInformation::get_propertys()
{
    return m_propertys;
}

void QProjectInformation::add_property(tagProperty *property)
{
    if(m_uuid_to_property.value(property->m_uuid)!=NULL)
    {
        delete property;
        return;
    }
    m_propertys.append(property);
    m_uuid_to_property.insert(property->m_uuid,property);
}

tagProjectDataInfo* QProjectInformation::get_project_data(const QString &uuid)
{
    return m_uuid_to_project_data.value(uuid);
}

QList<tagProjectDataInfo*> QProjectInformation::get_project_datas()
{
    return m_project_datas;
}

void QProjectInformation::add_project_data(tagProjectDataInfo *project_data)
{
    if(m_uuid_to_project_data.value(project_data->m_uuid)!=NULL)
    {
        delete project_data;
        return;
    }
    m_project_datas.append(project_data);
    m_uuid_to_project_data.insert(project_data->m_uuid,project_data);
}
