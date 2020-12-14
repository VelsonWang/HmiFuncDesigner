#include "qpagemanager.h"

#include "xmlobject.h"

#include "host/qabstracthost.h"
#include "host/qformhost.h"
#include "qhostfactory.h"
#include "property/qabstractproperty.h"

#include <QFile>

QPageManager::QPageManager()
{
}

void QPageManager::load(const QString &project_path)
{
    clear();
    m_project_path=project_path;
    QFile f(m_project_path+"/pages.xml");
    if(!f.open(QFile::ReadOnly))
    {
        return;
    }
    QString buffer=f.readAll();
    f.close();
    XMLObject xml;
    if(!xml.load(buffer,0))
    {
        return;
    }

    QList<XMLObject*> children=xml.getChildren();

    foreach(XMLObject* obj,children)
    {
        load_page(obj->get_property("path"));
    }
}

void QPageManager::save(const QString &project_path)
{
    XMLObject path;
    path.set_title("Pages");

    XMLObject page;

    XMLObject *temp;

    QString p;
    QString old_path;

    foreach(QAbstractHost* h,m_page_list)
    {
        temp=new XMLObject(&path);
        temp->set_title("Page");
        p=h->get_host_type()+"/"+h->get_property_value("objectName").toString()+".xml";
        temp->set_property("path",p);
        page.clear();
        h->to_object(&page);
        old_path=h->property("old_file_name").toString();
        if(old_path!="" && old_path!=p)
        {
            QFile::remove(project_path+"/pages/"+old_path);
        }
        h->setProperty("old_file_name",p);
        QFile f(project_path+"/pages/"+p);
        if(f.open(QFile::WriteOnly))
        {
            f.resize(0);
            f.write(page.write().toLocal8Bit());
            f.close();
        }
    }

    QFile f(project_path+"/pages.xml");
    if(f.open(QFile::WriteOnly))
    {
        f.resize(0);
        f.write(path.write().toLocal8Bit());
        f.close();
    }
}

void QPageManager::clear()
{
    //qDeleteAll(m_page_list);
    //m_page_list.clear();


    foreach(QAbstractHost* h,m_page_list)
    {
        h->deleteLater();
    }

    m_page_list.clear();
    m_project_path="";
}

void QPageManager::load_page(const QString &fileName)
{
    QFile f(m_project_path+"/pages/"+fileName);

    if(!f.open(QFile::ReadOnly))
    {
        return;
    }

    QString buffer=f.readAll();
    f.close();
    XMLObject xml;
    QString str;
    if(!xml.load(buffer,&str))
    {
        return;
    }

    QAbstractHost* host=QHostFactory::create_host(&xml);
    if(host!=NULL)
    {
        m_page_list.append(host);
        m_uuid_to_page.insert(host->get_uuid(),host);
        host->setProperty("old_file_name",fileName);
        host->set_default();
        QList<QAbstractHost*> list;
        list.append(host);
        while(list.size()>0)
        {
            QAbstractHost *h=list.takeFirst();
            QAbstractProperty* pro=h->get_property("objectName");
            if(pro!=NULL)
            {
                connect(pro,SIGNAL(value_chaged(QVariant,QVariant)),this,SLOT(host_name_changed_slot(QVariant,QVariant)));
            }
            list+=h->getChildren();
        }
    }
}

QList<QAbstractHost*> QPageManager::getPages()
{
    return m_page_list;
}

QAbstractHost* QPageManager::get_page(int index)
{
    if(index<0 || index>=m_page_list.size())
    {
        return NULL;
    }
    else
    {
        return m_page_list.at(index);
    }
}

QAbstractHost* QPageManager::get_page(const QString &uuid)
{
    return m_uuid_to_page.value(uuid);
}

void QPageManager::insert_page(QAbstractHost *host, int index)
{
    if(index<0 || index>m_page_list.size())
    {
        index=m_page_list.size();
    }
    m_page_list.insert(index,host);
    m_uuid_to_page.insert(host->get_uuid(),host);
    QList<QAbstractHost*> list;
    list.append(host);
    while(list.size()>0)
    {
        QAbstractHost *h=list.takeFirst();
        QAbstractProperty* pro=h->get_property("objectName");
        if(pro!=NULL)
        {
            connect(pro,SIGNAL(value_chaged(QVariant,QVariant)),this,SLOT(host_name_changed_slot(QVariant,QVariant)));
        }
        list+=h->getChildren();
    }
    emit insert_page_signal(host);
    emit host_name_changed(host);
}

void QPageManager::remove_page(QAbstractHost *host)
{
    QList<QAbstractHost*> list;
    list.append(host);
    while(list.size()>0)
    {
        QAbstractHost *h=list.takeFirst();
        QAbstractProperty* pro=h->get_property("objectName");
        if(pro!=NULL)
        {
            disconnect(pro,SIGNAL(value_chaged(QVariant,QVariant)),this,SLOT(host_name_changed_slot(QVariant,QVariant)));
        }
        list+=h->getChildren();
    }
    emit remove_page_signal(host);
    emit host_name_changed(host);
    m_page_list.removeAll(host);
    m_uuid_to_page.remove(host->get_uuid());
}

QList<QAbstractHost*> QPageManager::getPages_by_title(const QString &title)
{
    QList<QAbstractHost*> list;
    foreach(QAbstractHost* host,m_page_list)
    {
        if(host->property("title").toString()==title)
        {
            list.append(host);
        }
    }
    return list;
}

void QPageManager::host_name_changed_slot(const QVariant &, const QVariant &)
{
    QAbstractProperty *pro=(QAbstractProperty*)sender();

    emit host_name_changed(pro->get_host());
}
