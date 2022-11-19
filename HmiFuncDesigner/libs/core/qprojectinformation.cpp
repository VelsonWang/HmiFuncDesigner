#include "qprojectinformation.h"

QProjectInformation::QProjectInformation(QObject *parent) :
    QObject(parent)
{
}

QProjectInformation::~QProjectInformation()
{
    qDeleteAll(m_propertys);
    m_propertys.clear();
    m_idToProperty.clear();
}

tagProperty* QProjectInformation::getProperty(const QString &id)
{
    return m_idToProperty.value(id);
}

QList<tagProperty*> QProjectInformation::getPropertys()
{
    return m_propertys;
}

void QProjectInformation::addProperty(tagProperty *property)
{
    if(m_idToProperty.value(property->m_id) != NULL) {
        delete property;
        return;
    }
    m_propertys.append(property);
    m_idToProperty.insert(property->m_id, property);
}

tagProjectDataInfo* QProjectInformation::getProjectData(const QString &id)
{
    return m_idToProjectData.value(id);
}

QList<tagProjectDataInfo*> QProjectInformation::getProjectDatas()
{
    return m_projectDatas;
}

void QProjectInformation::addProjectData(tagProjectDataInfo *project_data)
{
    if(m_idToProjectData.value(project_data->m_id) != NULL) {
        delete project_data;
        return;
    }
    m_projectDatas.append(project_data);
    m_idToProjectData.insert(project_data->m_id, project_data);
}
