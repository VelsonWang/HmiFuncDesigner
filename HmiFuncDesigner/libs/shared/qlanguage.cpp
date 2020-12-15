#include "qlanguage.h"

#include "xmlobject.h"

#include <QUuid>

QLanguage::QLanguage()
{
}

QLanguage::~QLanguage()
{
    clear();
}

void QLanguage::toObject(XMLObject *xml)
{
    xml->setTagName("Language");
    xml->setProperty("name",m_language_name);
    xml->setProperty("information",m_information);
    xml->setProperty("uuid",m_uuid);

    QMapIterator<QString,tagTranslateInfo*>   it(m_translates);
    while(it.hasNext())
    {
        it.next();
        XMLObject *tr=new XMLObject(xml);
        tr->setTagName("Tr");
        tr->setProperty("translate",it.value()->m_translate);
        tr->setProperty("uuid",it.value()->m_uuid);
    }
}

void QLanguage::fromObject(XMLObject *xml)
{
    clear();

    if(xml->getTagName()=="Language")
    {
        m_language_name=xml->getProperty("name");
        m_information=xml->getProperty("information");
        m_uuid=xml->getProperty("uuid");
        if(m_uuid=="")
        {
            m_uuid=QUuid::createUuid().toString();
        }
        QList<XMLObject*> children=xml->getChildren();

        foreach(XMLObject *obj,children)
        {
            tagTranslateInfo* info=new tagTranslateInfo;
            info->m_translate=obj->getProperty("translate");
            info->m_uuid=obj->getProperty("uuid");
            if(info->m_uuid=="")
            {
                info->m_uuid=QUuid::createUuid().toString();
            }
            m_translates.insert(info->m_uuid,info);
        }
    }
}

void QLanguage::clear()
{
    qDeleteAll(m_translates.values());
    m_translates.clear();
    m_language_name="";
    m_file_name="";
}

QString QLanguage::get_language_name()
{
    return m_language_name;
}

void QLanguage::set_language_name(const QString &name)
{
    m_language_name=name;
}

tagTranslateInfo* QLanguage::get_translate(const QString &uuid)
{
    return m_translates.value(uuid);
}

QMap<QString,tagTranslateInfo*> QLanguage::get_all_translate()
{
    return m_translates;
}

void QLanguage::remove_translate(const QString &uiid)
{
    tagTranslateInfo* info=m_translates.value(uiid);
    if(info!=NULL)
    {
        m_translates.remove(uiid);
        delete info;
    }
}

void QLanguage::set_translate(const QString &uuid, const QString &translate)
{
    tagTranslateInfo *info=get_translate(uuid);
    if(info!=NULL)
    {
        info->m_translate=translate;
    }
}

tagTranslateInfo* QLanguage::add_translate(tagTranslateInfo &info)
{
    if(NULL==m_translates.value(info.m_uuid))
    {
        tagTranslateInfo *in=new tagTranslateInfo;
        in->m_translate=info.m_translate;
        in->m_uuid=info.m_uuid;
        if(in->m_uuid=="")
        {
            in->m_uuid=QUuid::createUuid().toString();
        }
        m_translates.insert(in->m_uuid,in);
        return in;
    }
    return NULL;
}

void QLanguage::set_file_name(const QString &fileName)
{
    m_file_name=fileName;
}

QString QLanguage::get_file_name()
{
    return m_file_name;
}

QString QLanguage::get_information()
{
    return m_information;
}

void QLanguage::set_information(const QString &information)
{
    m_information=information;
}

void QLanguage::setUuid(const QString &uuid)
{
    m_uuid=uuid;
}

QString QLanguage::getUuid()
{
    return m_uuid;
}

QLanguage& QLanguage::operator =(const QLanguage& l)
{
    m_file_name=l.m_file_name;
    m_language_name=l.m_language_name;
    m_information=l.m_information;
    m_uuid=l.m_uuid;

    QMapIterator<QString,tagTranslateInfo*>   it(l.m_translates);

    while(it.hasNext())
    {
        add_translate(*it.value());
    }

    return *this;
}
