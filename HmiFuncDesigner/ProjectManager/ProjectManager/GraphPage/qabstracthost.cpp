#include "qabstracthost.h"
#include <QMouseEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QUuid>
#include <QMetaMethod>

QAbstractHost::QAbstractHost(QAbstractHost *parent) :
    QObject(parent),
    m_parent(parent),
    m_object(Q_NULLPTR)
{
    setProperty("title",OBJECT_TITLE);
    setAttribute("uuid",QUuid::createUuid().toString());

    if(m_parent!=Q_NULLPTR)
    {
        m_parent->m_children.append(this);
    }
}

QAbstractHost::~QAbstractHost()
{
    clear();
    if(m_parent!=Q_NULLPTR)
    {
        m_parent->m_children.removeAll(this);
    }
    if(m_object!=Q_NULLPTR)
    {
        m_object->deleteLater();
    }
}

void QAbstractHost::insertChildren(const QList<int> &indexs, const QList<QAbstractHost *> &children)
{
    QList<int> ind;
    QList<QAbstractHost*> list;

    for(int i=0;i<children.size();i++)
    {
        QAbstractHost *h=children.at(i);
        if(h->m_parent!=Q_NULLPTR)
        {
            continue;
        }
        int index=indexs.at(i);
        if(index<0 || index>m_children.size())
        {
            index=m_children.size();
        }
        h->m_parent=this;
        if(h->getObject()->isWidgetType())
        {
            QWidget* w=(QWidget*)h->getObject();
            w->setParent((QWidget*)m_object);
            w->setVisible(true);
        }
        else
        {
            h->getObject()->setParent(m_object);
        }
        m_children.insert(index,h);
        ind.append(index);
        list.append(h);
    }
    emit insertChildrenSignal(list,ind);
}

void QAbstractHost::removeChildren(const QList<QAbstractHost *> &children)
{
    QList<QAbstractHost*> list;

    foreach(QAbstractHost* h,children)
    {
        if(h->m_parent!=this)
        {
            continue;
        }
        list.append(h);
    }

    if(list.size()==0)
    {
        return;
    }
    emit removeChildrenSignal(list);

    foreach(QAbstractHost* h,list)
    {
        h->m_parent=Q_NULLPTR;
        if(h->m_object->isWidgetType())
        {
            QWidget* w=(QWidget*)h->m_object;
            w->setParent(Q_NULLPTR);
            w->setVisible(false);
        }
        else
        {
            h->m_object->setParent(Q_NULLPTR);
        }
        m_children.removeAll(h);
    }
}

QList<QAbstractHost*> QAbstractHost::getChildren()
{
    return m_children;
}

QAbstractHost* QAbstractHost::getChild(int index)
{
    if(index<0 || index>=m_children.size())
    {
        return Q_NULLPTR;
    }
    else
    {
        return m_children.at(index);
    }
}

QAbstractHost* QAbstractHost::getParent()
{
    return m_parent;
}

int QAbstractHost::getChildCount()
{
    return m_children.size();
}


void QAbstractHost::setDefault()
{
    foreach(QAbstractHost* h,m_children)
    {
        h->setDefault();
    }

}

void QAbstractHost::insertAction(const QString &name, QAction *ac, int index)
{
    if(m_actions.contains(ac))
    {
        return;
    }

    if(index<0 || index>=m_actions.size())
    {
        index=m_actions.size();
    }

    m_actions.insert(index,ac);
    m_nameToAction.insert(name,ac);
}

void QAbstractHost::removeAction(const QString &name)
{
    QAction* ac=m_nameToAction.value(name);
    m_nameToAction.remove(name);
    if(ac!=Q_NULLPTR)
    {
        m_actions.removeAll(ac);
        delete ac;
    }
}

QAction* QAbstractHost::getAction(const QString &name)
{
    return m_nameToAction.value(name);
}

QList<QAction*> QAbstractHost::getActions()
{
    return m_actions;
}

