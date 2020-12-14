#ifndef QDATA_H
#define QDATA_H

#include "sharedlibglobal.h"

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QStringList>

class QData;
struct tagDataInfo
{
    QString     m_uuid;
    QString     m_name;
    QString     m_type;
    QString     m_information;
    QVariant    m_value;
    int         m_save_time;
    bool        m_hold;
    QData       *m_data;
};

class XMLObject;

class SHAREDLIB_EXPORT QData : public QObject
{
    Q_OBJECT
public:
    explicit QData(QObject *parent = 0);
    ~QData();

    void    insert_data(const tagDataInfo &info,int index=-1);
    void    remove_data(const QString &uuid);

    tagDataInfo*    get_data(const QString &uuid);
    tagDataInfo*    get_data_by_name(const QString& name);
    QList<tagDataInfo*> get_datas();
    QStringList     get_all_data_names();

    void    to_object(XMLObject* xml);
    void    from_object(XMLObject* xml);

    QString get_uuid();
    void    set_uuid(const QString &uuid);

    QString get_name();
    void    set_name(const QString &name);

    void    emit_refresh(tagDataInfo *info);
    void    copy(const QData& data);
protected:
    bool    event(QEvent *);
    void    clear();
signals:
    void    data_changed(const QString &uuid);
    void    data_insert(tagDataInfo* info,int index);
    void    data_remove(tagDataInfo* info);
    void    refresh(tagDataInfo* info);
public slots:
    
protected:
    QList<tagDataInfo*>             m_datas;
    QMap<QString,tagDataInfo*>      m_uuid_to_data;
    QString                         m_uuid;
    QString                         m_name;
};

#endif // QDATA_H
