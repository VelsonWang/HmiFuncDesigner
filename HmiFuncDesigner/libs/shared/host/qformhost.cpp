#include "qformhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include <QWidget>

QFormHost::QFormHost(QAbstractHost *parent):
    QAbstractWidgetHost(parent)
{
    setProperty("title",FORM_TITLE);
}

void QFormHost::createObject()
{
    m_object = new QWidget();
}

void QFormHost::initProperty()
{
    QAbstractWidgetHost::initProperty();

    QAbstractProperty* pObj = QPropertyFactory::create_property("StyleSheet");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "background");
        pObj->setAttribute("show_name", tr("背景颜色")); // tr("Background")
        pObj->setAttribute("group", "Style Sheet");
        insertProperty(pObj);
    }

    pObj = this->getProperty("geometry");
    pObj->setAttribute("show_name", tr("坐标大小")); // tr("Size")
    pObj->setAttribute(QString("x:") + ATTR_VISIBLE, false);
    pObj->setAttribute(QString("y:") + ATTR_VISIBLE, false);

    pObj = this->getProperty("objectName");
    pObj->setAttribute("editabled", false);

    setPropertyValue("geometry", QRect(0, 0, 800, 600));
}
