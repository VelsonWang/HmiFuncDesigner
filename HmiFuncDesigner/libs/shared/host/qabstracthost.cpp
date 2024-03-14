#include "qabstracthost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../xmlobject.h"
#include "../qhostfactory.h"
#include "../qcommonstruct.h"
#include "../property/stylesheetitem/qabstractstylesheetitem.h"
#include "../property/stylesheetitem/qstylesheetitemfactory.h"
#include "../property/stylesheetitem/stylesheetstruct.h"
#include <QMouseEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QMetaMethod>
#include <QScriptEngine>
#include <QApplication>
#include <cmath>

quint64 QAbstractHost::m_nextAllocID = 1;
float QAbstractHost::m_zoomRatio = 1.0;

QAbstractHost::QAbstractHost(QAbstractHost *parent) :
    QObject(parent),
    m_parent(parent),
    m_object(NULL),
    m_timer(new QTimer(this)),
    m_engine(new QScriptEngine)
{
    setProperty("title", OBJECT_TITLE);
    setAttribute("id", "-1");

    setProperty("function_list",
                QStringList()
                << "destroyed(QObject*)"
                << "destroyed()"
                << "deleteLater()"
                << "exec(QString,QMap<QString,QString>)"
                << "translateChanged(QString)"
                << "languageChanged()");

    if(m_parent != NULL) {
        m_parent->m_children.append(this);
    }
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onPropertyRefresh()));
}

QAbstractHost::~QAbstractHost()
{
    clear();
    if(m_parent != NULL) {
        m_parent->m_children.removeAll(this);
    }
    if(m_object != NULL) {
        m_object->deleteLater();
    }
    delete m_timer;
    delete m_engine;
}

void QAbstractHost::insertChildren(const QList<int> &indexs, const QList<QAbstractHost *> &children)
{
    QList<int> ind;
    QList<QAbstractHost*> list;

    for(int i = 0; i < children.size(); i++) {
        QAbstractHost *h = children.at(i);
        if(h->m_parent != NULL) {
            continue;
        }
        int index = indexs.at(i);
        if(index < 0 || index > m_children.size()) {
            index = m_children.size();
        }
        h->m_parent = this;
        if(h->getObject()->isWidgetType()) {
            QWidget* w = (QWidget*)h->getObject();
            w->setParent((QWidget*)m_object);
            w->setVisible(true);
        } else {
            h->getObject()->setParent(m_object);
        }
        m_children.insert(index, h);
        h->setPageManager(m_page_manager);
        h->setDataManager(m_data_manager);
        h->setResourceManager(m_resource_manager);
        ind.append(index);
        list.append(h);
    }
    emit notifyInsertChildren(list, ind);
}

