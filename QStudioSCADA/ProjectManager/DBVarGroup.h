#ifndef DBVARGROUP_H
#define DBVARGROUP_H

#include "../Public/Public.h"
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class DBVarGroup
{
public:
    DBVarGroup();

public:
    QString m_type;
    QString m_name;
    qint32 m_iPageID;
};

class DBVarGroups
{
public:
    DBVarGroups(QString projName);

private:
    void load(const QJsonObject &json);
    void save(QJsonObject &json);

public:
    bool loadFromFile(SaveFormat saveFormat);
    bool saveToFile(SaveFormat saveFormat);

private:
    QString m_strProjectName;
    QString m_strProjectPath;

public:
    QList<DBVarGroup *> m_VarBlockGroupList;
};


#endif // DBVARGROUP_H
