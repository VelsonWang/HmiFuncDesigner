#include "qprojecthost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qpagemanager.h"


QProjectHost::QProjectHost(QAbstractHost *parent):
    QAbstractHost(parent)
{
    setProperty("title", PROJECT_HOST_TITLE);
}

void QProjectHost::createObject()
{
    m_object = new QObject;
}

void QProjectHost::initProperty()
{
    QAbstractHost::initProperty();
    QAbstractProperty* pro;

    pro = m_nameToProperty.value("objectName");

    pro->setAttribute(ATTR_EDITABLE, false);

    pro = QPropertyFactory::create_property("ByteArray");
    if(pro != NULL) {
        pro->setObjectProperty("name", "projectPath");
        pro->setAttribute("show_name", tr("Project Path"));
        pro->setAttribute(ATTR_EDITABLE, false);
        pro->setAttribute("group", "Attributes");
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "start_user");
        pro->setAttribute("show_name", tr("User"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_NEEDSAVE, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "start_language");
        pro->setAttribute("show_name", tr("Language"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_NEEDSAVE, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "start_page");
        pro->setAttribute("show_name", tr("Page"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_NEEDSAVE, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Size");
    if(pro != NULL) {
        pro->setObjectProperty("name", "designer_size");
        pro->setAttribute("show_name", tr("Design Size"));
        pro->setAttribute("group", "Attributes");
        m_object->setProperty("designer_size", QSize(800, 600));
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Size");
    if(pro != NULL) {
        pro->setObjectProperty("name", "running_size");
        pro->setAttribute("show_name", tr("Running Size"));
        pro->setAttribute("group", "Attributes");
        m_object->setProperty("running_size", QSize(800, 600));
        insertProperty(pro);
    }
}


void QProjectHost::show_form(const QString &name)
{
    QList<QAbstractHost*> pages = m_page_manager->getPagesByTitle("form");
    foreach(QAbstractHost* p, pages) {
        qDebug(p->getPropertyValue("objectName").toByteArray());
        if(p->getPropertyValue("objectName").toString() == name) {
            show_form_by_id(p->getID());
            return;
        }
    }
}

void QProjectHost::show_dialog(const QString &name)
{

}

void QProjectHost::show_form_by_id(const QString &id)
{
    QAbstractHost* host = m_page_manager->get_page(id);

    if(host != NULL) {
        QWidget* wid = (QWidget*)host->getObject();
        emit notifyShowWidget(wid);
    }
}

