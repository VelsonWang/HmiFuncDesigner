#include "qpushbuttonhost.h"
#include "../widgets/qpushbuttonex.h"
#include "../xmlobject.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>

QPushButtonHost::QPushButtonHost(QAbstractHost *parent):
    QAbstractButtonHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QPushButtonHost::getShowName()
{
    return tr("Push Button");
}

QString QPushButtonHost::getShowGroup()
{
    return tr("Buttons");
}

QString QPushButtonHost::getShowIcon()
{
    return ":/images/pushbutton.png";
}

void QPushButtonHost::createObject()
{
    m_object = new QPushButtonEx;
    m_object->setObjectName("pushbutton");
}

void QPushButtonHost::initProperty()
{
    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "funcs");
        pObj->setAttribute("show_name", tr("功能操作"));
        pObj->setAttribute("group", "HMI");
        QStringList listEvents;
        getSupportEvents(listEvents);
        pObj->setAttribute("supportevents", listEvents.join("|"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Script");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "script");
        pObj->setAttribute("show_name", tr("执行脚本"));
        pObj->setAttribute("group", "HMI");
        QStringList listEvents;
        getSupportEvents(listEvents);
        pObj->setAttribute("supportevents", listEvents.join("|"));
        insertProperty(pObj);
    }

    //QAbstractButtonHost::initProperty();
    QWidgetHost::initProperty();
    /*
        pObj = QPropertyFactory::create_property("Script");
        if(pObj != NULL) {
            pObj->setObjectProperty("name", "clicked_down");
            pObj->setAttribute("show_name", tr("按下"));
            pObj->setAttribute("group", "Events");
            insertProperty(pObj);
        }

        pObj = QPropertyFactory::create_property("Script");
        if(pObj != NULL) {
            pObj->setObjectProperty("name", "clicked_up");
            pObj->setAttribute("show_name", tr("抬起"));
            pObj->setAttribute("group", "Events");
            insertProperty(pObj);
        }
    */
    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "showContent");
        pObj->setAttribute("show_name", tr("显示内容"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        QStringList contents;
        contents << tr("文本") << tr("图片");
        foreach(QString szEv, contents) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(tr("文本"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("String");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "text");
        pObj->setAttribute("show_name", tr("文本")); // tr("Text")
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "hAlign");
        pObj->setAttribute("show_name", tr("水平对齐"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        QStringList alignList;
        alignList << tr("左对齐") << tr("居中对齐") << tr("右对齐");
        foreach(QString szEv, alignList) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(tr("居中对齐"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "vAlign");
        pObj->setAttribute("show_name", tr("垂直对齐"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        QStringList alignList;
        alignList << tr("上对齐") << tr("居中对齐") << tr("下对齐");
        foreach(QString szEv, alignList) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(tr("居中对齐"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Image");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "picture");
        pObj->setAttribute("show_name", tr("选择图片"));
        pObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "textColor");
        pObj->setAttribute("show_name", tr("文本颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "backgroundColor");
        pObj->setAttribute("show_name", tr("背景颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "transparent");
        pObj->setAttribute("show_name", tr("透明"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Font");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "font");
        pObj->setAttribute("show_name", tr("字体"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Number");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "borderWidth");
        pObj->setAttribute("show_name", tr("边框宽度"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "borderColor");
        pObj->setAttribute("show_name", tr("边框颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "enableOnInitial");
        pObj->setAttribute("show_name", tr("初始有效"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "showOnInitial");
        pObj->setAttribute("show_name", tr("初始可见"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 100, 30));
    setPropertyValue("text", "button");
}

void QPushButtonHost::getSupportEvents(QStringList &listValue)
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
        if(szEventGroupName == "PushButton") {

            QList<XMLObject*> childrenGroup = eventGroup->getChildren();
            if(childrenGroup.size() < 1) {
                continue;
            }

            foreach(XMLObject* event, childrenGroup) {
                QString eventName = event->getProperty("name");
                QString eventShowName = event->getProperty("ShowName");
                listValue << QString("%1-%2").arg(eventName).arg(eventShowName);

                QList<XMLObject*> funcDesc = event->getChildren();
                if(funcDesc.size() < 1) {
                    continue;
                }
                QString strDesc = event->getCurrentChild("desc")->getText();
            }
        }
    }
}
