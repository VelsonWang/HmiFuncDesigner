#include "qabstractdriver.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../xmlobject.h"

#include <QFile>
#include <QUuid>

QAbstractDriver::QAbstractDriver(QAbstractHost *parent):
    QAbstractHost(parent)
{
}

QAbstractDriver::~QAbstractDriver()
{
    qDeleteAll(m_datas);
    m_datas.clear();
    m_uuid_to_data.clear();
}

void QAbstractDriver::initProperty()
{
    QAbstractHost::initProperty();

    QAbstractProperty *pro;

    removeProperty("objectName");

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->setProperty("name","driver_type");
        pro->setAttribute("show_name",tr("Driver Type"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_EDITABLE,false);
        m_object->setProperty("driver_type","");
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->setProperty("name","time_out");
        pro->setAttribute("show_name",tr("Time Out"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_NEEDSAVE,true);
        m_object->setProperty("time_out",1000);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->setProperty("name","package_data");
        pro->setAttribute("show_name",tr("Package Data"));
        pro->setAttribute("group","Events");
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->setProperty("name","analysy_data");
        pro->setAttribute("show_name",tr("Analysy Data"));
        pro->setAttribute("group","Events");
        insertProperty(pro);
    }
}

bool QAbstractDriver::load(const QString &path)
{
    QFile f(path);

    if(!f.open(QFile::ReadOnly))
    {
        return false;
    }

    QString buff=f.readAll();

    XMLObject xml;
    if(!xml.load(buff,0))
    {
        return false;
    }

    if(xml.getTagName()!="Driver")
    {
        return false;
    }
    QString uuid=xml.getProperty("uuid");
    if(uuid=="")
    {
        uuid=QUuid::createUuid().toString();
    }
    setUuid(uuid);

    m_name=xml.getProperty("name");

    if(m_name=="")
    {
        return false;
    }

    QList<XMLObject*> list=xml.getChildren();

    foreach(XMLObject* c,list)
    {
        if(c->getTagName()==PROPERTY_TITLE)
        {
            QAbstractProperty* pro=m_nameToProperty.value(c->getProperty("name"));
            if(pro!=NULL)
            {
                pro->fromObject(c);
            }
        }
        else if(c->getTagName()=="Data")
        {
            tagDriverDataInfo *data=new tagDriverDataInfo;

            data->m_name=c->getProperty("name");
            if(data->m_name=="")
            {
                delete data;
                continue;
            }
            data->m_data_address=c->getProperty("data_address").toInt();
            data->m_data_uuid=c->getProperty("data_uuid");
            data->m_level=c->getProperty("level").toInt();
            data->m_period=c->getProperty("period").toInt();
            data->m_temp_data=c->getProperty("temp_data").toInt();
            data->m_type=(enDataTpye)c->getProperty("type").toInt();
            data->m_uuid=c->getProperty("uuid");
            data->m_information=c->getProperty("information");
            data->m_scale=c->getProperty("scale").toInt();
            if(data->m_uuid=="")
            {
                data->m_uuid=QUuid::createUuid().toString();
            }
            if(data->m_scale==0)
            {
                data->m_scale=1;
            }
            m_datas.append(data);
            m_uuid_to_data.insert(data->m_uuid,data);
        }
    }
    return true;
}

void QAbstractDriver::save(const QString &path)
{
    QFile f(path);
    if(!f.open(QFile::WriteOnly))
    {
        return;
    }

    f.resize(0);

    XMLObject xml;

    xml.setTagName("Driver");
    xml.setProperty("uuid",getUuid());
    xml.setProperty("name",m_name);

    foreach(QAbstractProperty* pro,m_propertys)
    {
        if(pro->modified() || pro->getAttribute(ATTR_NEEDSAVE).toBool())
        {
            XMLObject *o=new XMLObject(&xml);
            pro->toObject(o);
        }
    }

    foreach(tagDriverDataInfo* data,m_datas)
    {
        XMLObject *o=new XMLObject(&xml);
        o->setTagName("Data");
        o->setProperty("data_address",QString::number(data->m_data_address));
        o->setProperty("data_uuid",data->m_data_uuid);
        o->setProperty("level",QString::number(data->m_level));
        o->setProperty("name",data->m_name);
        o->setProperty("period",QString::number(data->m_period));
        o->setProperty("temp_data",QString::number(data->m_temp_data));
        o->setProperty("type",QString::number(data->m_type));
        o->setProperty("uuid",data->m_uuid);
        o->setProperty("information",data->m_information);
        o->setProperty("scale",QString::number(data->m_scale));
    }

    QString str=xml.write();

    f.write(str.toLocal8Bit());
    f.close();
}

QList<tagDriverDataInfo*> QAbstractDriver::get_all_data()
{
    return m_datas;
}

tagDriverDataInfo* QAbstractDriver::get_data(const QString &uuid)
{
    return m_uuid_to_data.value(uuid);
}

void QAbstractDriver::insert_data(const tagDriverDataInfo &data, int index)
{
    tagDriverDataInfo *d=new tagDriverDataInfo;
    d->m_name=data.m_name;
    if(d->m_name=="")
    {
        delete d;
    }
    d->m_data_address=data.m_data_address;
    d->m_data_uuid=data.m_data_uuid;
    d->m_level=data.m_level;
    d->m_period=data.m_period;
    d->m_temp_data=data.m_temp_data;
    d->m_type=data.m_type;
    d->m_uuid=data.m_uuid;
    d->m_information=data.m_information;
    d->m_scale=data.m_scale;
    if(d->m_uuid=="")
    {
        d->m_uuid=QUuid::createUuid().toString();
    }
    if(d->m_scale==0)
    {
        d->m_scale=1;
    }
    if(index<0 || index>m_datas.size())
    {
        index=m_datas.size();
    }
    m_datas.insert(index,d);
    m_uuid_to_data.insert(d->m_uuid,d);
    emit insert_data_signals(d,index);
}

void QAbstractDriver::remove_data(const QString &uuid)
{
    tagDriverDataInfo *data=m_uuid_to_data.value(uuid);
    if(data==NULL)
    {
        return;
    }

    emit remove_data_signals(data);
    m_datas.removeAll(data);
    m_uuid_to_data.remove(uuid);
    delete data;
}

void QAbstractDriver::set_name(const QString &name)
{
    m_name=name;
}

QString QAbstractDriver::get_name()
{
    return m_name;
}

QStringList QAbstractDriver::get_all_data_names()
{
    QStringList l;
    foreach(tagDriverDataInfo* data,m_datas)
    {
        l.append(data->m_name);
    }
    return l;
}

void QAbstractDriver::changed_data(const QString &uuid, const QString &key, const QVariant &value)
{
    tagDriverDataInfo *data=m_uuid_to_data.value(uuid);

    if(data==NULL)
    {
        return;
    }

    if(key=="name")
    {
        data->m_name=value.toString();
    }
    else if(key=="data")
    {
        QString str=value.toString();
        if(str.startsWith("{"))
        {
            data->m_data_uuid=str;
            data->m_temp_data=0;
        }
        else
        {
            data->m_data_uuid="";
            data->m_temp_data=value.toInt();
        }
    }
    else if(key=="address")
    {
        data->m_data_address=value.toInt();
    }
    else if(key=="period")
    {
        data->m_period=value.toInt();
    }
    else if(key=="type")
    {
        data->m_type=(enDataTpye)value.toInt();
    }
    else if(key=="level")
    {
        data->m_level=value.toInt();
    }
    else if(key=="information")
    {
        data->m_information=value.toString();
    }
    else if(key=="scale")
    {
        data->m_scale=value.toInt();
    }

    emit data_refresh(data);
}

QString QAbstractDriver::getShowGroup()
{
    return tr("Base");
}