QObject* QAbstractHost::getObject()
{
    return m_object;
}

void QAbstractHost::setAttribute(const QString &key, const QString &value)
{
    m_attributes.insert(key,value);
}

QString QAbstractHost::getAttribute(const QString &key)
{
    return m_attributes.value(key);
}


void QAbstractHost::clear()
{
    while(m_children.size()>0)
    {
        delete m_children.first();
    }
}

QString QAbstractHost::getShowName()
{
    return "";
}

QString QAbstractHost::getShowIcon()
{
    return "";
}

QString QAbstractHost::getShowGroup()
{
    return "";
}

void QAbstractHost::createObject()
{
}

void QAbstractHost::init()
{
    createObject();

    if(m_object!=Q_NULLPTR)
    {
        m_object->installEventFilter(this);
    }

}

bool QAbstractHost::eventFilter(QObject *o, QEvent *e)
{
    if(o==m_object)
    {
        if(e->type()==QEvent::Paint)
        {
            return doPaintEvent((QPaintEvent*)e);
        }
        else if(e->type()==QEvent::MouseButtonPress)
        {
            return doMousePressEvent((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::MouseButtonRelease)
        {
            return doMouseReleaseEvent((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::MouseMove)
        {
            return doMouseMoveEvent((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::MouseButtonDblClick)
        {
            return doDoubleClickedEvent((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::FocusIn)
        {
            return doFocusInEvent((QFocusEvent*)e);
        }
        else if(e->type()==QEvent::FocusOut)
        {
            return doFocusOutEvent((QFocusEvent*)e);
        }
        else if(e->type()==QEvent::ContextMenu)
        {
            return doContextMenuEvent((QContextMenuEvent*)e);
        }
    }
    return QObject::eventFilter(o,e);
}

bool QAbstractHost::doContextMenuEvent(QContextMenuEvent *)
{
    return false;
}

bool QAbstractHost::doPaintEvent(QPaintEvent *)
{
    return false;
}

bool QAbstractHost::doMousePressEvent(QMouseEvent *e)
{
    (void)e;
    return false;
}

bool QAbstractHost::doMouseReleaseEvent(QMouseEvent *)
{
    return false;
}

bool QAbstractHost::doMouseMoveEvent(QMouseEvent *)
{
    return false;
}

bool QAbstractHost::doDoubleClickedEvent(QMouseEvent *)
{
    return false;
}

bool QAbstractHost::doFocusInEvent(QFocusEvent *)
{
    return false;
}

bool QAbstractHost::doFocusOutEvent(QFocusEvent *)
{
    return false;
}


QString QAbstractHost::getUuid()
{
    return m_attributes.value("uuid");
}

void QAbstractHost::setUuid(const QString &uuid)
{
    m_attributes.insert("uuid",uuid);
}


void QAbstractHost::setParent(QAbstractHost *parent, int index)
{
    if(m_parent==parent)
    {
        return;
    }
    if(m_parent!=Q_NULLPTR)
    {
        m_parent->m_children.removeAll(this);
    }

    m_parent=parent;
    if(m_parent!=Q_NULLPTR)
    {
        if(index<0 || index>parent->m_children.size())
        {
            index=m_children.size();
        }
        m_parent->m_children.insert(index,this);
        if(m_object->isWidgetType())
        {
            QWidget* wid=(QWidget*)m_object;
            wid->setParent((QWidget*)parent->m_object);
            wid->setVisible(true);
        }
        else
        {
            m_object->setParent(parent->m_object);
        }
    }
    else
    {
        if(m_object->isWidgetType())
        {
            QWidget* wid=(QWidget*)m_object;
            wid->setParent(Q_NULLPTR);
        }
        else
        {
            m_object->setParent(Q_NULLPTR);
        }
    }
    emit parentChanged();
}

QString QAbstractHost::getHostType()
{
    if(m_parent!=Q_NULLPTR)
    {
        return m_parent->getHostType();
    }
    else
    {
        return property("title").toString();
    }
}
