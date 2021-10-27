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

        QStringList listEvents;
        getSupportEvents(listEvents);
        pObj->setAttribute("supportevents", listEvents.join("|"));
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



void QFormHost::getSupportEvents(QStringList &szListValue)
{
    QString xmlFileName = QCoreApplication::applicationDirPath() + "/Config/ElementSupportEvents.xml";

    QFile fileCfg(xmlFileName);
    if(!fileCfg.exists()) {
        QMessageBox::critical(nullptr, tr("提示"), tr("事件配置列表文件不存在！"));
        return;
    }
    if(!fileCfg.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if(!xmlFuncSupportList.load(buffer, nullptr)) {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach(XMLObject* eventGroup, childrenFuncSupport) {
        QString szEventGroupName = eventGroup->getProperty("name");
        if(szEventGroupName == "GraphPage") {

            QList<XMLObject*> childrenGroup = eventGroup->getChildren();
            if(childrenGroup.size() < 1) {
                continue;
            }

            foreach(XMLObject* event, childrenGroup) {
                QString eventName = event->getProperty("name");
                QString eventShowName = event->getProperty("ShowName");
                szListValue << QString("%1-%2").arg(eventName).arg(eventShowName);
                QList<XMLObject*> funcDesc = event->getChildren();
                if(funcDesc.size() < 1) {
                    continue;
                }
                QString strDesc = event->getCurrentChild("desc")->getText();
            }
        }
    }
}
