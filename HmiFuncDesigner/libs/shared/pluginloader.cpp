#include "pluginloader.h"

#include "qabstractplugin.h"
#include "xmlobject.h"

#include <QApplication>
#include <QPluginLoader>
#include <QDir>

QMap<QString,QMap<QString,QAbstractPlugin*> > PluginLoader::m_plugins;


void PluginLoader::loadPlugin(const QString &xml_file_name)
{
    releasePlugin();

    XMLObject xml;
    QFile f(xml_file_name);
    if(!f.open(QFile::ReadOnly))
    {
        return;
    }
    QString str=f.readAll();
    if(!xml.load(str,0))
    {
        return;
    }
    if(xml.getTagName()!="Plugin")
    {
        return;
    }
    QList<XMLObject*> paths=xml.getChildren();
    QString path=QApplication::applicationDirPath();
    foreach(XMLObject* obj,paths)
    {
        QString p=path+"/"+obj->getProperty("path");//获取插件路径
        QDir dir(p);
        QFileInfoList list=dir.entryInfoList(QDir::Files);
        for(int i=0;i<list.size();i++)
        {
            QPluginLoader load(list.at(i).filePath());

            if(load.load())
            {
                QAbstractPlugin *plugin=qobject_cast<QAbstractPlugin*>(load.instance());
                if(plugin!=Q_NULLPTR)
                {
                    QString type = plugin->getPluginType();
                    QString name = plugin->getPluginName();
                    if(type!="" && name!="")
                    {
                        QMap<QString,QAbstractPlugin*> plugins=m_plugins.value(type);
                        QAbstractPlugin *pl=plugins.value(name);
                        if(pl!=Q_NULLPTR)
                        {
                            delete pl;
                        }
                        plugins.insert(name,plugin);
                        m_plugins.insert(type,plugins);
                        plugin->initPlugin();
                    }
                    else
                    {
                        delete plugin;
                    }
                }
            }
        }
    }
}

QMap<QString,QAbstractPlugin*> PluginLoader::getPluginByType(const QString &type)
{
    return m_plugins.value(type);
}

QAbstractPlugin* PluginLoader::getPlugin(const QString &type, const QString &name)
{
    return m_plugins.value(type).value(name);
}

void PluginLoader::releasePlugin()
{
    QMapIterator<QString,QMap<QString,QAbstractPlugin*> > it(m_plugins);

    while(it.hasNext())
    {
        it.next();
        QMapIterator<QString,QAbstractPlugin*>  t(it.value());
        while(t.hasNext())
        {
            t.next();
            delete t.value();
        }
    }
    m_plugins.clear();
}
