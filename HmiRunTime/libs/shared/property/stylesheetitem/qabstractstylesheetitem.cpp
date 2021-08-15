#include "qabstractstylesheetitem.h"

#include "../../xmlobject.h"

QAbstractStylesheetItem::QAbstractStylesheetItem(QAbstractStylesheetItem *parent) :
    QObject(parent),
    m_parent(parent),
    m_value("")
{
    if(m_parent != Q_NULLPTR) {
        m_parent->m_children.append(this);
        connect(this, SIGNAL(valueChanged()), m_parent, SLOT(subValueChanged()));
    }
}

QAbstractStylesheetItem::~QAbstractStylesheetItem()
{
    while(m_children.size() > 0) {
        delete m_children.first();
    }

    if(m_parent != Q_NULLPTR) {
        m_parent->m_children.removeAll(this);
    }
}

QVariant QAbstractStylesheetItem::value()
{
    return m_value;
}

void QAbstractStylesheetItem::setValue(const QVariant &value)
{
    if(m_value != value) {
        m_value = value;
        emit valueChanged();
    }
}

QString QAbstractStylesheetItem::attribute(const QString &key, const QString &def)
{
    return m_attributes.value(key, def);
}

void QAbstractStylesheetItem::setAttribute(const QString &key, const QString &value)
{
    m_attributes.insert(key, value);
}

void QAbstractStylesheetItem::read(XMLObject *xml)
{
    QString value;
    bool setvalue = false;
    m_attributes = xml->getPropertys();
    if(m_attributes.keys().contains("value")) {
        value = m_attributes.value("value");
        setvalue = true;
        m_attributes.remove("value");
    }

    QAbstractStylesheetItem *item;

    QList<XMLObject*> list = xml->getChildren();

    foreach(XMLObject* c, list) {
        item = getChild(c->getProperty("title"));
        if(item != Q_NULLPTR) {
            item->read(c);
        }
    }

    if(setvalue) {
        setValue(value);
    }
}

void QAbstractStylesheetItem::write(XMLObject *xml)
{
    if(!isUsed()) {
        return;
    }

    xml->setTagName("Item");

    QMapIterator<QString, QString> it(m_attributes);
    while(it.hasNext()) {
        it.next();
        xml->setProperty(it.key(), it.value());
    }

    xml->setProperty("title", property("title").toString());
    if(m_children.size() == 0) {
        xml->setProperty("value", m_value.toString());
    } else {
        foreach(QAbstractStylesheetItem* item, m_children) {
            if(item->isUsed()) {
                XMLObject *o = new XMLObject(xml);
                item->write(o);
            }
        }
    }
}

QAbstractStylesheetItem* QAbstractStylesheetItem::getChild(const QString &name)
{
    foreach(QAbstractStylesheetItem* item, m_children) {
        if(item->property("title") == name) {
            return item;
        }
    }
    return Q_NULLPTR;
}

void QAbstractStylesheetItem::subValueChanged()
{

}

QString QAbstractStylesheetItem::makeStylesheet()
{
    return "";
}

bool QAbstractStylesheetItem::isUsed()
{
    if(m_children.size() > 0) {
        foreach(QAbstractStylesheetItem* item, m_children) {
            if(item->isUsed()) {
                return true;
            }
        }
        return false;
    } else {
        if(m_value.toString() == property("default").toString() || m_value.toString() == "") {
            return false;
        }
        return true;
    }
}

void QAbstractStylesheetItem::setDefault(const QVariant &value)
{
    setProperty("default", value.toString());
    m_value = value;
}

void QAbstractStylesheetItem::clear()
{
    m_attributes.clear();
    m_value = "";
    foreach(QAbstractStylesheetItem* item, m_children) {
        item->clear();
    }
}
