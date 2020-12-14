#include "qabstracthost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../xmlobject.h"
#include "../qhostfactory.h"
#include "../qlanguage.h"
#include "../qlanguagemanager.h"
#include "../qcommonstruct.h"
#include "../qdatamanager.h"
#include "../property/stylesheetitem/qabstractstylesheetitem.h"
#include "../property/stylesheetitem/qstylesheetitemfactory.h"
#include "../property/stylesheetitem/stylesheetstruct.h"

#include <QMouseEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QUuid>
#include <QMetaMethod>
#include <QScriptEngine>

QAbstractHost::QAbstractHost(QAbstractHost *parent) :
    QObject(parent),
    m_parent(parent),
    m_object(NULL),
    m_timer(new QTimer(this)),
    m_engine(new QScriptEngine)
{
    setProperty("title",OBJECT_TITLE);
    set_attribute("uuid",QUuid::createUuid().toString());

    setProperty("function_list",QStringList()<<"destroyed(QObject*)"<<"destroyed()"<<"deleteLater()"
                <<"exec(QString,QMap<QString,QString>)"<<"translateChanged(QString)"
                <<"languageChanged()");

    if(m_parent!=NULL)
    {
        m_parent->m_children.append(this);
    }
    connect(m_timer,SIGNAL(timeout()),this,SLOT(property_refresh()));
}

QAbstractHost::~QAbstractHost()
{
    clear();
    if(m_parent!=NULL)
    {
        m_parent->m_children.removeAll(this);
    }
    if(m_object!=NULL)
    {
        m_object->deleteLater();
    }
    delete m_timer;
    delete m_engine;
}

void QAbstractHost::insertChildren(const QList<int> &indexs, const QList<QAbstractHost *> &children)
{
    QList<int> ind;
    QList<QAbstractHost*> list;

    for(int i=0;i<children.size();i++)
    {
        QAbstractHost *h=children.at(i);
        if(h->m_parent!=NULL)
        {
            continue;
        }
        int index=indexs.at(i);
        if(index<0 || index>m_children.size())
        {
            index=m_children.size();
        }
        h->m_parent=this;
        if(h->get_object()->isWidgetType())
        {
            QWidget* w=(QWidget*)h->get_object();
            w->setParent((QWidget*)m_object);
            w->setVisible(true);
        }
        else
        {
            h->get_object()->setParent(m_object);
        }
        m_children.insert(index,h);
        h->set_page_manager(m_page_manager);
        h->set_language_manager(m_language_manager);
        h->set_data_manager(m_data_manager);
        h->set_resource_manager(m_resource_manager);
        ind.append(index);
        list.append(h);
    }
    emit insertChildren_signal(list,ind);
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
    emit removeChildren_signal(list);

    foreach(QAbstractHost* h,list)
    {
        h->m_parent=NULL;
        if(h->m_object->isWidgetType())
        {
            QWidget* w=(QWidget*)h->m_object;
            w->setParent(NULL);
            w->setVisible(false);
        }
        else
        {
            h->m_object->setParent(NULL);
        }
        m_children.removeAll(h);
    }
}

QList<QAbstractHost*>   QAbstractHost::getChildren()
{
    return m_children;
}

