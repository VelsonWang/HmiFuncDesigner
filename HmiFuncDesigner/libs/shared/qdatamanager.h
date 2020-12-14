#ifndef QDATAMANAGER_H
#define QDATAMANAGER_H

#include "sharedlibglobal.h"

#include <QObject>
#include <QMap>

class QData;
struct tagDataInfo;

class SHAREDLIB_EXPORT QDataManager : public QObject
{
    Q_OBJECT
public:
    explicit QDataManager(QObject *parent = 0);
    ~QDataManager();
    
    void    load(const QString &path);
    void    clear();
    void    save(const QString &path);

    QList<QData*> get_all_datas();
    QData       *get_data_group(const QString &uuid);
    tagDataInfo *get_data(const QString &uuid);
    QData       *get_data_group_by_name(const QString &name);
    QStringList  get_all_group_name();

    void        emit_refresh(QData* data);

    void        insert_group(const QData& data,int index);
    void        remove_group(const QString &uuid);
signals:
    void    refresh(QData*  data);
    void    insert_group_signal(QData* data,int index);
    void    remove_group_signal(QData* data);
public slots:
protected:
    QMap<QString,QData*>            m_uuid_to_data;
    QList<QData*>                   m_datas;
    QString                         m_path;
};

#endif // QDATAMANAGER_H
