#include "XMLObject.h"

#include <QDebug>

XMLObject::XMLObject(XMLObject *parent) :
    m_parent(parent) {
    if(m_parent != Q_NULLPTR) {
        m_parent->m_children.append(this);
    }
}

XMLObject::~XMLObject() {
    clear();
    if(m_parent != Q_NULLPTR) {
        m_parent->m_children.removeAll(this);
    }
}

void XMLObject::clear() {
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
QVector<XMLObject* > XMLObject::getChildren() {
    return m_children;
}

//
// 获取本节点name属性的值
// <person age="29" name="jason" sex="male">hi, my name is jason.</person>
// age 属性值为29
//
QString XMLObject::getProperty(const QString &name) {
    return m_property.value(name);
}

//
// 设置本节点name属性的值
// setProperty("sex", "female")
// <person sex="female">hi, my name is jason.</person>
//
void XMLObject::setProperty(const QString &name, const QString &value) {
    m_property.insert(name, value);
}

//
// 获取标签名称
// <person age="29" name="jason" sex="male">hi, my name is jason.</person>
// 标签名称为person
//
QString XMLObject::getTagName() {
    return m_tagName;
}

//
// 设置标签名称
// setTagName("page")
// <page name="jason">hi, my name is jason.</page>
// 标签名称为page
//
void XMLObject::setTagName(const QString &tagName) {
    m_tagName = tagName;
}

//
// 获取标签文本
// <person age="29" name="jason" sex="male">hi, my name is jason.</person>
// 标签文本为 hi, my name is jason.
//
QString XMLObject::getText() {
    return m_text;
}

//
// 设置标签文本
// setText("hello, jack!")
// <person name="jason">hello, jack!</person>
//
void XMLObject::setText(const QString &text) {
    m_text = text;
}

void XMLObject::insertChild(int index, XMLObject *child) {
    if(child->m_parent == this)
        return;

    if(child->m_parent != Q_NULLPTR)
        child->m_parent->m_children.removeAll(child);

    child->m_parent = this;

    if(index<0 || index>=m_children.size())
        index = m_children.size();

    m_children.insert(index, child);
}


bool XMLObject::load(const QString &buffer, QString *error) {
    QXmlStreamReader r(buffer);
    while(!r.atEnd()) {
        if(r.readNext() == QXmlStreamReader::StartElement) {
            load(&r);
            if(r.hasError()) {
                if(error!=NULL) {
                    *error=r.errorString();
                }
                return false;
            } else {
                return true;
            }
        }
    }
    return false;
}

void XMLObject::load(QXmlStreamReader *r) {
    clear();
    setTagName(r->name().toString());

    foreach(QXmlStreamAttribute attr, r->attributes())
        setProperty(attr.name().toString(), attr.value().toString());

    bool b = true;
    while(b && !r->hasError()) {
        switch(r->readNext()) {
        case QXmlStreamReader::StartElement: {
            XMLObject *xml = new XMLObject(this);
            xml->load(r);
            break;
        }            
        case QXmlStreamReader::EndElement:
            b = false;
            break;
        case QXmlStreamReader::Characters:
            if(!r->isWhitespace())
                setText(r->text().toString());
            break;
        default:
            break;
        }
    }
}

QString XMLObject::write() {
    QString ret;
    QXmlStreamWriter w(&ret);

    w.writeStartDocument();
    w.setAutoFormatting(true);
    write(&w);
    w.writeEndDocument();

    return ret;
}

void XMLObject::write(QXmlStreamWriter *w) {
    if(m_tagName == "")
        return;
    w->writeStartElement(m_tagName);

    QMapIterator<QString,QString> it(m_property);
    while(it.hasNext()) {
        it.next();
        w->writeAttribute(it.key(), it.value());
    }

    if(m_text != "")
        w->writeCharacters(m_text);

    foreach(XMLObject* xml, m_children)
        xml->write(w);

    w->writeEndElement();
}

//
// 获取当前节点的所有属性值
//
QMap<QString,QString> XMLObject::getPropertys() {
    return m_property;
}


//
// 获取当前节点的名称为name的子节点
//
XMLObject* XMLObject::getCurrentChild(const QString& name) {
    foreach(XMLObject* xml, m_children) {
        if(xml->getTagName() == name)
            return xml;
    }
    return Q_NULLPTR;
}


//
// 获取当前节点的所有名称为name的子节点
//
QVector<XMLObject* > XMLObject::getCurrentChildren(const QString& name) {
    QVector<XMLObject*> children;
    foreach(XMLObject* xml, m_children) {
        if(xml->getTagName() == name)
            children.append(xml);
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
// QList<XMLObject* > children 找到的子节点
//
void XMLObject::getChildrenByParentTagName(QStringList &tagNames,
                                QList<XMLObject* > &children) {
    if(tagNames.size() < 1)
        return;
    QString tagName = tagNames.first();
    if(tagName == getTagName()) {
        if(tagNames.size() == 1) {
            children.append(this);
        } else {
            tagName = tagNames.at(1);
            QVector<XMLObject* > subChildren = getCurrentChildren(tagName);
            tagNames.removeFirst();
            foreach(XMLObject* xml, subChildren) {
                QStringList copyTagNames = tagNames;
                xml->getChildrenByParentTagName(copyTagNames, children);
            }
        }
    }
}


void XMLObject::showXMLObject() {
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
#include "XMLObject.h"
#include "FileHelper.h"
#include <QDebug>

void load(const QString &project_path)
{
    QString buffer = FileHelper::readString(project_path + "/pages.xml");
    XMLObject xml;
    if(!xml.load(buffer, 0))
        return;

    QList<XMLObject*> children = xml.getChildren();
    foreach(XMLObject* obj, children)
        obj->showXMLObject();

    qDebug() << "\n\n\n=========get child name is person=========\n";
    children.clear();
    children = xml.getCurrentChildren("person");
    foreach(XMLObject* obj, children)
        obj->showXMLObject();

    qDebug() << "\n\n\n=========getChildrenByParentTagName=========\n";
    QStringList parents;
    parents<< "persons";
    parents<< "person";
    parents<< "baby";
    children.clear();
    xml.getChildrenByParentTagName(parents, children);
    foreach(XMLObject* obj, children){
        obj->showXMLObject();
        obj->setProperty("cb", "123");
    }
    FileHelper::writeString(project_path + "/pages.xml", xml.write());
}

void save(const QString &project_path)
{
    XMLObject persons;
    persons.setTagName("persons");

    XMLObject *person;
    person = new XMLObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is jason.");
    person->setProperty("name", "jason");
    person->setProperty("age", "29");
    person->setProperty("sex", "male");

    person = new XMLObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is velson.");
    person->setProperty("name", "velson");
    person->setProperty("age", "28");
    person->setProperty("sex", "male");

    person = new XMLObject(&persons);
    person->setTagName("person");
    person->setText("hi, my name is lucy.");
    person->setProperty("name", "lucy");
    person->setProperty("age", "25");
    person->setProperty("sex", "female");

    XMLObject *baby = new XMLObject(person);
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

