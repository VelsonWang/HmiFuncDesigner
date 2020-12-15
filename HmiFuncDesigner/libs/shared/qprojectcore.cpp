#include "qprojectcore.h"

#include "xmlobject.h"
#include "qpagemanager.h"
#include "qlanguagemanager.h"
#include "qusermanager.h"
#include "host/qprojecthost.h"
#include "host/qformhost.h"
#include "property/qabstractproperty.h"
#include "qcommonstruct.h"
#include "qlanguage.h"
#include "qdatamanager.h"
#include "qdata.h"
#include "resource/qresourcemanager.h"
#include "qdrivermanager.h"
#include "driver/qabstractdriver.h"

#include <QFile>
#include <QDir>
#include <QUuid>
#include <QScriptEngine>

QProjectCore::QProjectCore(QObject *parent) :
    QObject(parent),
    m_project_host(NULL),
    m_is_open(false),
    m_page_manager(new QPageManager),
    m_language_manager(new QLanguageManager),
    m_user_manager(new QUserManager),
    m_data_manager(new QDataManager),
    m_resource_manager(new QResourceManager),
    m_driver_manager(new QDriverManager)
{
    connect(m_user_manager,SIGNAL(refresh(tagUserInfo*)),this,SLOT(user_refresh(tagUserInfo*)));
    connect(m_language_manager,SIGNAL(refresh(QLanguage*)),this,SLOT(language_refresh(QLanguage*)));
    connect(m_page_manager,SIGNAL(host_name_changed(QAbstractHost*)),this,SLOT(form_refresh(QAbstractHost*)));
}

QProjectCore::~QProjectCore()
{
    close();
    delete m_page_manager;
    delete m_language_manager;
    delete m_user_manager;
    delete m_data_manager;
    delete m_resource_manager;
    delete m_driver_manager;
}

bool QProjectCore::open(const QString &fileName)
{
    close();
    QFile f(fileName);

    if(!f.open(QFile::ReadOnly))
    {
        return false;
    }
    QString buffer=f.readAll();
    f.close();
    XMLObject xml;
    if(!xml.load(buffer,0))
    {
        return false;
    }

    if(xml.getTagName()!=PROJECT_HOST_TITLE)
    {
        return false;
    }

    int index=fileName.lastIndexOf("/");
    m_project_path=fileName.left(index);
    index=m_project_path.lastIndexOf("/");
    m_project_name=m_project_path.mid(index+1);

    m_project_host=new QProjectHost;
    m_project_host->init();
    m_project_host->fromObject(&xml);
    m_project_host->setPropertyValue("objectName",m_project_name);
    m_project_host->setPropertyValue("projectPath",m_project_path);

    m_page_manager->load(m_project_path);
    m_language_manager->load(m_project_path);
    m_data_manager->load(m_project_path);
    m_resource_manager->load(m_project_path);
    m_driver_manager->load(m_project_path);
    m_user_manager->load(m_project_path);

    QAbstractProperty* pro=m_project_host->getProperty("start_user");
    if(pro!=NULL)
    {
        QString str=pro->get_value().toString();
        if(m_user_manager->get_user(str)==NULL)
        {
            if(m_user_manager->get_users().size()>0)
            {
                pro->set_value(m_user_manager->get_user(0)->m_uuid);
            }
            else
            {
                pro->set_value("");
            }
        }

        QList<tagUserInfo*> list=m_user_manager->get_users();
        tagComboItem item;
        ComboItems items;
        foreach(tagUserInfo* info,list)
        {
            item.m_text=info->m_name;
            item.m_value=info->m_uuid;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);

    }

    pro=m_project_host->getProperty("start_language");
    if(pro!=NULL)
    {
        QString str=pro->get_value().toString();
        if(m_language_manager->get_language(str)==NULL)
        {
            if(m_language_manager->get_all_languages().size()>0)
            {
                pro->set_value(m_language_manager->get_all_languages().at(0)->getUuid());
            }
            else
            {
                pro->set_value("");
            }
        }

        m_language_manager->set_current_language(pro->get_value().toString());

        QList<QLanguage*> list=m_language_manager->get_all_languages();
        tagComboItem item;
        ComboItems items;
        foreach(QLanguage* l,list)
        {
            item.m_text=l->get_language_name();
            item.m_value=l->getUuid();
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);

    }

    pro=m_project_host->getProperty("start_page");
    if(pro!=NULL)
    {
        QString str=pro->get_value().toString();
        QList<QAbstractHost*> pages=m_page_manager->getPages_by_title("form");
        if(m_page_manager->get_page(str)==NULL)
        {
            if(pages.size()>0)
            {
                pro->set_value(pages.at(0)->getUuid());
            }
            else
            {
                pro->set_value("");
            }
        }

        tagComboItem item;
        ComboItems items;
        foreach(QAbstractHost* p,pages)
        {
            item.m_text=p->getPropertyValue("objectName").toString();
            item.m_value=p->getUuid();
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);

    }

    m_project_host->setLanguageManager(m_language_manager);
    m_project_host->setPageManager(m_page_manager);

    foreach(QAbstractHost* h,m_page_manager->getPages())
    {
        h->setLanguageManager(m_language_manager);
        h->setPageManager(m_page_manager);
    }

    m_project_host->setDefault();
    m_is_open=true;

    emit opened_signals();
    return true;
}