void QAbstractHost::removeChildren(const QList<QAbstractHost *> &children)
{
    QList<QAbstractHost*> list;

    foreach(QAbstractHost* h, children) {
        if(h->m_parent != this) {
            continue;
        }
        list.append(h);
    }

    if(list.size() == 0) {
        return;
    }
    emit notifyRemoveChildren(list);

    foreach(QAbstractHost* h, list) {
        h->m_parent = NULL;
        if(h->m_object->isWidgetType()) {
            QWidget* w = (QWidget*)h->m_object;
            w->setParent(NULL);
            w->setVisible(false);
        } else {
            h->m_object->setParent(NULL);
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
    if(index < 0 || index >= m_children.size()) {
        return NULL;
    } else {
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

QAbstractProperty* QAbstractHost::getProperty(const QString &name)
{
    QStringList list = name.split(".");
    if(list.size() >= 0) {
        QAbstractProperty* pro = m_nameToProperty.value(list.takeFirst());
        while(list.size() > 0) {
            pro = pro->getChild(list.takeFirst());
            if(pro == NULL) {
                break;
            }
        }
        return pro;
    }
    return NULL;
}

QList<QAbstractProperty*> QAbstractHost::getPropertys()
{
    return m_propertys;
}

QVariant QAbstractHost::getPropertyValue(const QString &name)
{
    QAbstractProperty *pro = getProperty(name);
    if(pro != NULL) {
        return pro->getValue();
    } else {
        return QVariant();
    }
}

void QAbstractHost::setPropertyValue(const QString &name, const QVariant &value)
{
    QAbstractProperty *pro = getProperty(name);
    if(pro != NULL) {
        pro->setValue(value);
        QAbstractProperty *p = pro;
        while(p->getParent() != NULL) {
            p = p->getParent();
        }

        QString targetPropertyName = p->getObjectProperty("name").toString().toLower();
        //字体和控件大小缩放处理
        if(targetPropertyName == "geometry") {
            QRect rect = p->getValue().toRect();
            int x = floor(rect.x() * QAbstractHost::m_zoomRatio);
            int y = floor(rect.y() * QAbstractHost::m_zoomRatio);
            int width = floor(rect.width() * QAbstractHost::m_zoomRatio);
            int height = floor(rect.height() * QAbstractHost::m_zoomRatio);
            QRect newRect(x, y, width, height);
            m_object->setProperty(p->getObjectProperty("name").toByteArray(), newRect);
        } else if(targetPropertyName == "font") {
            QFont font = qvariant_cast<QFont>(p->getValue());
            if(font.family() != qApp->font().family()) {
                font.family() = qApp->font().family();
            }
            font.setPointSize(floor(font.pointSize() * QAbstractHost::m_zoomRatio));
            m_object->setProperty(p->getObjectProperty("name").toByteArray(), font);
        } else {
            m_object->setProperty(p->getObjectProperty("name").toByteArray(), p->getValue());
        	if(pro->getAttribute("group") == "Style Sheet") {
            	makeStyleSheet();
        	}
        }
    }
}

void QAbstractHost::insertProperty(QAbstractProperty *property, int index)
{
    if(m_propertys.contains(property)) {
        return;
    }

    if(index < 0 || index >= m_propertys.size()) {
        index = m_propertys.size();
    }
    property->setHost(this);
    m_propertys.insert(index, property);
    m_nameToProperty.insert(property->getObjectProperty("name").toString(), property);
}

void QAbstractHost::removeProperty(const QString &name)
{
    QAbstractProperty *pro = m_nameToProperty.value(name);
    m_nameToProperty.remove(name);
    if(pro != NULL) {
        m_propertys.removeAll(pro);
        delete pro;
    }
}

void QAbstractHost::setDefault()
{
    foreach(QAbstractProperty* pro, m_propertys) {
        pro->setDefault();
        if(m_object != NULL) {
            m_object->setProperty(pro->getObjectProperty("name").toByteArray(), pro->getValue());
            if(pro->getObjectProperty("name").toString() == "borderStyle") {
                m_object->setProperty("borderStyle", pro->getValue());
            }
        }
        makeStyleSheet();
    }
    foreach(QAbstractHost* h, m_children) {
        h->setDefault();
        h->makeStyleSheet();
    }

    m_timer->start(50);
}

void QAbstractHost::insertAction(const QString &name, QAction *ac, int index)
{
    if(m_actions.contains(ac)) {
        return;
    }

    if(index < 0 || index >= m_actions.size()) {
        index = m_actions.size();
    }

    m_actions.insert(index, ac);
    m_nameToAction.insert(name, ac);
}

void QAbstractHost::removeAction(const QString &name)
{
    QAction* ac = m_nameToAction.value(name);
    m_nameToAction.remove(name);
    if(ac != NULL) {
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
    m_attributes.insert(key, value);
}

QString QAbstractHost::getAttribute(const QString &key)
{
    return m_attributes.value(key);
}

void QAbstractHost::initProperty()
{
    QAbstractProperty* pro;

    pro = QPropertyFactory::create_property("ByteArray");
    if(pro != NULL) {
        pro->setObjectProperty("name", "objectName");
        pro->setAttribute("show_name", tr("名称")); // tr("Name")
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_NEEDSAVE, true);
        pro->setAttribute(ATTR_EDITABLE, false);
        insertProperty(pro);
    }
}


/**
 * @brief QAbstractHost::allocID
 * @details 分配一个
 * @return ID
 */
quint64 QAbstractHost::allocID()
{
    ++m_nextAllocID;
    return m_nextAllocID;
}


void QAbstractHost::toObject(XMLObject *xml)
{
    if(xml != NULL) {
        xml->clear();

        xml->setTagName(property("title").toString());

        QMapIterator<QString, QString> it(m_attributes);
        while(it.hasNext()) {
            it.next();
            xml->setProperty(it.key(), it.value());
        }

        XMLObject* obj;
        foreach(QAbstractProperty* pro, m_propertys) {
            if(pro->modified() || pro->getAttribute(ATTR_NEEDSAVE).toBool()) {
                obj = new XMLObject;
                pro->toObject(obj);
                xml->inser_child(-1, obj);
            }
        }

        foreach(QAbstractHost* h, m_children) {
            obj = new XMLObject;
            h->toObject(obj);
            xml->inser_child(-1, obj);
        }
    }
}

void QAbstractHost::fromObject(XMLObject *xml)
{
    if(xml != NULL) {
        if(xml->getTagName() != property("title").toString()) {
            return;
        }

        clear();

        QMapIterator<QString, QString> it(m_attributes);
        while(it.hasNext()) {
            it.next();
            m_attributes.insert(it.key(), xml->getProperty(it.key()));
        }

        bool ok;
        int id = getID().toInt(&ok);
        if(ok) {
            if(m_nextAllocID < id) {
                m_nextAllocID = id;
            }
        } else {
            setID(QString::number(this->allocID()));
        }

        QList<XMLObject*> children = xml->getChildren();
        foreach(XMLObject* obj, children) {
            if(obj->getTagName() == PROPERTY_TITLE) {
                QString name = obj->getProperty("name");
                QAbstractProperty *pro = m_nameToProperty.value(name);
                if(pro != NULL) {
                    pro->fromObject(obj);
                    pro->setAttribute(ATTR_NEEDSAVE, true);
                }
            } else {
                QString name = obj->getProperty(HOST_TYPE);

                QAbstractHost *pHostObj = QHostFactory::createHost(name);
                if(pHostObj != NULL) {
                    pHostObj->fromObject(obj);
                    pHostObj->m_parent = this;
                    if(pHostObj->getObject()->isWidgetType()) {
                        QWidget* w = (QWidget*)pHostObj->getObject();
                        w->setParent((QWidget*)m_object);
                        w->setVisible(true);
                    } else {
                        pHostObj->getObject()->setParent(m_object);
                    }
                    m_children.append(pHostObj);
                }
            }
        }
        makeStyleSheet();
    }
}

void QAbstractHost::clear()
{
    while(m_children.size() > 0) {
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
    initProperty();

    if(m_object != NULL) {
        m_object->installEventFilter(this);
        foreach(QAbstractProperty* pro, m_propertys) {
            QVariant v = m_object->property(pro->getObjectProperty("name").toByteArray());
            QString propertyName = pro->getObjectProperty("name").toString().toLower();
            //字体和控件大小缩放处理
            if(propertyName == "geometry") {
                QRect rect = v.toRect();
                int x = ceil(rect.x() / QAbstractHost::m_zoomRatio);
                int y = ceil(rect.y() / QAbstractHost::m_zoomRatio);
                int width = ceil(rect.width() / QAbstractHost::m_zoomRatio);
                int height = ceil(rect.height() / QAbstractHost::m_zoomRatio);
                QRect newRect(x, y, width, height);
                pro->setValue(newRect);
            } else if(propertyName == "font") {
                QFont font = qvariant_cast<QFont>(v);
                if(font.family() != qApp->font().family()) {
                    font.family() = qApp->font().family();
                }
                font.setPointSize(ceil(font.pointSize() / QAbstractHost::m_zoomRatio));
                pro->setValue(font);
            } else {
                pro->setValue(v);
            }
        }
    }
}

bool QAbstractHost::eventFilter(QObject *o, QEvent *e)
{
    if(o == m_object) {
        if(e->type() == QEvent::Paint) {
            return handlePaintEvent((QPaintEvent*)e);
        } else if(e->type() == QEvent::MouseButtonPress) {
            return handleMousePressEvent((QMouseEvent*)e);
        } else if(e->type() == QEvent::MouseButtonRelease) {
            return handleMouseReleaseEvent((QMouseEvent*)e);
        } else if(e->type() == QEvent::MouseMove) {
            return handleMouseMoveEvent((QMouseEvent*)e);
        } else if(e->type() == QEvent::MouseButtonDblClick) {
            return handleDoubleClickedEvent((QMouseEvent*)e);
        } else if(e->type() == QEvent::FocusIn) {
            return handleFocusInEvent((QFocusEvent*)e);
        } else if(e->type() == QEvent::FocusOut) {
            return handleFocusOutEvent((QFocusEvent*)e);
        } else if(e->type() == QEvent::ContextMenu) {
            return handleContextMenuEvent((QContextMenuEvent*)e);
        }
    }
    return QObject::eventFilter(o, e);
}

bool QAbstractHost::handleContextMenuEvent(QContextMenuEvent *)
{
    return false;
}

bool QAbstractHost::handlePaintEvent(QPaintEvent *)
{
    return false;
}

bool QAbstractHost::handleMousePressEvent(QMouseEvent *e)
{
    QString str = getPropertyValue("mousePress").toString();
    if(str != "") {
        QMap<QString, QString> param;
        param.insert("_x", QString::number(e->x()));
        param.insert("_y", QString::number(e->y()));
        param.insert("_button", QString::number(e->button()));
        exec(str, param);
    }
    return false;
}

bool QAbstractHost::handleMouseReleaseEvent(QMouseEvent *e)
{
    QString str = getPropertyValue("mouseRelease").toString();
    if(str != "") {
        QMap<QString, QString> param;
        param.insert("_x", QString::number(e->x()));
        param.insert("_y", QString::number(e->y()));
        param.insert("_button", QString::number(e->button()));
        exec(str, param);
        return true;
    }
    return false;
}

bool QAbstractHost::handleMouseMoveEvent(QMouseEvent *e)
{
    QString str = getPropertyValue("mouseMove").toString();
    if(str != "") {
        QMap<QString, QString> param;
        param.insert("_x", QString::number(e->x()));
        param.insert("_y", QString::number(e->y()));
        param.insert("_button", QString::number(e->button()));
        exec(str, param);
    }
    return false;
}

bool QAbstractHost::handleDoubleClickedEvent(QMouseEvent *e)
{
    QString str = getPropertyValue("doubleClick").toString();
    if(str != "") {
        QMap<QString, QString> param;
        param.insert("_x", QString::number(e->x()));
        param.insert("_y", QString::number(e->y()));
        param.insert("_button", QString::number(e->button()));
        exec(str, param);
        return true;
    }
    return false;
}

bool QAbstractHost::handleFocusInEvent(QFocusEvent *)
{
    return false;
}

bool QAbstractHost::handleFocusOutEvent(QFocusEvent *)
{
    return false;
}

void QAbstractHost::onPropertyRefresh()
{
    if(m_object == NULL) {
        return;
    }
    foreach(QAbstractProperty* pro, m_propertys) {
        QVariant v = m_object->property(pro->getObjectProperty("name").toByteArray());
        if(v != pro->getValue()) {
            QString propertyName = pro->getObjectProperty("name").toString().toLower();
            //字体和控件大小缩放处理
            if(propertyName == "geometry") {
                QRect rect = v.toRect();
                int x = ceil(rect.x() / QAbstractHost::m_zoomRatio);
                int y = ceil(rect.y() / QAbstractHost::m_zoomRatio);
                int width = ceil(rect.width() / QAbstractHost::m_zoomRatio);
                int height = ceil(rect.height() / QAbstractHost::m_zoomRatio);
                QRect newRect(x, y, width, height);
                pro->setValue(newRect);
            } else if(propertyName == "font") {
                QFont font = qvariant_cast<QFont>(v);
                if(font.family() != qApp->font().family()) {
                    font.family() = qApp->font().family();
                }
                font.setPointSize(ceil(font.pointSize() / QAbstractHost::m_zoomRatio));
                pro->setValue(font);
            } else {
                pro->setValue(v);
            }
        }
    }
}

QString QAbstractHost::getID()
{
    return m_attributes.value("id");
}

void QAbstractHost::setID(const QString &id)
{
    m_attributes.insert("id", id);
}


void QAbstractHost::onCurTextChanged(const QString &id)
{
    //    QLanguage* l=m_language_manager->get_current_language();
    //    foreach(QAbstractProperty* pro,m_propertys)
    //    {
    //        if(pro->getProperty("id").toString()==id)
    //        {
    //            tagTranslateInfo *info=NULL;
    //            if(l!=NULL)info=l->get_translate(id);
    //            if(info==NULL)
    //            {
    //                pro->get_host()->setObjectPropertyValue(pro->getProperty("name").toString(),"");
    //            }
    //            else
    //            {
    //                pro->get_host()->setObjectPropertyValue(pro->getProperty("name").toString(),info->m_translate);
    //            }
    //            return;
    //        }
    //    }
}

QString QAbstractHost::functionInformation(const QString &name)
{
    int index = metaObject()->indexOfMethod(name.toLocal8Bit());
    if(index == -1) {
        return "";
    }
    QString ret;
    QMetaMethod method = this->metaObject()->method(index);

    QList<QByteArray> parameters = method.parameterNames();
    QString para;

    QAbstractProperty *pro;

    for(int i = 0; i < parameters.size(); i++) {
        para = parameters.at(i);
        ret += (tr("Parameter ") + QString::number(i + 1) + " : " + para + "\n");
        pro = m_nameToProperty.value(para, NULL);
        if(pro != NULL) {
            if(pro->getObjectProperty("type") == "Enum") {
                ComboItems items = pro->getAttribute("items").value<ComboItems>();
                foreach(tagComboItem item, items) {
                    ret += ("    " + item.m_value.toString() + " = " + item.m_text + "\n");
                }
            } else if(pro->getObjectProperty("type") == "Cursor") {
                ComboItems items = pro->getAttribute("items").value<ComboItems>();
                foreach(tagComboItem item, items) {
                    QCursor c = item.m_value.value<QCursor>();
                    ret += ("    " + QString::number(c.shape()) + "=" + item.m_text + "\n");
                }
            }
        }

    }
    if(parameters.size() > 0) {
        ret = ret.left(ret.size() - 1);
    }
    return ret;
}

void QAbstractHost::setPageManager(QPageManager *page)
{
    m_page_manager = page;
    foreach(QAbstractHost* h, m_children) {
        h->setPageManager(page);
    }
}

void QAbstractHost::setDataManager(QDataManager *data)
{
    m_data_manager = data;
    foreach(QAbstractHost* h, m_children) {
        h->setDataManager(data);
    }
}

void QAbstractHost::setResourceManager(QResourceManager *resource)
{
    m_resource_manager = resource;
    foreach(QAbstractHost* h, m_children) {
        h->setResourceManager(resource);
    }
}

QScriptEngine* QAbstractHost::getScriptEngine()
{
    return m_engine;
}

void QAbstractHost::exec(const QString &code, const QMap<QString, QString> &param)
{
    QScriptValue global = m_engine->globalObject();
    QMapIterator<QString, QString> it(param);


    while(it.hasNext()) {
        it.next();
        global.setProperty(it.key(), it.value());
    }

    m_engine->setGlobalObject(global);
    m_engine->evaluate(code);
    if(m_engine->hasUncaughtException()) {
        qDebug(m_engine->uncaughtException().toString().toLocal8Bit());
    }
}

void QAbstractHost::makeStyleSheet()
{
    QString str;
    QString temp;
    QString head;
    foreach(QAbstractProperty* pro, m_propertys) {
        if(pro->inherits("QStylesheetProperty")) {
            QAbstractStylesheetItem *item = QStylesheetItemFactory::createItem(pro->getObjectProperty("name").toString());
            if(item != NULL) {
                tagStylesheetItems list = pro->getValue().value<tagStylesheetItems>();
                foreach(tagStylesheetItem it, list) {
                    QVariant v;
                    v.setValue<tagStylesheetItem>(it);
                    item->setValue(v);
                    temp = item->makeStylesheet();
                    if(temp != "") {
                        if(!it.m_attributes.value("no-head").toBool()) {
                            head = "#" + m_object->property("objectName").toString();
                        }
                        if(it.m_attributes.value("title").toString() != "Normal") {
                            head += (it.m_attributes.value("title").toString());
                        }
                        temp = head + temp;
                    }
                    if(temp != "") {
                        str += temp;
                    }
                }
            }
        }
    }
    m_object->setProperty("styleSheet", str);
}

void QAbstractHost::setParent(QAbstractHost *parent, int index)
{
    if(m_parent == parent) {
        return;
    }
    if(m_parent != NULL) {
        m_parent->m_children.removeAll(this);
    }

    m_parent = parent;
    if(m_parent != NULL) {
        if(index < 0 || index > parent->m_children.size()) {
            index = m_children.size();
        }
        m_parent->m_children.insert(index, this);
        if(m_object->isWidgetType()) {
            QWidget* wid = (QWidget*)m_object;
            wid->setParent((QWidget*)parent->m_object);
            wid->setVisible(true);
        } else {
            m_object->setParent(parent->m_object);
        }
    } else {
        if(m_object->isWidgetType()) {
            QWidget* wid = (QWidget*)m_object;
            wid->setParent(NULL);
        } else {
            m_object->setParent(NULL);
        }
    }
    emit notifyParentChanged();
}

QString QAbstractHost::getHostType()
{
    if(m_parent != NULL) {
        return m_parent->getHostType();
    } else {
        return property("title").toString();
    }
}
