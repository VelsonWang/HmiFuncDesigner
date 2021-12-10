#ifndef QFILEMANAGER_H
#define QFILEMANAGER_H

#include "corelibglobal.h"
#include <QObject>
#include <QMap>

#define FILE_SYSTEM     "system"
#define FILE_USER       "user"

struct tagFileInfo {
    QString m_file_name;
    QString m_uuid;
    QString m_type;
    QByteArray m_file_data;
    QString m_group_uuid;
    QString m_exp;
};

struct tagFileGroupInfo {
    QString m_group_name;
    QString m_uuid;
    QList<tagFileInfo*> m_files;
    QMap<QString, tagFileInfo*> m_uuid_to_file;
};

class CORELIB_EXPORT QFileManager : public QObject
{
    Q_OBJECT
public:
    explicit QFileManager(QObject *parent = 0);
    ~QFileManager();

    void load();
    void save(const QString &uuid);
    void clear();
    void rename(const QString &uuid, const QString &new_group_name);

    void insertGroup(tagFileGroupInfo *group, int index = -1);
    void removeGroup(const QString &uuid);

    void insertFile(const QString &group_uuid, tagFileInfo *info, int index = -1);
    void removeFile(const QString &group_uuid, const QString &file_uuid);

    void changedFile(const QString &group_uuid, tagFileInfo *info);

    tagFileGroupInfo *getGroup(const QString &group_uuid);
    tagFileInfo *getFile(const QString &file_uuid);
    QList<tagFileGroupInfo*> getAllGroup();

protected:
    void load(const QString &file);
    void save_group_list();

signals:
    void sigInsertGroup(tagFileGroupInfo *group, int index);
    void sigRemoveGroup(tagFileGroupInfo* group);
    void sigInsertFile(tagFileGroupInfo *group, tagFileInfo* file, int index);
    void sigRemoveFile(tagFileGroupInfo* group, tagFileInfo* file);
    void refresh(tagFileGroupInfo* group, tagFileInfo* file);

protected:
    QList<tagFileGroupInfo*> m_groups;
    QMap<QString, tagFileGroupInfo*> m_uuidToGroup;
};

#endif // QFILEMANAGER_H
