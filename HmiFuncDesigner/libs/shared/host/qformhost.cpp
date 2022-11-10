#include "qformhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../xmlobject.h"
#include "../widgets/qform.h"
#include <QWidget>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>

QFormHost::QFormHost(QAbstractHost *parent):
    QAbstractWidgetHost(parent)
{
    setProperty("title", FORM_TITLE);
}

void QFormHost::createObject()
{
    m_object = new QForm();
}

void QFormHost::initProperty()
{
    QAbstractWidgetHost::initProperty();

    QAbstractProperty* pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "background");
        pObj->setAttribute("show_name", tr("背景颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "openFuncs");
        pObj->setAttribute("show_name", tr("画面打开时执行功能"));
        pObj->setAttribute("group", "HMI");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "closeFuncs");
        pObj->setAttribute("show_name", tr("画面关闭时执行功能"));
        pObj->setAttribute("group", "HMI");
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