QAbstractHost* QAbstractHost::getChild(int index)
{
    if(index<0 || index>=m_children.size())
    {
        return NULL;
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

QAbstractProperty* QAbstractHost::get_property(const QString &name)
{
    QStringList list=name.split(".");
    if(list.size()>=0)
    {
        QAbstractProperty* pro=m_nameToProperty.value(list.takeFirst());
        while(list.size()>0)
        {
            pro=pro->getChild(list.takeFirst());
            if(pro==NULL)
            {
                break;
            }
        }
        return pro;
    }
    return NULL;
}

QList<QAbstractProperty*> QAbstractHost::get_propertys()
{
    return m_propertys;
}

QVariant QAbstractHost::get_property_value(const QString &name)
{
    QAbstractProperty *pro=get_property(name);
    if(pro!=NULL)
    {
        return pro->get_value();
    }
    else
    {
        return QVariant();
    }
}

void QAbstractHost::set_property_value(const QString &name, const QVariant &value)
{
    QAbstractProperty *pro=get_property(name);
    if(pro!=NULL)
    {
        pro->set_value(value);
        QAbstractProperty *p=pro;
        while(p->getParent()!=NULL)
        {
            p=p->getParent();
        }
        m_object->setProperty(p->get_property("name").toByteArray(),p->get_value());
        if(pro->get_attribute("group")=="Style Sheet")
        {
            make_stylesheet();
        }
    }
}

void QAbstractHost::insert_property(QAbstractProperty *property, int index)
{
    if(m_propertys.contains(property))
    {
        return;
    }

    if(index<0 || index>=m_propertys.size())
    {
        index=m_propertys.size();
    }
    property->set_host(this);
    m_propertys.insert(index,property);
    m_nameToProperty.insert(property->get_property("name").toString(),property);
}

void QAbstractHost::remove_property(const QString &name)
{
    QAbstractProperty *pro=m_nameToProperty.value(name);
    m_nameToProperty.remove(name);
    if(pro!=NULL)
    {
        m_propertys.removeAll(pro);
        delete pro;
    }
}

void QAbstractHost::set_default()
{
    foreach(QAbstractProperty* pro,m_propertys)
    {
        pro->set_default();
        if(m_object!=NULL)
        {
            m_object->setProperty(pro->get_property("name").toByteArray(),pro->get_value());
        }
        make_stylesheet();
    }
    foreach(QAbstractHost* h,m_children)
    {
        h->set_default();
        h->make_stylesheet();
    }

    m_timer->start(50);
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

void QAbstractHost::remove_action(const QString &name)
{
    QAction* ac=m_nameToAction.value(name);
    m_nameToAction.remove(name);
    if(ac!=NULL)
    {
        m_actions.removeAll(ac);
        delete ac;
    }
}

QAction* QAbstractHost::getAction(const QString &name)
{
    return m_nameToAction.value(name);
}

QList<QAction*> QAbstractHost::get_actions()
{
    return m_actions;
}

QObject* QAbstractHost::get_object()
{
    return m_object;
}

void QAbstractHost::set_attribute(const QString &key, const QString &value)
{
    m_attributes.insert(key,value);
}

QString QAbstractHost::get_attribute(const QString &key)
{
    return m_attributes.value(key);
}

void QAbstractHost::init_property()
{
    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->set_property("name","objectName");
        pro->set_attribute("show_name",tr("Name"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        insert_property(pro);
    }
}

void QAbstractHost::to_object(XMLObject *xml)
{
    if(xml!=NULL)
    {
        xml->clear();

        xml->set_title(property("title").toString());


        QMapIterator<QString,QString>       it(m_attributes);
        while(it.hasNext())
        {
            it.next();
            xml->set_property(it.key(),it.value());
        }

        XMLObject* obj;
        foreach(QAbstractProperty* pro,m_propertys)
        {
            if(pro->modified() || pro->get_attribute(ATTR_NEEDSAVE).toBool())
            {
                obj=new XMLObject;
                pro->toObject(obj);
                xml->inser_child(-1,obj);
            }
        }

        foreach(QAbstractHost* h,m_children)
        {
            obj=new XMLObject;
            h->to_object(obj);
            xml->inser_child(-1,obj);
        }
    }
}

void QAbstractHost::from_object(XMLObject *xml)
{
    if(xml!=NULL)
    {
        if(xml->get_title()!=property("title").toString())
        {
            return;
        }

        clear();

        QMapIterator<QString,QString>       it(m_attributes);
        while(it.hasNext())
        {
            it.next();
            m_attributes.insert(it.key(),xml->get_property(it.key()));
        }

        if(get_uuid()=="")
        {
            set_uuid(QUuid::createUuid().toString());
        }

        QList<XMLObject*>   children=xml->getChildren();
        foreach(XMLObject* obj,children)
        {
            if(obj->get_title()==PROPERTY_TITLE)
            {
                QString name=obj->get_property("name");
                QAbstractProperty *pro=m_nameToProperty.value(name);
                if(pro!=NULL)
                {
                    pro->fromObject(obj);
                    pro->set_attribute(ATTR_NEEDSAVE,true);
                }
            }
            else
            {
                QString name=obj->get_property(HOST_TYPE);

                QAbstractHost *h=QHostFactory::create_host(name);
                if(h!=NULL)
                {
                    h->from_object(obj);
                    h->m_parent=this;
                    if(h->get_object()->isWidgetType())
                    {
                        QWidget* w=(QWidget*)h->get_object();
                        w->setParent((QWidget*)m_object);
                        w->setVisible(true);
                    }
                    else
                    {
                        h->get_object()->setParent(m_object);
                    }
                    m_children.append(h);
                }
            }
        }
        make_stylesheet();
    }
}

void QAbstractHost::clear()
{
    while(m_children.size()>0)
    {
        delete m_children.first();
    }
}

QString QAbstractHost::get_show_name()
{
    return "";
}

QString QAbstractHost::get_show_icon()
{
    return "";
}

QString QAbstractHost::get_show_group()
{
    return "";
}

void QAbstractHost::create_object()
{
}

void QAbstractHost::init()
{
    create_object();
    init_property();

    if(m_object!=NULL)
    {
        m_object->installEventFilter(this);
        foreach(QAbstractProperty* pro,m_propertys)
        {
            pro->set_value(m_object->property(pro->get_property("name").toByteArray()));
        }
    }

}

bool QAbstractHost::eventFilter(QObject *o, QEvent *e)
{
    if(o==m_object)
    {
        if(e->type()==QEvent::Paint)
        {
            return handle_paint_event((QPaintEvent*)e);
        }
        else if(e->type()==QEvent::MouseButtonPress)
        {
            return handle_mouse_press_event((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::MouseButtonRelease)
        {
            return handle_mouse_release_event((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::MouseMove)
        {
            return handle_mouse_move_event((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::MouseButtonDblClick)
        {
            return handle_double_clicked_event((QMouseEvent*)e);
        }
        else if(e->type()==QEvent::FocusIn)
        {
            return handle_focus_in_event((QFocusEvent*)e);
        }
        else if(e->type()==QEvent::FocusOut)
        {
            return handle_focus_out_event((QFocusEvent*)e);
        }
        else if(e->type()==QEvent::ContextMenu)
        {
            return handle_context_menu_event((QContextMenuEvent*)e);
        }
    }
    return QObject::eventFilter(o,e);
}

bool QAbstractHost::handle_context_menu_event(QContextMenuEvent *)
{
    return false;
}

bool QAbstractHost::handle_paint_event(QPaintEvent *)
{
    return false;
}

bool QAbstractHost::handle_mouse_press_event(QMouseEvent *e)
{
    QString str=get_property_value("mousePress").toString();
    if(str!="")
    {
        QMap<QString,QString> param;
        param.insert("_x",QString::number(e->x()));
        param.insert("_y",QString::number(e->y()));
        param.insert("_button",QString::number(e->button()));
        exec(str,param);
    }
    return false;
}

bool QAbstractHost::handle_mouse_release_event(QMouseEvent *e)
{
    QString str=get_property_value("mouseRelease").toString();
    if(str!="")
    {
        QMap<QString,QString> param;
        param.insert("_x",QString::number(e->x()));
        param.insert("_y",QString::number(e->y()));
        param.insert("_button",QString::number(e->button()));
        exec(str,param);
        return true;
    }
    return false;
}

bool QAbstractHost::handle_mouse_move_event(QMouseEvent *e)
{
    QString str=get_property_value("mouseMove").toString();
    if(str!="")
    {
        QMap<QString,QString> param;
        param.insert("_x",QString::number(e->x()));
        param.insert("_y",QString::number(e->y()));
        param.insert("_button",QString::number(e->button()));
        exec(str,param);
    }
    return false;
}

bool QAbstractHost::handle_double_clicked_event(QMouseEvent *e)
{
    QString str=get_property_value("doubleClick").toString();
    if(str!="")
    {
        QMap<QString,QString> param;
        param.insert("_x",QString::number(e->x()));
        param.insert("_y",QString::number(e->y()));
        param.insert("_button",QString::number(e->button()));
        exec(str,param);
        return true;
    }
    return false;
}

bool QAbstractHost::handle_focus_in_event(QFocusEvent *)
{
    return false;
}

bool QAbstractHost::handle_focus_out_event(QFocusEvent *)
{
    return false;
}

void QAbstractHost::property_refresh()
{
    if(m_object==NULL)
    {
        return;
    }
    foreach(QAbstractProperty* pro,m_propertys)
    {
        QVariant v=m_object->property(pro->get_property("name").toByteArray());
        if(v!=pro->get_value())
        {
            pro->set_value(v);
        }
    }
}

QString QAbstractHost::get_uuid()
{
    return m_attributes.value("uuid");
}

void QAbstractHost::set_uuid(const QString &uuid)
{
    m_attributes.insert("uuid",uuid);
}

void QAbstractHost::set_language_manager(QLanguageManager *language)
{
    m_language_manager=language;
    connect(language,SIGNAL(current_language_changed()),this,SLOT(current_language_changed()));
    connect(language,SIGNAL(current_text_changed(QString)),this,SLOT(current_text_changed(QString)));
    foreach(QAbstractHost* host,m_children)
    {
        host->set_language_manager(language);
    }
    current_language_changed();
}

QLanguageManager* QAbstractHost::get_language_manager()
{
    return m_language_manager;
}

void QAbstractHost::current_language_changed()
{
    QLanguage* l=m_language_manager->get_current_language();
    foreach(QAbstractProperty* pro,m_propertys)
    {
        if(pro->get_property("tr").toBool())
        {
            tagTranslateInfo *info=NULL;
            if(l!=NULL)info=l->get_translate(pro->get_property("uuid").toString());
            if(info==NULL)
            {
                pro->get_host()->set_property_value(pro->get_property("name").toString(),"");
            }
            else
            {
                pro->get_host()->set_property_value(pro->get_property("name").toString(),info->m_translate);
            }
        }
    }
}

void QAbstractHost::current_text_changed(const QString &uuid)
{
    QLanguage* l=m_language_manager->get_current_language();
    foreach(QAbstractProperty* pro,m_propertys)
    {
        if(pro->get_property("uuid").toString()==uuid)
        {
            tagTranslateInfo *info=NULL;
            if(l!=NULL)info=l->get_translate(uuid);
            if(info==NULL)
            {
                pro->get_host()->set_property_value(pro->get_property("name").toString(),"");
            }
            else
            {
                pro->get_host()->set_property_value(pro->get_property("name").toString(),info->m_translate);
            }
            return;
        }
    }
}

QString QAbstractHost::functionInformation(const QString &name)
{
    int index=metaObject()->indexOfMethod(name.toLocal8Bit());
    if(index==-1)
    {
        return "";
    }
    QString ret;
    QMetaMethod method=this->metaObject()->method(index);

    QList<QByteArray> parameters=method.parameterNames();
    QString para;

    QAbstractProperty *pro;

    for(int i=0;i<parameters.size();i++)
    {
        para=parameters.at(i);
        ret+=(tr("Parameter ")+QString::number(i+1)+" : "+para+"\n");
        pro=m_nameToProperty.value(para,NULL);
        if(pro!=NULL)
        {
            if(pro->get_property("type")=="Enum")
            {
                ComboItems items=pro->get_attribute("items").value<ComboItems>();
                foreach(tagComboItem item,items)
                {
                    ret+=("    "+item.m_value.toString()+" = "+item.m_text+"\n");
                }
            }
            else if(pro->get_property("type")=="Cursor")
            {
                ComboItems items=pro->get_attribute("items").value<ComboItems>();
                foreach(tagComboItem item,items)
                {
                    QCursor c=item.m_value.value<QCursor>();
                    ret+=("    "+QString::number(c.shape())+"="+item.m_text+"\n");
                }
            }
        }

    }
    if(parameters.size()>0)
    {
        ret=ret.left(ret.size()-1);
    }
    return ret;
}

void QAbstractHost::set_page_manager(QPageManager *page)
{
    m_page_manager=page;
    foreach(QAbstractHost* h,m_children)
    {
        h->set_page_manager(page);
    }
}

void QAbstractHost::set_data_manager(QDataManager *data)
{
    m_data_manager=data;
    foreach(QAbstractHost* h,m_children)
    {
        h->set_data_manager(data);
    }
}

void QAbstractHost::set_resource_manager(QResourceManager *resource)
{
    m_resource_manager=resource;
    foreach(QAbstractHost* h,m_children)
    {
        h->set_resource_manager(resource);
    }
}

QScriptEngine* QAbstractHost::get_script_engine()
{
    return m_engine;
}

void QAbstractHost::exec(const QString &code, const QMap<QString, QString> &param)
{
    QScriptValue global=m_engine->globalObject();
    QMapIterator<QString, QString> it(param);


    while(it.hasNext())
    {
        it.next();
        global.setProperty(it.key(),it.value());
    }

    m_engine->setGlobalObject(global);
    m_engine->evaluate(code);
    if(m_engine->hasUncaughtException())
    {
        qDebug(m_engine->uncaughtException().toString().toLocal8Bit());
    }
}

void QAbstractHost::make_stylesheet()
{
    QString str;
    QString temp;
    QString head;
    foreach(QAbstractProperty* pro,m_propertys)
    {
        if(pro->inherits("QStylesheetProperty"))
        {
            QAbstractStylesheetItem *item=QStylesheetItemFactory::createItem(pro->get_property("name").toString());
            if(item!=NULL)
            {
                tagStylesheetItems list=pro->get_value().value<tagStylesheetItems>();
                foreach(tagStylesheetItem it,list)
                {
                    QVariant v;
                    v.setValue<tagStylesheetItem>(it);
                    item->setValue(v);
                    temp=item->makeStylesheet();
                    if(temp!="")
                    {
                        if(!it.m_attributes.value("no-head").toBool())
                        {
                            head="#"+m_object->property("objectName").toString();
                        }
                        if(it.m_attributes.value("title").toString()!="Normal")
                        {
                            head+=(it.m_attributes.value("title").toString());
                        }
                        temp=head+temp;
                    }
                    if(temp!="")
                    {
                        str+=temp;
                    }
                }
            }
        }
    }
    m_object->setProperty("styleSheet",str);
}

void QAbstractHost::setParent(QAbstractHost *parent, int index)
{
    if(m_parent==parent)
    {
        return;
    }
    if(m_parent!=NULL)
    {
        m_parent->m_children.removeAll(this);
    }

    m_parent=parent;
    if(m_parent!=NULL)
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
            wid->setParent(NULL);
        }
        else
        {
            m_object->setParent(NULL);
        }
    }
    emit parent_changed();
}

QString QAbstractHost::get_host_type()
{
    if(m_parent!=NULL)
    {
        return m_parent->get_host_type();
    }
    else
    {
        return property("title").toString();
    }
}
