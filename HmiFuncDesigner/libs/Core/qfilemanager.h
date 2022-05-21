#ifndef QFILEMANAGER_H
#define QFILEMANAGER_H

#include "corelibglobal.h"
#include <QObject>
#include <QMap>

#define FILE_SYSTEM     "system"
#define FILE_USER       "user"

struct tagFileInfo {
    QString m_file_name;
    QString m_id;
    QString m_type;
    QByteArray m_file_data;
    QString m_group_id;
    QString m_exp;
};

struct tagFileGroupInfo {
    QString m_group_name;
    QString m_id;
    QList<tagFileInfo*> m_files;
    QMap<QString, tagFileInfo*> m_id_to_file;
};

class CORELIB_EXPORT QFileManager : public QObject
{
    Q_OBJECT
public:
    explicit QFileManager(QObject *parent = 0);
    ~QFileManager();

    void load();
    void save(const QString &id);
    void clear();
    void rename(const QString &id, const QString &new_group_name);

    void insertGroup(tagFileGroupInfo *group, int index = -1);
    void removeGroup(const QString &id);

    void insertFile(const QString &group_id, tagFileInfo *info, int index = -1);
    void removeFile(const QString &group_id, const QString &file_id);

    void changedFile(const QString &group_id, tagFileInfo *info);

    tagFileGroupInfo *getGroup(const QString &group_id);
    tagFileInfo *getFile(const QString &file_id);
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
    QMap<QString, tagFileGroupInfo*> m_idToGroup;
};

#endif // QFILEMANAGER_H
