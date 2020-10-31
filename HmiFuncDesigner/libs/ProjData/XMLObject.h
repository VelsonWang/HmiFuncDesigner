#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "projdata_global.h"

class PROJDATASHARED_EXPORT XMLObject : public QObject
{
    Q_OBJECT
public:
    explicit XMLObject(XMLObject *parent = 0);
    ~XMLObject();
    void clear();

    QList<XMLObject* > getChildren();

    QString getProperty(const QString& name);
    void setProperty(const QString& name, const QString& value);

    QString getTagName();
    void setTagName(const QString& tagName);

    QString getText();
    void setText(const QString& text);

    void insertChild(int index, XMLObject* child);

    bool load(const QString &buffer, QString *error);
    void load(QXmlStreamReader *r);

    QString write();
    void write(QXmlStreamWriter *w);

    QMap<QString, QString> getPropertys();
    XMLObject* getCurrentChild(const QString& name);
    QList<XMLObject* > getCurrentChildren(const QString& name);
    void getChildrenByParentTagName(QStringList &tagNames, QList<XMLObject* > &children);

    void showXMLObject();

private:
    QList<XMLObject*> m_children;
    QMap<QString,QString> m_property;
    XMLObject *m_parent;
    QString m_tagName;
    QString m_text;
};

#endif // XMLOBJECT_H
