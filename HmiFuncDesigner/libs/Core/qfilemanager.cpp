#include "qfilemanager.h"

#include "../shared/xmlobject.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QUuid>

QFileManager::QFileManager(QObject *parent) :
    QObject(parent)
{
}

QFileManager::~QFileManager()
{
    clear();
}

void QFileManager::load()
{
    QString path=QApplication::applicationDirPath()+"/resources/";

    QFile f(path+"res.xml");

    if(!f.open(QFile::ReadOnly))
    {
        return;
    }
    QString buff=f.readAll();

    XMLObject xml;
    if(xml.load(buff,0))
    {
        if(xml.get_title()=="Res")
        {
            QList<XMLObject*>   list=xml.getChildren();
            foreach(XMLObject* c,list)
            {
                if(c->get_title()=="Group")
                {
                    load(path+c->get_property("name")+".res");
                }
            }
        }
    }
}

void QFileManager::load(const QString &file)
{
    QFile f(file);

    if(!f.open(QFile::ReadOnly))
    {
        return;
    }

    QByteArray data=f.readAll();
    f.close();

    tagFileGroupInfo *group=new tagFileGroupInfo;
    int index=file.lastIndexOf("/");
    group->m_group_name=file.mid(index+1);
    group->m_group_name=group->m_group_name.left(group->m_group_name.size()-4);
    group->m_uuid=QUuid::createUuid().toString();

    char* pt=data.data();

    int point=0;
    QString name;
    QString type;
    QString exp;
    int     size;
    int     temp;
    while(1)
    {
        if(point>=data.size())
        {
            break;
        }
        name=QString::fromLocal8Bit(pt+point);
        if(name=="")
        {
            break;
        }
        point+=(strlen(pt+point)+1);
        if(point>=data.size())
        {
            break;
        }
        exp=QString::fromLocal8Bit(pt+point);
        if(exp=="")
        {
            break;
        }
        point+=(strlen(pt+point)+1);
        if(point>=data.size())
        {
            break;
        }
        memcpy(&temp,pt+point,sizeof(int));
        if(temp==0)
        {
            type=FILE_SYSTEM;
        }
        else
        {
            type=FILE_USER;
        }
        point+=sizeof(int);
        if(point>=data.size())
        {
            break;
        }
        memcpy(&size,pt+point,sizeof(int));
        if(size<=0)
        {
            break;
        }
        point+=sizeof(int);
        if(point+size>=data.size())
        {
            break;
        }
        tagFileInfo *info=new tagFileInfo;
        info->m_file_name=name;
        info->m_file_data=QByteArray(pt+point,size);
        info->m_exp=exp;
        info->m_type=type;
        info->m_uuid=QUuid::createUuid().toString();
        info->m_group_uuid=group->m_uuid;
        group->m_files.append(info);
        group->m_uuid_to_file.insert(info->m_uuid,info);
    }

    m_groups.append(group);
    m_uuid_to_group.insert(group->m_uuid,group);
}

void QFileManager::save(const QString &uuid)
{
    tagFileGroupInfo *group=m_uuid_to_group.value(uuid);

    if(group==NULL)
    {
        return;
    }

    QString path=QApplication::applicationDirPath()+"/resources/";

    QFile f(path+group->m_group_name+".res");

    if(!f.open(QFile::WriteOnly))
    {
        return;
    }
    f.resize(0);

    int temp;
    foreach(tagFileInfo *info,group->m_files)
    {
        f.write(info->m_file_name.toLocal8Bit(),strlen(info->m_file_name.toLocal8Bit())+1);
        f.write(info->m_exp.toLocal8Bit(),strlen(info->m_exp.toLocal8Bit())+1);
        if(info->m_type==FILE_SYSTEM)
        {
            temp=0;
        }
        else
        {
            temp=1;
        }
        f.write((char*)&temp,sizeof(int));
        temp=info->m_file_data.size();
        f.write((char*)&temp,sizeof(int));
        f.write(info->m_file_data);
    }

    f.close();
}

void QFileManager::clear()
{
    m_uuid_to_group.clear();

    while(m_groups.size()>0)
    {
        qDeleteAll(m_groups.first()->m_files);
        delete m_groups.takeFirst();
    }
}

void QFileManager::rename(const QString &uuid, const QString &new_group_name)
{
    tagFileGroupInfo *info=m_uuid_to_group.value(uuid);
    if(info==NULL || info->m_group_name==new_group_name)
    {
        return;
    }

    QString path=QApplication::applicationDirPath()+"/resources/";

    QFile f1(path+info->m_group_name+".res");
    QFile f2(path+new_group_name+".res");

    if(!f1.open(QFile::ReadOnly) || !f2.open(QFile::WriteOnly))
    {
        return;
    }
    f2.resize(0);
    f2.write(f1.readAll());
    f1.close();
    f2.close();

    QFile::remove(path+info->m_group_name+".res");

    info->m_group_name=new_group_name;

    save_group_list();
}