void QProjectCore::close()
{
    if(m_is_open)
    {
        closed_signals();
        if(m_project_host!=NULL)
        {
            delete m_project_host;
            m_project_host=NULL;
        }

        m_project_path="";

        m_page_manager->clear();

        m_language_manager->clear();

        m_data_manager->clear();

        m_resource_manager->clear();

        m_driver_manager->clear();

        m_user_manager->clear();

        m_is_open=false;
    }
}


void QProjectCore::save()
{
    if(!m_is_open)
    {
        return;
    }

    QDir dir;

    dir.mkpath(m_project_path+"/pages/form");
    dir.mkpath(m_project_path+"/pages/dialog");
    dir.mkpath(m_project_path+"/pages/keyboard");
    dir.mkpath(m_project_path+"/translates");

    QFile::remove(m_project_path+"/config.sfb");

    XMLObject xml;

    m_project_host->toObject(&xml);

    QString str=xml.write();
    QFile f(m_project_path+"/config.sfb");
    if(f.open(QFile::ReadWrite))
    {
        f.resize(0);
        f.write(str.toLocal8Bit());
        f.close();
    }

    m_page_manager->save(m_project_path);

    m_language_manager->save(m_project_path);

    m_data_manager->save(m_project_path);

    m_resource_manager->save(m_project_path);

    m_driver_manager->save(m_project_path);

    m_user_manager->save(m_project_path);
}

QPageManager* QProjectCore::get_page_manager()
{
    return m_page_manager;
}

QLanguageManager* QProjectCore::getLanguageManager()
{
    return m_language_manager;
}

QUserManager *QProjectCore::get_user_manager()
{
    return m_user_manager;
}

QDataManager* QProjectCore::get_data_manager()
{
    return m_data_manager;
}

QResourceManager* QProjectCore::get_resource_manager()
{
    return m_resource_manager;
}

QDriverManager* QProjectCore::get_driver_manager()
{
    return m_driver_manager;
}

QAbstractHost* QProjectCore::get_project_host()
{
    return m_project_host;
}

bool QProjectCore::create_new(const QString &path, const QString &name)
{
    close();

    QString base_path=path+"/"+name;

    QDir dir;

    dir.mkpath(base_path+"/pages/form");
    dir.mkpath(base_path+"/pages/dialog");
    dir.mkpath(base_path+"/pages/keyboard");
    dir.mkpath(base_path+"/translates");

    QMap<QString,QString>   copys;

    copys.insert(":/files/config.xml","/config.sfb");
    copys.insert(":/files/default_form.xml","/pages/form/main.xml");
    copys.insert(":/files/pages.xml","/pages.xml");
    copys.insert(":/files/language_1.xml","/translates/中文.xml");
    copys.insert(":/files/language_2.xml","/translates/English.xml");
    copys.insert(":/files/users.xml","/users.xml");
    copys.insert(":/files/data.xml","/data.xml");

    QMapIterator<QString,QString> it(copys);
    while(it.hasNext())
    {
        it.next();
        copy_file(it.key(),base_path+it.value());
    }

    return open(base_path+"/config.sfb");
}

bool QProjectCore::createNewProj(const QString &szfile)
{
    close();
    int index = szfile.lastIndexOf("/");
    m_project_path = szfile.left(index);
    index = m_project_path.lastIndexOf("/");
    m_project_name = m_project_path.mid(index+1);
    m_page_manager->load(NULL);
    m_is_open = true;

    return true;
}


bool QProjectCore::openProj(XMLObject *pXmlObj)
{
    close();

    m_page_manager->load(pXmlObj);
    m_is_open=true;

    emit opened_signals();
    return true;
}

void QProjectCore::saveProj(XMLObject *pXmlObj)
{
    if(!m_is_open)
    {
        return;
    }
    m_page_manager->save(pXmlObj);
}

bool QProjectCore::is_opened()
{
    return m_is_open;
}

