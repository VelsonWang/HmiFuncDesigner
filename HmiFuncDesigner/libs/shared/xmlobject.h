#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#include "sharedlibglobal.h"
#include <QObject>
#include <QMap>
#include <QList>
#include <QVector>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class SHAREDLIB_EXPORT XMLObject : public QObject
{
    Q_OBJECT
public:
    explicit XMLObject(XMLObject *parent = 0);
    ~XMLObject();

    QList<XMLObject*>   getChildren();
    QVector<XMLObject* > getCurrentChildren(const QString& name);
    XMLObject* getCurrentChild(const QString& name);
    QString             getProperty(const QString& name);
    void                clear();

    void                inser_child(int index, XMLObject* child);
    void                setProperty(const QString& name, const QString& value);

    QString             getTagName();
    void                setTagName(const QString& title);

    QString getText();
    void setText(const QString& text);

    bool                load(const QString &buffer, QString *error);
    void                load(QXmlStreamReader *r);

    QString             write();
    void                write(QXmlStreamWriter *w);

    QMap<QString, QString> getPropertys();

protected:
    QList<XMLObject*>       m_children;
    QMap<QString, QString>   m_property;
    XMLObject               *m_parent;
    QString m_tagName;
    QString m_text;
};

#endif // XMLOBJECT_H
