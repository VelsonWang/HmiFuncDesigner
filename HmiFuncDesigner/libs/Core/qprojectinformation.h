#ifndef QPROJECTINFORMATION_H
#define QPROJECTINFORMATION_H

#include "corelibglobal.h"

#include <QObject>
#include <QMap>
#include <QVariant>

struct tagProperty {
    QString m_uuid;
    QString m_value;
    QString m_name;
};

struct tagProjectDataInfo {
    QString m_uuid;
    QString m_group;
    QString m_name;
    QVariant m_value;
};

class CORELIB_EXPORT QProjectInformation : public QObject
{
    Q_OBJECT
public:
    explicit QProjectInformation(QObject *parent = 0);
    ~QProjectInformation();

    tagProperty* getProperty(const QString &uuid);
    QList<tagProperty*> getPropertys();
    void addProperty(tagProperty* property);

    tagProjectDataInfo* getProjectData(const QString &uuid);
    QList<tagProjectDataInfo*> getProjectDatas();
    void addProjectData(tagProjectDataInfo* project_data);

signals:

public slots:
protected:
    QList<tagProperty*> m_propertys;
    QMap<QString, tagProperty*> m_uuidToProperty;

    QList<tagProjectDataInfo*> m_projectDatas;
    QMap<QString, tagProjectDataInfo*> m_uuidToProjectData;
};

#endif // QPROJECTINFORMATION_H
