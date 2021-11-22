#include "qformhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../xmlobject.h"
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
    m_object = new QWidget();
}

void QFormHost::initProperty()
{
    QAbstractWidgetHost::initProperty();

    QAbstractProperty* pObj = QPropertyFactory::create_property("StyleSheet");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "background");
        pObj->setAttribute("show_name", tr("背景颜色")); // tr("Background")
        pObj->setAttribute("group", "Style Sheet");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "function");
        pObj->setAttribute("show_name", tr("功能操作"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute("supportevents", supportFuncEvents().join("|"));
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

/**
 * @brief QFormHost::supportFuncEvents
 * @details 控件支持的功能事件
 * @return
 */
QStringList QFormHost::supportFuncEvents()
{
    QStringList supportFuncEvents;
    supportFuncEvents << QString("%1-%2").arg("OpenGraphPage").arg(tr("打开画面"));
    supportFuncEvents << QString("%1-%2").arg("CloseGraphPage").arg(tr("关闭画面"));
    return supportFuncEvents;
}

