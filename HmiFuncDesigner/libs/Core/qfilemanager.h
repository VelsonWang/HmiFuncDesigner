#ifndef QFILEMANAGER_H
#define QFILEMANAGER_H

#include "corelibglobal.h"

#include <QObject>
#include <QMap>

#define FILE_SYSTEM     "system"
#define FILE_USER       "user"

struct tagFileInfo
{
    QString     m_file_name;
    QString     m_uuid;
    QString     m_type;
    QByteArray  m_file_data;
    QString     m_group_uuid;
    QString     m_exp;
};

struct tagFileGroupInfo
{
    QString                     m_group_name;
    QString                     m_uuid;
    QList<tagFileInfo*>         m_files;
    QMap<QString,tagFileInfo*>  m_uuid_to_file;
};

class CORELIB_EXPORT QFileManager : public QObject
{
    Q_OBJECT
public:
    explicit QFileManager(QObject *parent = 0);
    ~QFileManager();

    void    load();
    void    save(const QString &uuid);
    void    clear();
    void    rename(const QString &uuid,const QString &new_group_name);

    void    insert_group(tagFileGroupInfo *group,int index=-1);
    void    remove_group(const QString &uuid);

    void    insert_file(const QString &group_uuid,tagFileInfo *info,int index=-1);
    void    remove_file(const QString &group_uuid,const QString &file_uuid);

    void    changed_file(const QString &group_uuid,tagFileInfo *info);

    tagFileGroupInfo * get_group(const QString &group_uuid);
    tagFileInfo     *  get_file(const QString &file_uuid);
    QList<tagFileGroupInfo*> get_all_group();
protected:
    void    load(const QString &file);
    void    save_group_list();
signals:
    void    insert_group_signal(tagFileGroupInfo *group, int index);
    void    remove_group_signal(tagFileGroupInfo* group);
    void    insert_file_signal(tagFileGroupInfo *group,tagFileInfo* file,int index);
    void    remove_file_signal(tagFileGroupInfo* group,tagFileInfo* file);
    void    refresh(tagFileGroupInfo* group,tagFileInfo* file);
public slots:
protected:
    QList<tagFileGroupInfo*>        m_groups;
    QMap<QString,tagFileGroupInfo*> m_uuid_to_group;
};

#endif // QFILEMANAGER_H
