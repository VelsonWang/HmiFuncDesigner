#include "qprojectinformation.h"

QProjectInformation::QProjectInformation(QObject *parent) :
    QObject(parent)
{
}

QProjectInformation::~QProjectInformation()
{
    qDeleteAll(m_propertys);
    m_propertys.clear();
    m_uuidToProperty.clear();
}

tagProperty* QProjectInformation::getProperty(const QString &uuid)
{
    return m_uuidToProperty.value(uuid);
}

QList<tagProperty*> QProjectInformation::getPropertys()
{
    return m_propertys;
}

void QProjectInformation::addProperty(tagProperty *property)
{
    if(m_uuidToProperty.value(property->m_uuid) != NULL) {
        delete property;
        return;
    }
    m_propertys.append(property);
    m_uuidToProperty.insert(property->m_uuid, property);
}

tagProjectDataInfo* QProjectInformation::getProjectData(const QString &uuid)
{
    return m_uuidToProjectData.value(uuid);
}

QList<tagProjectDataInfo*> QProjectInformation::getProjectDatas()
{
    return m_projectDatas;
}

void QProjectInformation::addProjectData(tagProjectDataInfo *project_data)
{
    if(m_uuidToProjectData.value(project_data->m_uuid) != NULL) {
        delete project_data;
        return;
    }
    m_projectDatas.append(project_data);
    m_uuidToProjectData.insert(project_data->m_uuid, project_data);
}