void QFileManager::save_group_list()
{
    XMLObject xml;
    xml.set_title("Res");

    foreach(tagFileGroupInfo *group,m_groups)
    {
        XMLObject *c=new XMLObject(&xml);
        c->set_title("Group");
        c->set_property("name",group->m_group_name);
    }

    QString buff=xml.write();

    QString path=QApplication::applicationDirPath()+"/resources/";

    QFile f(path+"res.xml");
    if(!f.open(QFile::WriteOnly))
    {
        return;
    }
    f.resize(0);

    f.write(buff.toLocal8Bit());
    f.close();
}

void QFileManager::insert_group(tagFileGroupInfo *group, int index)
{
    tagFileGroupInfo *g=m_uuid_to_group.value(group->m_uuid);
    if(g!=NULL)
    {
        return;
    }
    g=new tagFileGroupInfo;
    g->m_uuid=group->m_uuid;
    g->m_group_name=group->m_group_name;

    foreach(tagFileInfo *info,group->m_files)
    {
        tagFileInfo *in=new tagFileInfo;
        in->m_file_data=info->m_file_data;
        in->m_exp=info->m_exp;
        in->m_file_name=info->m_file_name;
        in->m_type=info->m_type;
        in->m_uuid=info->m_uuid;
        in->m_group_uuid=info->m_group_uuid;
        g->m_files.append(in);
        g->m_uuid_to_file.insert(in->m_uuid,in);
    }

    if(index<0 || index>m_groups.size())
    {
        index=m_groups.size();
    }
    m_groups.insert(index,g);
    m_uuid_to_group.insert(g->m_uuid,g);
    emit insert_group_signal(g,index);
}

void QFileManager::remove_group(const QString &uuid)
{
    tagFileGroupInfo *group=m_uuid_to_group.value(uuid);
    if(group!=NULL)
    {
        emit remove_group_signal(group);
        qDeleteAll(group->m_files);
        m_groups.removeAll(group);
        m_uuid_to_group.remove(uuid);
    }
}

void QFileManager::insert_file(const QString &group_uuid, tagFileInfo *info, int index)
{
    tagFileGroupInfo *group=m_uuid_to_group.value(group_uuid);

    if(group!=NULL)
    {
        tagFileInfo *f=new tagFileInfo;
        f->m_file_data=info->m_file_data;
        f->m_file_name=info->m_file_name;
        f->m_exp=info->m_exp;
        f->m_type=info->m_type;
        f->m_uuid=info->m_uuid;
        f->m_group_uuid=group->m_uuid;

        if(index<0 || index>group->m_files.size())
        {
            index=group->m_files.size();
        }

        group->m_files.insert(index,f);
        group->m_uuid_to_file.insert(f->m_uuid,f);
        emit insert_file_signal(group,f,index);
    }
}

void QFileManager::remove_file(const QString &group_uuid, const QString &file_uuid)
{
    tagFileGroupInfo *group=m_uuid_to_group.value(group_uuid);

    if(group!=NULL)
    {
        tagFileInfo *f=group->m_uuid_to_file.value(file_uuid);

        if(f!=NULL)
        {
            emit remove_file_signal(group,f);
            group->m_uuid_to_file.remove(file_uuid);
            group->m_files.removeAll(f);
            delete f;
        }
    }
}

void QFileManager::changed_file(const QString &group_uuid, tagFileInfo *info)
{
    tagFileGroupInfo *group=m_uuid_to_group.value(group_uuid);

    if(group!=NULL)
    {
        tagFileInfo *f=group->m_uuid_to_file.value(info->m_uuid);

        if(f!=NULL)
        {
            f->m_file_data=info->m_file_data;
            f->m_file_name=info->m_file_name;
            f->m_exp=info->m_exp;
            f->m_type=info->m_type;
            emit refresh(group,f);
        }
    }
}

tagFileGroupInfo* QFileManager::get_group(const QString &group_uuid)
{
    return m_uuid_to_group.value(group_uuid);
}

QList<tagFileGroupInfo*> QFileManager::get_all_group()
{
    return m_groups;
}

tagFileInfo *QFileManager::get_file(const QString &file_uuid)
{
    tagFileInfo *ret=NULL;
    foreach(tagFileGroupInfo *group,m_groups)
    {
        ret=group->m_uuid_to_file.value(file_uuid);
        if(ret!=NULL)
        {
            return ret;
        }
    }
    return ret;
}
