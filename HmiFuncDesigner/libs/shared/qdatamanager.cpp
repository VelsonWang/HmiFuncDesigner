#include "qdatamanager.h"

#include "xmlobject.h"
#include "qdata.h"

#include <QFile>

QDataManager::QDataManager(QObject *parent) :
    QObject(parent)
{
}

QDataManager::~QDataManager()
{
    clear();
}

void QDataManager::load(const QString &path)
{
    clear();
    m_path=path;

    QFile f(m_path+"/data.xml");

    if(f.open(QFile::ReadOnly))
    {
        QString buffer=f.readAll();

        XMLObject xml;
        if(xml.load(buffer,0))
        {
            if(xml.getTagName()=="Groups")
            {
                QList<XMLObject*> groups=xml.getChildren();

                foreach(XMLObject *o,groups)
                {
                    if(o->getTagName()=="Group")
                    {
                        QData *data=new QData;
                        data->fromObject(o);
                        m_datas.append(data);
                        m_uuid_to_data.insert(data->getUuid(),data);
                    }
                }
            }
        }
    }
}

void QDataManager::save(const QString &path)
{

    XMLObject xml;

    xml.setTagName("Groups");

    foreach(QData *data,m_datas)
    {
        XMLObject* g=new XMLObject(&xml);
        data->toObject(g);
    }

    QString str=xml.write();

    QFile f(path+"/data.xml");
    if(f.open(QFile::WriteOnly))
    {
        f.write(str.toLocal8Bit());
        f.close();
    }
}

void QDataManager::clear()
{
    m_uuid_to_data.clear();
    qDeleteAll(m_datas);
    m_datas.clear();
    m_path="";
}

QList<QData*>   QDataManager::get_all_datas()
{
    return m_datas;
}

QData* QDataManager::get_data_group(const QString &uuid)
{
    return m_uuid_to_data.value(uuid);
}

QData* QDataManager::get_data_group_by_name(const QString &name)
{
    foreach(QData* data,m_datas)
    {
        if(data->get_name()==name)
        {
            return data;
        }
    }
    return NULL;
}

tagDataInfo* QDataManager::get_data(const QString &uuid)
{
    foreach(QData* info,m_datas)
    {
        tagDataInfo *data=info->get_data(uuid);
        if(data!=NULL)
            return data;
    }
    return NULL;
}

void QDataManager::insert_group(const QData &data, int index)
{
    if(index<0 || index>m_datas.size())
    {
        index=m_datas.size();
    }
    QData *dt=new QData;
    dt->copy(data);
    m_datas.insert(index,dt);
    m_uuid_to_data.insert(dt->getUuid(),dt);
    emit insert_group_signal(dt,index);
}

void QDataManager::remove_group(const QString &uuid)
{
    QData *data=m_uuid_to_data.value(uuid);
    if(data!=NULL)
    {
        remove_group_signal(data);
        m_uuid_to_data.remove(uuid);
        m_datas.removeAll(data);
        delete data;
    }
}

void QDataManager::emit_refresh(QData *data)
{
    emit refresh(data);
}

QStringList QDataManager::get_all_group_name()
{
    QStringList list;
    foreach(QData* data,m_datas)
    {
        list.append(data->get_name());
    }
    return list;
}
