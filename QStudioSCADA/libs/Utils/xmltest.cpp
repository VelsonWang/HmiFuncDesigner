
#if 0

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "xmlobject.h"





void load(const QString &project_path)
{
    //qDebug() << "load()" << project_path+"/pages.xml";
    QFile f(project_path+"/pages.xml");
    if(!f.open(QFile::ReadOnly))
    {
        return;
    }
    //qDebug() << "read file...";
    QString buffer=f.readAll();
    //qDebug() << buffer << "\n\n";
    f.close();
    XMLObject xml;
    if(!xml.load(buffer,0))
    {
        //qDebug() << "xml.load false";
        return;
    }

    QList<XMLObject*> children=xml.get_children();
    //qDebug() << "QList<XMLObject*>.size(): " << children.size();

    foreach(XMLObject* obj,children)
    {
        qDebug() << obj->get_title() << obj->get_property("path")  << obj->get_text() << "\n";
    }

}

void save(const QString &project_path)
{
    XMLObject path;
    path.set_title("Pages");

    XMLObject page;

    XMLObject *temp;

    QString p;
    QString old_path;


    temp=new XMLObject(&path);
    temp->set_title("Page");
    temp->set_text("1989");
    temp->set_property("path", QCoreApplication::applicationDirPath()+"/test.xml");
    page.clear();

    QFile f(project_path+"/pages.xml");
    if(f.open(QFile::WriteOnly))
    {
        f.resize(0);
        f.write(path.write().toLocal8Bit());
        f.close();
    }
}








int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    qDebug() << a.applicationDirPath();
    save(a.applicationDirPath());
    load(a.applicationDirPath());

    return 0;
}

#endif
