#include "xmlobject.h"
#include "qpropertyfactory.h"

XMLObject::XMLObject(XMLObject *parent) :
    m_parent(parent)
{
    if(m_parent != NULL) {
        m_parent->m_children.append(this);
    }
}

XMLObject::~XMLObject()
{
    clear();
    if(m_parent != NULL) {
        m_parent->m_children.removeAll(this);
    }
}

void XMLObject::clear()
{
    while(m_children.size() > 0) {
        delete m_children.first();
    }

    m_property.clear();
    m_tagName = "";
    m_text = "";
}

QList<XMLObject*> XMLObject::getChildren()
{
    return m_children;
}


//
// 获取当前节点的名称为name的子节点
//
XMLObject* XMLObject::getCurrentChild(const QString& name)
{
    foreach(XMLObject* xml, m_children) {
        if(xml->getTagName() == name) {
            return xml;
        }
    }
    return NULL;
}


//
// 获取当前节点的所有名称为name的子节点
//
QVector<XMLObject* > XMLObject::getCurrentChildren(const QString& name)
{
    QVector<XMLObject*> children;
    foreach(XMLObject* xml, m_children) {
        if(xml->getTagName() == name) {
            children.append(xml);
        }
    }
    return children;
}

QString XMLObject::getProperty(const QString &name)
{
    return m_property.value(name);
}

QString XMLObject::getTagName()
{
    return m_tagName;
}

void XMLObject::inser_child(int index, XMLObject *child)
{
    if(child->m_parent == this) {
        return;
    }

    if(child->m_parent != NULL) {
        child->m_parent->m_children.removeAll(child);

    }
    child->m_parent = this;
    if(index < 0 || index >= m_children.size()) {
        index = m_children.size();
    }
    m_children.insert(index, child);
}

void XMLObject::setProperty(const QString &name, const QString &value)
{
    m_property.insert(name, value);
}

void XMLObject::setTagName(const QString &title)
{
    m_tagName = title;
}


//
// 获取标签文本
// <person age="29" name="jason" sex="male">hi, my name is jason.</person>
// 标签文本为 hi, my name is jason.
//
QString XMLObject::getText()
{
    return m_text;
}

//
// 设置标签文本
// setText("hello, jack!")
// <person name="jason">hello, jack!</person>
//
void XMLObject::setText(const QString &text)
{
    m_text = text;
}

bool XMLObject::load(const QString &buffer, QString *error)
{
    QXmlStreamReader r(buffer);
    while(!r.atEnd()) {
        if(r.readNext() == QXmlStreamReader::StartElement) {
            load(&r);
            if(r.hasError()) {
                if(error != NULL) {
                    *error = r.errorString();
                }
                return false;
            } else {
                return true;
            }
        }
    }
    return false;
}

void XMLObject::load(QXmlStreamReader *r)
{
    clear();
    setTagName(r->name().toString());
    foreach(QXmlStreamAttribute attr, r->attributes()) {
        setProperty(attr.name().toString(), attr.value().toString());
    }
    bool b = true;
    while(b && !r->hasError()) {
        switch(r->readNext()) {
            case QXmlStreamReader::StartElement: {
                XMLObject *xml = new XMLObject(this);
                xml->load(r);
            }
            break;
            case QXmlStreamReader::EndElement:
                b = false;
                break;
            case QXmlStreamReader::Characters:
                if(!r->isWhitespace()) {
                    setText(r->text().toString());
                }
                break;
            default:
                break;
        }
    }
}

QString XMLObject::write()
{
    QString ret;
    QXmlStreamWriter w(&ret);

    w.writeStartDocument();
    w.setAutoFormatting(true);
    write(&w);
    w.writeEndDocument();

    return ret;
}

void XMLObject::write(QXmlStreamWriter *w)
{
    if(m_tagName == "") {
        return;
    }
    w->writeStartElement(m_tagName);

    QMapIterator<QString, QString> it(m_property);
    while(it.hasNext()) {
        it.next();
        w->writeAttribute(it.key(), it.value());
    }

    if(m_text != "") {
        w->writeCharacters(m_text);
    }

    foreach(XMLObject* xml, m_children) {
        xml->write(w);
    }

    w->writeEndElement();
}

QMap<QString, QString> XMLObject::getPropertys()
{
    return m_property;
}
