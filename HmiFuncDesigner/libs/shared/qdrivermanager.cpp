#include "qdrivermanager.h"

#include "driver/qabstractdriver.h"
#include "xmlobject.h"
#include "qdriverfactory.h"

#include <QFile>

QDriverManager::QDriverManager(QObject *parent) :
    QObject(parent)
{
}

QDriverManager::~QDriverManager()
{
    clear();
}

void QDriverManager::save(const QString &path)
{
    XMLObject xml;

    xml.set_title("Drivers");

    foreach(QAbstractDriver* driver,m_drivers)
    {
        XMLObject* o=new XMLObject(&xml);
        o->set_title("Driver");
        o->set_property("type",driver->get_attribute(HOST_TYPE));
        o->set_property("name",driver->get_name());
        driver->save(path+"/drivers/"+driver->get_name()+".xml");
    }

    QString str=xml.write();

    QFile f(path+"/drivers.xml");
    if(!f.open(QFile::WriteOnly))
    {
        return;
    }
    f.resize(0);
    f.write(str.toLocal8Bit());
    f.close();
}

void QDriverManager::load(const QString &path)
{
    QFile f(path+"/drivers.xml");
    if(!f.open(QFile::ReadOnly))
    {
        return;
    }

    QString buff=f.readAll();

    XMLObject xml;
    if(!xml.load(buff,0))
    {
        return;
    }

    if(xml.get_title()!="Drivers")
    {
        return;
    }

    QList<XMLObject*>   list=xml.getChildren();

    foreach(XMLObject* c,list)
    {
        if(c->get_title()=="Driver")
        {
            QString type=c->get_property("type");
            QString name=c->get_property("name");
            QAbstractDriver *driver=QDriverFactory::create_driver(type);

            if(driver!=NULL)
            {
                if(driver->load(path+"/drivers/"+name+".xml"))
                {
                    m_drivers.append(driver);
                    m_uuid_to_driver.insert(driver->get_uuid(),driver);
                    driver->set_default();
                }
                else
                {
                    delete driver;
                }
            }
        }
    }
}

void QDriverManager::clear()
{
    foreach(QAbstractDriver* driver,m_drivers)
    {
        driver->deleteLater();
    }
    m_drivers.clear();

    m_uuid_to_driver.clear();
}

QList<QAbstractDriver*> QDriverManager::get_drivers()
{
    return m_drivers;
}

QAbstractDriver* QDriverManager::get_driver(const QString &uuid)
{
    return m_uuid_to_driver.value(uuid);
}

void QDriverManager::remove_driver(const QString &uuid)
{
    QAbstractDriver* driver=m_uuid_to_driver.value(uuid);
    if(driver!=NULL)
    {
        emit remove_driver_signal(driver);
        m_uuid_to_driver.remove(uuid);
        m_drivers.removeAll(driver);
    }
}

void QDriverManager::insert_driver(int index, QAbstractDriver *driver)
{
    if(index<0 || index>m_drivers.size())
    {
        index=m_drivers.size();
    }

    m_drivers.insert(index,driver);
    m_uuid_to_driver.insert(driver->get_uuid(),driver);
    emit insert_driver_signal(driver,index);
}
