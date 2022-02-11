#include "qabstractproperty.h"
#include "../xmlobject.h"
#include <QTimer>

QAbstractProperty::QAbstractProperty(QAbstractProperty *parent) :
    m_parent(parent)
{
    if(m_parent != NULL) {
        parent->m_children.append(this);
        connect(this, SIGNAL(value_chaged(QVariant, QVariant)), m_parent, SLOT(child_value_changed(QVariant, QVariant)));
    }
    setAttribute(ATTR_RESET_ABLEABLE, m_parent == NULL);
    setAttribute(ATTR_VISIBLE, true);
    setAttribute(ATTR_EDITABLE, true);
    setAttribute(ATTR_CAN_SAME, false);
}

QAbstractProperty::~QAbstractProperty()
{
    while(m_children.size() > 0) {
        delete m_children.first();
    }
    if(m_parent != NULL) {
        m_parent->m_children.removeAll(this);
    }
}

void QAbstractProperty::setValue(const QVariant &value)
{
    QVariant old = m_value;
    if(old != value) {
        m_value = value;
        emit value_chaged(old, m_value);
    }
    emit refresh();
}

void QAbstractProperty::setDefault()
{
    m_defaultValue = m_value;
    emit refresh();
}

bool QAbstractProperty::modified()
{
    return m_defaultValue != m_value;
}

void QAbstractProperty::child_value_changed(const QVariant &, const QVariant &)
{

}

void QAbstractProperty::connect_children()
{
    foreach(QAbstractProperty* child, m_children) {
        connect(child, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(child_value_changed(QVariant, QVariant)));
    }
}

void QAbstractProperty::disconnect_children()
{
    foreach(QAbstractProperty* child, m_children) {
        disconnect(child, SIGNAL(value_chaged(QVariant, QVariant)), this, SLOT(child_value_changed(QVariant, QVariant)));
    }
}

void QAbstractProperty::toObject(XMLObject *xml)
{
    if(xml != NULL) {
        xml->clear();
        xml->setTagName(PROPERTY_TITLE);
        writeValue();

        QMapIterator<QString, QVariant> it(m_propertys);

        while(it.hasNext()) {
            it.next();
            xml->setProperty(it.key(), it.value().toString());
        }

        XMLObject *obj;
        foreach(QAbstractProperty* child, m_children) {
            obj = new XMLObject;
            child->toObject(obj);
            xml->inser_child(-1, obj);
        }
    }
}

void QAbstractProperty::fromObject(XMLObject *xml)
{
    if(xml != NULL) {
        if(xml->getTagName() != PROPERTY_TITLE) {
            return;
        }

        QMapIterator<QString, QString> it(xml->getPropertys());
        while(it.hasNext()) {
            it.next();
            m_propertys.insert(it.key(), it.value());
        }

        QList<XMLObject*> children = xml->getChildren();
        foreach(XMLObject* obj, children) {
            QAbstractProperty *pro = getChild(obj->getProperty("name"));
            if(pro != NULL) {
                pro->fromObject(obj);
            }
        }

        makeValue();
        refresh();
    }
}

void QAbstractProperty::makeValue()
{
    if(m_propertys.keys().contains("value")) {
        m_value = m_propertys.value("value");
    }
}

void QAbstractProperty::writeValue()
{
    m_propertys.insert("value", m_value);
}

QVariant QAbstractProperty::getAttribute(const QString &key)
{
    int index = key.indexOf(":");
    if(index > 0) {
        QString c = key.left(index);
        QString k = key.mid(index + 1);
        foreach(QAbstractProperty* p, m_children) {
            if(p->getObjectProperty("name").toString() == c) {
                return p->getAttribute(k);
            }
        }
    }

    return m_attributes.value(key);
}

QVariant QAbstractProperty::getObjectProperty(const QString &key)
{
    return m_propertys.value(key);
}

void QAbstractProperty::setAttribute(const QString &key, const QVariant &value)
{
    int index = key.indexOf(":");
    if(index > 0) {
        QString c = key.left(index);
        QString k = key.mid(index + 1);
        foreach(QAbstractProperty* p, m_children) {
            if(p->getObjectProperty("name").toString() == c) {
                p->setAttribute(k, value);
                break;
            }
        }
    } else {
        m_attributes.insert(key, value);
    }
}

void QAbstractProperty::setObjectProperty(const QString &key, const QVariant &value)
{
    if(value.isValid()) {
        m_propertys.insert(key, value);
    } else {
        m_propertys.remove(key);
    }
}

QAbstractProperty & QAbstractProperty::operator =(const QAbstractProperty &pro)
{
    this->m_propertys = pro.m_propertys;
    setValue(pro.m_value);
    return *this;
}

QVariant QAbstractProperty::getValue()
{
    return m_value;
}

void QAbstractProperty::reset()
{
    notifyEditValue(m_defaultValue);
}

QAbstractProperty* QAbstractProperty::getChild(const QString &name)
{
    foreach(QAbstractProperty* pro, m_children) {
        if(pro->getObjectProperty("name").toString() == name) {
            return pro;
        }
    }
    return NULL;
}

QList<QAbstractProperty*> QAbstractProperty::getChildren()
{
    return m_children;
}

void QAbstractProperty::notifyEditValue(const QVariant &value)
{
    emit edit_value(value);
}

QAbstractProperty* QAbstractProperty::getParent()
{
    return m_parent;
}

void QAbstractProperty::setHost(QAbstractHost *host)
{
    m_host = host;
}

QAbstractHost *QAbstractProperty::getHost()
{
    return m_host;
}


