#include "XMLNodeObject.h"

#include <QDebug>

XMLNodeObject::XMLNodeObject(XMLNodeObject *parent) :
    m_parent(parent)
{
    if(m_parent != NULL) {
        m_parent->m_children.append(this);
    }
}

XMLNodeObject::~XMLNodeObject()
{
    clear();
    if(m_parent != NULL) {
        m_parent->m_children.removeAll(this);
    }
}

void XMLNodeObject::clear()
{
    while(m_children.size() > 0) {
        delete m_children.takeFirst();
    }
    m_property.clear();
    m_tagName = "";
    m_text = "";
}

//
// 获取本节点的所有子对象
//
QList<XMLNodeObject* > XMLNodeObject::getChildren()
{
    return m_children;
}

//
// 获取本节点name属性的值
// <person age="29" name="jason" sex="male">hi, my name is jason.</person>
// age 属性值为29
//
QString XMLNodeObject::getProperty(const QString &name)
{
    return m_property.value(name);
}

//
// 设置本节点name属性的值
// setProperty("sex", "female")
// <person sex="female">hi, my name is jason.</person>
//
void XMLNodeObject::setProperty(const QString &name, const QString &value)
{
    m_property.insert(name, value);
}

//
// 获取标签名称
// <person age="29" name="jason" sex="male">hi, my name is jason.</person>
// 标签名称为person
//
QString XMLNodeObject::getTagName()
{
    return m_tagName;
}

//
// 设置标签名称
// setTagName("page")
// <page name="jason">hi, my name is jason.</page>
// 标签名称为page
//
void XMLNodeObject::setTagName(const QString &tagName)
{
    m_tagName = tagName;
}

//
// 获取标签文本
// <person age="29" name="jason" sex="male">hi, my name is jason.</person>
// 标签文本为 hi, my name is jason.
//
QString XMLNodeObject::getText()
{
    return m_text;
}

//
// 设置标签文本
// setText("hello, jack!")
// <person name="jason">hello, jack!</person>
//
void XMLNodeObject::setText(const QString &text)
{
    m_text = text;
}

void XMLNodeObject::insertChild(int index, XMLNodeObject *child)
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


bool XMLNodeObject::load(const QString &buffer, QString *error)
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

void XMLNodeObject::load(QXmlStreamReader *r)
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
                XMLNodeObject *xml = new XMLNodeObject(this);
                xml->load(r);
                break;
            }
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

QString XMLNodeObject::write()
{
    QString ret;
    QXmlStreamWriter w(&ret);

    w.writeStartDocument();
    w.setAutoFormatting(true);
    write(&w);
    w.writeEndDocument();

    return ret;
}

void XMLNodeObject::write(QXmlStreamWriter *w)
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

    foreach(XMLNodeObject* xml, m_children) {
        xml->write(w);
    }

    w->writeEndElement();
}

//
// 获取当前节点的所有属性值
//
QMap<QString, QString> XMLNodeObject::getPropertys()
{
    return m_property;
}


//
// 获取当前节点的名称为name的子节点
//
XMLNodeObject* XMLNodeObject::getCurrentChild(const QString& name)
{
    foreach(XMLNodeObject* xml, m_children) {
        if(xml->getTagName() == name) {
            return xml;
        }
    }
    return NULL;
}


//
// 获取当前节点的所有名称为name的子节点
//
QList<XMLNodeObject* > XMLNodeObject::getCurrentChildren(const QString& name)
{
    QList<XMLNodeObject*> children;
    foreach(XMLNodeObject* xml, m_children) {
        if(xml->getTagName() == name) {
            children.append(xml);
        }
    }
    return children;
}

//
// 在当前节点下根据父子节点名称查找子节点
//
//<root_parent>
//  <parent1>
//      <child>
//          target is mime.
//      </child>
//  </parent1>
//</root_parent>
//
// QStringList *tagNmaes 形如：
// tagNmaes(0)=root_parent
// tagNmaes(1)=parent1
// tagNmaes(3)=child
// QList<XMLNodeObject* > children 找到的子节点
//
void XMLNodeObject::getChildrenByParentTagName(QStringList &tagNames,
        QList<XMLNodeObject* > &children)
{
    if(tagNames.size() < 1) {
        return;
    }
    QString tagName = tagNames.first();
    if(tagName == getTagName()) {
        if(tagNames.size() == 1) {
            children.append(this);
        } else {
            tagName = tagNames.at(1);
            QList<XMLNodeObject* > subChildren = getCurrentChildren(tagName);
            tagNames.removeFirst();
            foreach(XMLNodeObject* xml, subChildren) {
                QStringList copyTagNames = tagNames;
                xml->getChildrenByParentTagName(copyTagNames, children);
            }
        }
    }
}


void XMLNodeObject::showXMLObject()
{
    qDebug() << "====================================================";
    qDebug() << "TagName: " << getTagName();
    qDebug() << "Text: " << getText();
    qDebug() << "Propertys: ";
    QMap<QString, QString> propertys = getPropertys();
    QMap<QString, QString>::const_iterator iter = propertys.begin();
    while (iter != propertys.end()) {
        qDebug() << "\t" << iter.key() << "=" << iter.value();
        ++iter;
    }
    qDebug() << "\n";
}

#if 0

#include <QCoreApplication>
#include <QFile>
#include "XMLNodeObject.h"
#include "FileHelper.h"
#include <QDebug>

void load(const QString &project_path)
{
    QString buffer = FileHelper::readString(project_path + "/pages.xml");
    XMLNodeObject xml;
    if(!xml.load(buffer, 0)) {
        return;
    }

    QList<XMLNodeObject*> children = xml.getChildren();
    foreach(XMLNodeObject* obj, children) {
        obj->showXMLNodeObject();
    }

    qDebug() << "\n\n\n=========get child name is person=========\n";
    children.clear();
    children = xml.getCurrentChildren("person");
    foreach(XMLNodeObject* obj, children) {
        obj->showXMLNodeObject();
    }

    qDebug() << "\n\n\n=========getChildrenByParentTagName=========\n";
    QStringList parents;
    parents << "persons";
    parents << "person";
    parents << "baby";
    children.clear();
    xml.getChildrenByParentTagName(parents, children);
    foreach(XMLNodeObject* obj, children) {
        obj->showXMLNodeObject();
        obj->setProperty("cb", "123");
    }
    FileHelper::writeString(project_path + "/pages.xml", xml.write());
}

void save(const QString &project_path)
{
    XMLNodeObject persons;
    persons.setTagName("persons");

    XMLNodeObject *person;
    person = new XMLNodeObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is jason.");
    person->setProperty("name", "jason");
    person->setProperty("age", "29");
    person->setProperty("sex", "male");

    person = new XMLNodeObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is velson.");
    person->setProperty("name", "velson");
    person->setProperty("age", "28");
    person->setProperty("sex", "male");

    person = new XMLNodeObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is lucy.");
    person->setProperty("name", "lucy");
    person->setProperty("age", "25");
    person->setProperty("sex", "female");

    XMLNodeObject *baby = new XMLNodeObject(person);
    baby->setTagName("baby");
    baby->setText("lucy==baby");
    baby->setProperty("name", "baby");
    baby->setProperty("age", "1");
    baby->setProperty("sex", "female");

    FileHelper::writeString(project_path + "/pages.xml", persons.write());

}




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    save(QCoreApplication::applicationDirPath());
    load(QCoreApplication::applicationDirPath());
    return a.exec();
}


#endif