void QProjectCore::user_refresh(tagUserInfo *info)
{
    QAbstractProperty* pro=m_project_host->getProperty("start_user");
    if(pro!=NULL)
    {

        QList<tagUserInfo*> list=m_user_manager->get_users();
        tagComboItem item;
        ComboItems items;
        foreach(tagUserInfo* info,list)
        {
            item.m_text=info->m_name;
            item.m_value=info->m_uuid;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);
        if(info!=NULL && pro->get_value().toString()==info->m_uuid)
        {
            pro->set_value("");
            pro->set_value(info->m_uuid);
        }
    }
}

void QProjectCore::language_refresh(QLanguage *language)
{
    QAbstractProperty* pro=m_project_host->getProperty("start_language");
    if(pro!=NULL)
    {

        QList<QLanguage*> list=m_language_manager->get_all_languages();
        tagComboItem item;
        ComboItems items;
        foreach(QLanguage* l,list)
        {
            item.m_text=l->get_language_name();
            item.m_value=l->getUuid();
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);
        if(language!=NULL && pro->get_value().toString()==language->getUuid())
        {
            pro->set_value("");
            pro->set_value(language->getUuid());
        }
    }
}

void QProjectCore::form_refresh(QAbstractHost *form)
{
    if(form !=NULL && (form->getHostType()!="form" || form->getParent()!=NULL))
    {
        return;
    }
    QAbstractProperty* pro=m_project_host->getProperty("start_page");
    if(pro!=NULL)
    {
        QList<QAbstractHost*> list=m_page_manager->getPages_by_title("form");
        tagComboItem item;
        ComboItems items;
        foreach(QAbstractHost* host,list)
        {
            item.m_text=host->getPropertyValue("objectName").toString();
            item.m_value=host->getUuid();
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);
        if(form!=NULL && pro->get_value().toString()==form->getUuid())
        {
            pro->set_value("");
            pro->set_value(form->getUuid());
        }
    }
}

QAbstractHost* QProjectCore::get_host_by_uuid(const QString &uuid)
{
    QList<QAbstractHost*> list=m_page_manager->getPages();
    list.append(m_project_host);

    while(list.size()>0)
    {
        QAbstractHost* h=list.takeFirst();
        if(h->getUuid()==uuid)
        {
            return h;
        }
        list+=h->getChildren();
    }
    QList<QAbstractDriver*> drivers=m_driver_manager->get_drivers();

    foreach(QAbstractDriver* driver,drivers)
    {
        if(driver->getUuid()==uuid)
        {
            return driver;
        }
    }

    return NULL;
}

void QProjectCore::copy_file(const QString &old_file, const QString &new_file)
{
    QFile old(old_file);
    QFile now(new_file);
    if(old.open(QFile::ReadOnly))
    {
        if(now.open(QFile::WriteOnly))
        {
            now.write(old.readAll());
            now.close();
        }
        old.close();
    }
}

void QProjectCore::init_script_engine()
{
    init_script_engine(m_project_host);


    foreach(QAbstractHost* h,m_page_manager->getPages())
    {
        init_script_engine(h);
    }
}

void QProjectCore::init_script_engine(QAbstractHost *host)
{
    QScriptEngine *engine=host->getScriptEngine();

    QScriptValue global = engine->globalObject();

    QScriptValue temp=get_script_object(m_project_host,engine);

    QList<QData*> groups=m_data_manager->get_all_datas();

    foreach(QData *g,groups)
    {
        if(g->get_datas().size()>0)
        {
            QScriptValue s=engine->newQObject(g);
            temp.setProperty(g->get_name(),s);
        }
    }

    global.setProperty("global",temp);

    bool keyboard=host->property("title").toString()=="keyboard";
    bool project=host->property("title").toString()=="Project";

    if(!keyboard || project)
    {
        foreach(QAbstractHost* h,m_page_manager->getPages())
        {
            if(project || h->property("title").toString()=="keyboard")
            {
                temp=get_script_object(host,engine);
                global.setProperty(h->getPropertyValue("objectName").toString(),temp);
            }
        }
    }

    temp=get_script_object(host,engine);
    global.setProperty("self",temp);
    if(!keyboard || project)
    {
        if(host->getParent()!=NULL)
        {
            temp=get_script_object(host->getParent(),engine);
            global.setProperty("parent",temp);
        }
    }
    else
    {
        temp=get_script_object(host,engine);
        global.setProperty(host->getPropertyValue("objectName").toString(),temp);
    }

    engine->setGlobalObject(global);

    foreach(QAbstractHost* h,host->getChildren())
    {
        init_script_engine(h);
    }
}

QScriptValue QProjectCore::get_script_object(QAbstractHost *host,QScriptEngine *engine)
{
    QScriptValue value=engine->newQObject(host);

    foreach(QAbstractHost* h,host->getChildren())
    {
        QScriptValue temp=get_script_object(h,engine);
        value.setProperty(h->getPropertyValue("objectName").toString(),temp);
    }
    return value;
}
