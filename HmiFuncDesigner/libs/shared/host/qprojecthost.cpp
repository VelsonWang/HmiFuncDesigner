#include "qprojecthost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qlanguagemanager.h"
#include "../qlanguage.h"
#include "../qpagemanager.h"


QProjectHost::QProjectHost(QAbstractHost *parent):
    QAbstractHost(parent)
{
    setProperty("title",PROJECT_HOST_TITLE);
}

void QProjectHost::create_object()
{
    m_object=new QObject;
}

void QProjectHost::init_property()
{
    QAbstractHost::init_property();
    QAbstractProperty* pro;

    pro=m_nameToProperty.value("objectName");

    pro->set_attribute(ATTR_EDITABLE,false);

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->set_property("name","projectPath");
        pro->set_attribute("show_name",tr("Project Path"));
        pro->set_attribute(ATTR_EDITABLE,false);
        pro->set_attribute("group","Attributes");
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","start_user");
        pro->set_attribute("show_name",tr("User"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","start_language");
        pro->set_attribute("show_name",tr("Language"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","start_page");
        pro->set_attribute("show_name",tr("Page"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_NEEDSAVE,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Size");
    if(pro!=NULL)
    {
        pro->set_property("name","designer_size");
        pro->set_attribute("show_name",tr("Design Size"));
        pro->set_attribute("group","Attributes");
        m_object->setProperty("designer_size",QSize(800,600));
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Size");
    if(pro!=NULL)
    {
        pro->set_property("name","running_size");
        pro->set_attribute("show_name",tr("Running Size"));
        pro->set_attribute("group","Attributes");
        m_object->setProperty("running_size",QSize(800,600));
        insert_property(pro);
    }
}

void QProjectHost::set_language(const QString &language)
{
    QList<QLanguage*> languages=m_language_manager->get_all_languages();

    foreach(QLanguage* l,languages)
    {
        if(l->get_language_name()==language)
        {
            m_language_manager->set_current_language(l->get_uuid());
            return;
        }
    }
    m_language_manager->set_current_language("");
}

QString QProjectHost::current_language()
{
    QLanguage *l=m_language_manager->get_current_language();
    if(l==NULL)
    {
        return "";
    }
    else
    {
        return l->get_language_name();
    }
}

void QProjectHost::show_form(const QString &name)
{
    QList<QAbstractHost*> pages=m_page_manager->getPages_by_title("form");
    foreach(QAbstractHost* p,pages)
    {
        qDebug(p->get_property_value("objectName").toByteArray());
        if(p->get_property_value("objectName").toString()==name)
        {
            show_form_by_uuid(p->get_uuid());
            return;
        }
    }
}

void QProjectHost::show_dialog(const QString &name)
{

}

void QProjectHost::show_form_by_uuid(const QString &uuid)
{
    QAbstractHost* host=m_page_manager->get_page(uuid);

    if(host!=NULL)
    {
        QWidget* wid=(QWidget*)host->get_object();
        emit show_widget(wid);
    }
}
