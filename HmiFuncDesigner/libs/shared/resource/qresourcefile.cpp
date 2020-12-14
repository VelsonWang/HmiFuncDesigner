#include "qresourcefile.h"

#include <QFile>

void QResourceFile::clear()
{
    qDeleteAll(m_resourceInfos);
    m_resourceInfos.clear();
    m_name_to_resource.clear();
}

QResourceFile::~QResourceFile()
{
    clear();
}

tagResourceInfo* QResourceFile::resource(const QString &name)
{
    return m_name_to_resource.value(name);
}

void QResourceFile::addResource(const QString &name, const QByteArray &data)
{
    tagResourceInfo *info=resource(name);
    if(info!=NULL)
    {
        info->m_resourceData=data;
    }
    else
    {
        info=new tagResourceInfo;
        info->m_resourceData=data;
        info->m_resourceName=name;
        m_name_to_resource.insert(name,info);
        m_resourceInfos.append(info);
    }
}

void QResourceFile::removeResource(const QString &name)
{
    tagResourceInfo *info=m_name_to_resource.value(name);
    if(info!=NULL)
    {
        m_resourceInfos.removeAll(info);
        m_name_to_resource.remove(name);
        delete info;
    }
}

bool QResourceFile::save(const QString &path)
{

    QString fileName=path+"/resource.rus";

    QFile f(fileName);
    if(!f.open(QFile::WriteOnly))
    {
        return false;
    }

    f.resize(0);

    QString str;
    int temp;
    foreach(tagResourceInfo* info,m_resourceInfos)
    {
        str=info->m_resourceName;
        f.write(str.toLocal8Bit().data(),strlen(str.toLocal8Bit().data())+1);
        temp=info->m_resourceData.size();
        f.write((char*)&temp,4);
        f.write(info->m_resourceData);
    }

    f.close();
    return true;
}

bool QResourceFile::load(const QString &path)
{
    QString fileName=path+"/resource.rus";

    QFile f(fileName);
    if(!f.open(QFile::ReadOnly))
    {
        return false;
    }

    QByteArray array=f.readAll();

    int pt=0;
    int size=array.size();
    char* buffer=array.data();
    QString name;
    int resize;
    QByteArray data;
    while(pt<size)
    {
        name=QString(buffer+pt);
        pt+=(strlen(buffer+pt)+1);
        if(pt>=size)
        {
            break;
        }
        memcpy((char*)&resize,buffer+pt,4);
        pt+=4;
        if(pt+resize>size)
        {
            break;
        }
        data=array.mid(pt,resize);
        addResource(name,data);
        pt+=resize;
    }

    f.close();

    return true;
}

QStringList QResourceFile::get_all_file()
{
    return m_name_to_resource.keys();
}

QList<tagResourceInfo*> QResourceFile::get_all_file_info()
{
    return m_resourceInfos;
}
