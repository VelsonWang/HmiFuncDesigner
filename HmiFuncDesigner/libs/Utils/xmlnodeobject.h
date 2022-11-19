#ifndef XMLNODEOBJECT_H
#define XMLNODEOBJECT_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class XMLNodeObject : public QObject
{
    Q_OBJECT
public:
    explicit XMLNodeObject(XMLNodeObject *parent = 0);
    ~XMLNodeObject();
    void clear();

    QList<XMLNodeObject* > getChildren();

    QString getProperty(const QString& name);
    void setProperty(const QString& name, const QString& value);

    QString getTagName();
    void setTagName(const QString& tagName);

    QString getText();
    void setText(const QString& text);

    void insertChild(int index, XMLNodeObject* child);

    bool load(const QString &buffer, QString *error);
    void load(QXmlStreamReader *r);

    QString write();
    void write(QXmlStreamWriter *w);

    QMap<QString, QString> getPropertys();
    XMLNodeObject* getCurrentChild(const QString& name);
    QList<XMLNodeObject* > getCurrentChildren(const QString& name);
    void getChildrenByParentTagName(QStringList &tagNames, QList<XMLNodeObject* > &children);

    void showXMLObject();

private:
    QList<XMLNodeObject*> m_children;
    QMap<QString,QString> m_property;
    XMLNodeObject *m_parent;
    QString m_tagName;
    QString m_text;
};

#endif // XMLNODEOBJECT_H
