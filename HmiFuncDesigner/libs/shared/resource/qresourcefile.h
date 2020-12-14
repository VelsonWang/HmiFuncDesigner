#ifndef QRESOURCEFILE_H
#define QRESOURCEFILE_H

#include "../sharedlibglobal.h"

#include <QList>
#include <QStringList>
#include <QMap>

struct tagResourceInfo
{
    QString     m_resourceName;
    QByteArray  m_resourceData;
};

class SHAREDLIB_EXPORT QResourceFile
{
public:
    ~QResourceFile();
    bool    load(const QString &path);
    bool    save(const QString &path);

    void    clear();

    tagResourceInfo* resource(const QString &name);

    void    addResource(const QString &name, const QByteArray &data);
    void    removeResource(const QString &name);

    QStringList get_all_file();

    QList<tagResourceInfo*>     get_all_file_info();
protected:
    QList<tagResourceInfo*>           m_resourceInfos;
    QMap<QString,tagResourceInfo*>    m_name_to_resource;
};

#endif // QRESOURCEFILE_H
