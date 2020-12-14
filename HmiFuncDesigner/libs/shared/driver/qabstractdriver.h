#ifndef QABSTRACTDRIVER_H
#define QABSTRACTDRIVER_H

#include "../host/qabstracthost.h"

#include <QMap>

enum  enDataTpye
{
    DT_READ,
    DT_WRITE
};

struct tagDriverDataInfo
{
    QString     m_uuid;
    QString     m_name;
    QString     m_data_uuid;
    QString     m_information;
    int         m_data_address;
    int         m_period;
    int         m_temp_data;
    int         m_level;
    int         m_scale;
    enDataTpye  m_type;
    tagDriverDataInfo()
    {
        m_uuid=m_name=m_data_uuid=m_information="";
        m_data_address=0;
        m_period=1000;
        m_temp_data=0;
        m_level=10;
        m_scale=1;
        m_type=DT_READ;
    }
};

class SHAREDLIB_EXPORT QAbstractDriver : public QAbstractHost
{
    Q_OBJECT
public:
    QAbstractDriver(QAbstractHost *parent = 0);
    ~QAbstractDriver();

    bool    load(const QString &path);
    void    save(const QString &path);

    QList<tagDriverDataInfo*>   get_all_data();
    tagDriverDataInfo*          get_data(const QString &uuid);

    void                        insert_data(const tagDriverDataInfo& data,int index=-1);
    void                        remove_data(const QString &uuid);

    QString                     get_name();
    void                        set_name(const QString &name);
    QStringList                 get_all_data_names();

    static QString get_show_group();

    void                        changed_data(const QString &uuid,const QString &key,const QVariant &value);
signals:
    void    insert_data_signals(tagDriverDataInfo* data,int index);
    void    remove_data_signals(tagDriverDataInfo* data);
    void    data_refresh(tagDriverDataInfo *data);
protected:
    void init_property();
protected:
    QList<tagDriverDataInfo*>       m_datas;
    QMap<QString,tagDriverDataInfo*>    m_uuid_to_data;
    QString                         m_name;
};

class SHAREDLIB_EXPORT QAbstractDriverObject : public QObject
{
    Q_OBJECT
public:
    virtual void    start()=0;
    virtual void    close()=0;

};

#endif // QABSTRACTDRIVER_H
