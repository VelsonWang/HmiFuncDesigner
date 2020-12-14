#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#include "sharedlibglobal.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class SHAREDLIB_EXPORT XMLObject : public QObject
{
    Q_OBJECT
public:
    explicit XMLObject(XMLObject *parent = 0);
    ~XMLObject();

    QList<XMLObject*>   getChildren();
    QString             get_property(const QString& name);
    QString             get_title();

    void                clear();

    void                inser_child(int index,XMLObject* child);
    void                set_property(const QString& name,const QString& value);
    void                set_title(const QString& title);

    bool                load(const QString &buffer,QString *error);
    void                load(QXmlStreamReader *r);

    QString             write();
    void                write(QXmlStreamWriter *w);

    QMap<QString,QString>   get_propertys();
protected:
    QList<XMLObject*>       m_children;
    QMap<QString,QString>   m_property;
    XMLObject               *m_parent;
    QString                 m_title;
};

#endif // XMLOBJECT_H
