#include "qusermanager.h"

#include "xmlobject.h"

#include <QUuid>
#include <QFile>

QUserManager::QUserManager(QObject *parent) :
    QObject(parent)
{
}

QUserManager::~QUserManager()
{
    clear();
}

void QUserManager::clear()
{
    qDeleteAll(m_users);
    m_users.clear();
    m_id_to_user.clear();
}

void QUserManager::load(const QString &path)
{
    clear();

    QFile f(path+"/users.xml");

    if(!f.open(QFile::ReadOnly))
    {
        return;
    }

    QString str=f.readAll();
    f.close();

    XMLObject xml;

    if(xml.load(str,0))
    {
        if(xml.getTagName()=="Users")
        {
            QList<XMLObject*> children=xml.getChildren();

            foreach(XMLObject* u,children)
            {
                if(u->getTagName()=="User")
                {
                    tagUserInfo* info=new tagUserInfo;
                    info->m_name=u->getProperty("name");
                    info->m_level=u->getProperty("level").toInt();
                    info->m_password=u->getProperty("password");
                    info->m_information=u->getProperty("information");
                    info->m_uuid=u->getProperty("uuid");
                    if(info->m_uuid=="")
                    {
                        info->m_uuid=QUuid::createUuid().toString();
                    }
                    m_id_to_user.insert(info->m_uuid,info);
                    m_users.append(info);
                }
            }
        }
    }
}

void QUserManager::save(const QString &path)
{

    QFile f(path+"/users.xml");

    if(!f.open(QFile::WriteOnly))
    {
        return;
    }

    XMLObject xml;
    xml.setTagName("Users");

    foreach(tagUserInfo *info,m_users)
    {
        XMLObject *u=new XMLObject(&xml);
        u->setTagName("User");
        u->setProperty("name",info->m_name);
        u->setProperty("level",QString::number(info->m_level));
        u->setProperty("password",info->m_password);
        u->setProperty("information",info->m_information);
        u->setProperty("uuid",info->m_uuid);
    }
    QString s=xml.write();
    f.resize(0);
    f.write(s.toLocal8Bit());
    f.close();
}

QList<tagUserInfo*>   QUserManager::get_users()
{
    return m_users;
}

tagUserInfo* QUserManager::get_user(const QString &uiid)
{
    return m_id_to_user.value(uiid);
}

tagUserInfo* QUserManager::get_user(int index)
{
    if(index<0 || index>=m_users.size())
    {
        return NULL;
    }
    else
    {
        return m_users.value(index);
    }
}

void QUserManager::insert_user(const tagUserInfo &info, int index)
{
    tagUserInfo *temp=m_id_to_user.value(info.m_uuid);
    if(temp==NULL)
    {
        temp=new tagUserInfo;
        temp->m_uuid=info.m_uuid;
    }
    else
    {
        m_users.removeAll(temp);
        m_id_to_user.remove(info.m_name);
    }
    temp->m_level=info.m_level;
    temp->m_information=info.m_information;
    temp->m_password=info.m_password;
    temp->m_name=info.m_name;

    m_id_to_user.insert(temp->m_uuid,temp);
    if(index<0 || index>m_users.size())
    {
        index=m_users.size();
    }
    m_users.insert(index,temp);
    emit inser_user_signal(temp,index);
}

void QUserManager::remove_user(const QString &uuid)
{
    tagUserInfo *temp=m_id_to_user.value(uuid);
    if(temp!=NULL)
    {
        m_id_to_user.remove(uuid);
        m_users.removeAll(temp);
        emit remove_user_signal(temp);
        delete temp;
    }
}

void QUserManager::emit_refresh(tagUserInfo *info)
{
    emit refresh(info);
}
