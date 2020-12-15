#include "qdata.h"

#include "xmlobject.h"

#include <QDynamicPropertyChangeEvent>

#include <QUuid>

QData::QData(QObject *parent) :
    QObject(parent)
{
    m_uuid=QUuid::createUuid().toString();
}

QData::~QData()
{
    clear();
}

void QData::clear()
{
    m_uuid_to_data.clear();

    while(m_datas.size()>0)
    {
        tagDataInfo* info=m_datas.takeFirst();
        setProperty(info->m_name.toLocal8Bit(),QVariant());
        delete info;
    }

    m_name="";
    m_uuid="";
}

void QData::insert_data(const tagDataInfo &info, int index)
{
    if(info.m_uuid=="")
    {
        return;
    }
    tagDataInfo *data=m_uuid_to_data.value(info.m_uuid);
    if(data!=NULL)
    {
        return;
    }

    data=new tagDataInfo;
    data->m_uuid=info.m_uuid;
    data->m_name=info.m_name;
    data->m_value=info.m_value;
    data->m_save_time=info.m_save_time;
    data->m_information=info.m_information;
    data->m_hold=info.m_hold;
    data->m_type=info.m_type;
    data->m_data=this;

    if(index<0 || index>=m_datas.size())
    {
        index=m_datas.size();
    }

    m_datas.insert(index,data);
    m_uuid_to_data.insert(data->m_uuid,data);

    emit data_insert(data,index);
}

void QData::remove_data(const QString &uuid)
{
    tagDataInfo *data=m_uuid_to_data.value(uuid);
    if(data!=NULL)
    {
        emit data_remove(data);
        m_uuid_to_data.remove(uuid);
        m_datas.removeAll(data);
        delete data;
    }
}

bool QData::event(QEvent *e)
{
    if(e->type()==QEvent::DynamicPropertyChange)
    {
        QDynamicPropertyChangeEvent* event=(QDynamicPropertyChangeEvent*)e;
        QString name=event->propertyName();
        tagDataInfo* data=get_data_by_name(name);
        if(data!=NULL)
        {
            if(data->m_value!=property(name.toLocal8Bit()))
            {
                data->m_value=property(name.toLocal8Bit());
                emit data_changed(data->m_uuid);
            }
        }
    }
    return QObject::event(e);
}

tagDataInfo *QData::get_data_by_name(const QString &name)
{
    foreach(tagDataInfo* data,m_datas)
    {
        if(data->m_name==name)
        {
            return data;
        }
    }
    return NULL;
}

tagDataInfo* QData::get_data(const QString &uuid)
{
    return m_uuid_to_data.value(uuid);
}

QList<tagDataInfo*> QData::get_datas()
{
    return m_datas;
}

QStringList     QData::get_all_data_names()
{
    QStringList list;
    foreach(tagDataInfo* data,m_datas)
    {
        list.append(data->m_name);
    }
    return list;
}

void QData::toObject(XMLObject *xml)
{
    xml->clear();
    xml->setTagName("Group");
    xml->setProperty("uuid",m_uuid);
    xml->setProperty("name",m_name);

    foreach(tagDataInfo* data,m_datas)
    {
        XMLObject* obj=new XMLObject(xml);
        obj->setTagName("Data");
        obj->setProperty("uuid",data->m_uuid);
        obj->setProperty("name",data->m_name);
        obj->setProperty("type",data->m_type);
        obj->setProperty("value",data->m_value.toString());
        obj->setProperty("save_period",QString::number(data->m_save_time));
        obj->setProperty("hold",data->m_hold?"true":"false");
        obj->setProperty("information",data->m_information);
    }
}

void QData::fromObject(XMLObject *xml)
{
    clear();

    if(xml->getTagName()!="Group")
    {
        return;
    }

    m_name=xml->getProperty("name");
    if(m_name=="")
    {
        return;
    }

    m_uuid=xml->getProperty("uuid");
    if(m_uuid=="")
    {
        m_uuid=QUuid::createUuid().toString();
    }



    QList<XMLObject*>   data_object=xml->getChildren();

    QString uuid;
    tagDataInfo *data;
    foreach(XMLObject* obj,data_object)
    {
        if(obj->getTagName()=="Data")
        {
            uuid=obj->getProperty("uuid");
            if(uuid=="")
            {
                uuid=QUuid::createUuid().toString();
            }
            data=new tagDataInfo;
            data->m_uuid=uuid;
            data->m_type=obj->getProperty("type");
            if(data->m_type=="")
            {
                data->m_type="String";
            }
            data->m_name=obj->getProperty("name");
            data->m_value=obj->getProperty("value");
            data->m_information=obj->getProperty("information");
            data->m_save_time=obj->getProperty("save_period").toInt();
            data->m_hold=(obj->getProperty("hold")=="true");
            data->m_data=this;
            setProperty(data->m_name.toLocal8Bit(),data->m_value);
            m_datas.append(data);
            m_uuid_to_data.insert(data->m_uuid,data);
        }
    }
}

void QData::setUuid(const QString &uuid)
{
    m_uuid=uuid;
}

QString QData::getUuid()
{
    return m_uuid;
}

void QData::set_name(const QString &name)
{
    m_name=name;
}

QString QData::get_name()
{
    return m_name;
}

void QData::emit_refresh(tagDataInfo *info)
{
    emit refresh(info);
}

void QData::copy(const QData& data)
{
    clear();

    m_uuid=data.m_uuid;
    m_name=data.m_name;

    foreach(tagDataInfo* info,data.m_datas)
    {
        insert_data(*info);
    }
}
