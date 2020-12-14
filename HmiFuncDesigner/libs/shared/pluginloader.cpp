#include "pluginloader.h"

#include "qabstractplugin.h"
#include "xmlobject.h"

#include <QApplication>
#include <QPluginLoader>
#include <QDir>

QMap<QString,QMap<QString,QAbstractPlugin*> > PluginLoader::m_plugins;


void PluginLoader::load_plugin(const QString &xml_file_name)
{
    release_plugin();

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
    if(xml.get_title()!="Plugin")
    {
        return;
    }
    QList<XMLObject*> paths=xml.getChildren();
    QString path=QApplication::applicationDirPath();
    foreach(XMLObject* obj,paths)
    {
        QString p=path+"/"+obj->get_property("path");//获取插件路径
        QDir dir(p);
        QFileInfoList list=dir.entryInfoList(QDir::Files);
        for(int i=0;i<list.size();i++)
        {
            QPluginLoader load(list.at(i).filePath());

            if(load.load())
            {
                QAbstractPlugin *plugin=qobject_cast<QAbstractPlugin*>(load.instance());
                if(plugin!=NULL)
                {
                    QString type=plugin->get_plugin_type();
                    QString name=plugin->get_plugin_name();
                    if(type!="" && name!="")
                    {
                        QMap<QString,QAbstractPlugin*> plugins=m_plugins.value(type);
                        QAbstractPlugin *pl=plugins.value(name);
                        if(pl!=NULL)
                        {
                            delete pl;
                        }
                        plugins.insert(name,plugin);
                        m_plugins.insert(type,plugins);
                        plugin->init_plugin();
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

QMap<QString,QAbstractPlugin*> PluginLoader::get_plugin_by_type(const QString &type)
{
    return m_plugins.value(type);
}

QAbstractPlugin* PluginLoader::get_plugin(const QString &type, const QString &name)
{
    return m_plugins.value(type).value(name);
}

void PluginLoader::release_plugin()
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
