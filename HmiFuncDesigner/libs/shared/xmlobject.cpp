#include "xmlobject.h"

#include "qpropertyfactory.h"

XMLObject::XMLObject(XMLObject *parent) :
    m_parent(parent)
{
    if(m_parent!=NULL)
    {
        m_parent->m_children.append(this);
    }
}

XMLObject::~XMLObject()
{
    clear();
    if(m_parent!=NULL)
    {
        m_parent->m_children.removeAll(this);
    }
}

void XMLObject::clear()
{
    while(m_children.size()>0)
    {
        delete m_children.first();
    }

    m_property.clear();
    m_title="";
}

QList<XMLObject*> XMLObject::getChildren()
{
    return m_children;
}

QString XMLObject::get_property(const QString &name)
{
    return m_property.value(name);
}

QString XMLObject::get_title()
{
    return m_title;
}

void XMLObject::inser_child(int index, XMLObject *child)
{
    if(child->m_parent==this)
    {
        return;
    }

    if(child->m_parent!=NULL)
    {
        child->m_parent->m_children.removeAll(child);

    }
    child->m_parent=this;
    if(index<0 || index>=m_children.size())
    {
        index=m_children.size();
    }
    m_children.insert(index,child);
}

void XMLObject::set_property(const QString &name, const QString &value)
{
    m_property.insert(name,value);
}

void XMLObject::set_title(const QString &title)
{
    m_title=title;
}

bool XMLObject::load(const QString &buffer, QString *error)
{
    QXmlStreamReader r(buffer);
    while(!r.atEnd())
    {
        if(r.readNext()==QXmlStreamReader::StartElement)
        {
            load(&r);
            if(r.hasError())
            {
                if(error!=NULL)
                {
                    *error=r.errorString();
                }
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

void XMLObject::load(QXmlStreamReader *r)
{
    clear();
    set_title(r->name().toString());
    foreach(QXmlStreamAttribute attr,r->attributes())
    {
        set_property(attr.name().toString(),attr.value().toString());
    }
    bool b=true;
    while(b && !r->hasError())
    {
        switch(r->readNext())
        {
        case QXmlStreamReader::StartElement:
        {
            XMLObject *xml=new XMLObject(this);
            xml->load(r);
        }
            break;
        case QXmlStreamReader::EndElement:
            b=false;
            break;
        case QXmlStreamReader::Characters:
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
    if(m_title=="")
    {
        return;
    }
    w->writeStartElement(m_title);

    QMapIterator<QString,QString> it(m_property);
    while(it.hasNext())
    {
        it.next();
        w->writeAttribute(it.key(),it.value());
    }

    foreach(XMLObject* xml,m_children)
    {
        xml->write(w);
    }

    w->writeEndElement();
}

QMap<QString,QString> XMLObject::get_propertys()
{
    return m_property;
}
