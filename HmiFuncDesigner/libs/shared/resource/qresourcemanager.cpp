#include "qresourcemanager.h"


#include "qresourceregister.h"
#include "qresourcefile.h"

QResourceManager::QResourceManager(QObject *parent) :
    QObject(parent),
    m_file(new QResourceFile)
{
}

QResourceManager::~QResourceManager()
{
    delete m_file;
}

void QResourceManager::load(const QString &path)
{
    m_file->load(path);
    m_resourceNumber.clear();
    if(QResourceRegister::reg(m_file))
    {
        QStringList names=m_file->get_all_file();
        foreach(QString name,names)
        {
            m_resourceNumber.insert(name,1);
        }
    }

}

void QResourceManager::save(const QString &path)
{
    m_file->save(path);
}

void QResourceManager::clear()
{
    QResourceRegister::unreg();
    m_file->clear();
    m_resourceNumber.clear();
}

void QResourceManager::addResource(const QString &name, const QByteArray &data)
{
    int k=m_resourceNumber.value(name,0);
    k++;
    if(k==1)
    {
        m_file->addResource(name,data);
        QResourceRegister::reg(m_file);
    }
    m_resourceNumber.insert(name,k);
}

void QResourceManager::removeResource(const QString &name)
{
    int k=m_resourceNumber.value(name);
    k--;
    if(k==0)
    {
        m_file->removeResource(name);
        if(m_file->get_all_file_info().size()>0)
        {
            QResourceRegister::reg(m_file);
        }
        else
        {
            QResourceRegister::unreg();
        }
        m_resourceNumber.remove(name);
    }
    else
    {
        m_resourceNumber.insert(name,k);
    }
}

QResourceFile* QResourceManager::get_file()
{
    return m_file;
}
