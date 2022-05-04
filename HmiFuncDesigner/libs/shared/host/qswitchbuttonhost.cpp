#include "qswitchbuttonhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qswitchbutton.h"
#include "../xmlobject.h"
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>

QSwitchButtonHost::QSwitchButtonHost(QAbstractHost *parent) : QWidgetHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
    setProperty("scaledContents", true);
}

QString QSwitchButtonHost::getShowName()
{
    return tr("SwitchButton");
}

QString QSwitchButtonHost::getShowGroup()
{
    return tr("Buttons");
}

QString QSwitchButtonHost::getShowIcon()
{
    return ":/images/switchbutton.png";
}

void QSwitchButtonHost::createObject()
{
    m_object = new QSwitchButton();
    m_object->setObjectName("SwitchButton");
}

void QSwitchButtonHost::initProperty()
{
    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "tag");
        pObj->setAttribute("show_name", tr("选择变量"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "funcs");
        pObj->setAttribute("show_name", tr("功能操作"));
        pObj->setAttribute("group", "HMI");
        insertProperty(pObj);
    }

    QWidgetHost::initProperty();

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "stateOnInitial");
        pObj->setAttribute("show_name", tr("初始状态"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

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

    pObj = QPropertyFactory::create_property("Image");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "resetPictureFile");
        pObj->setAttribute("show_name", tr("选择关时图片"));
        pObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Image");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "setPictureFile");
        pObj->setAttribute("show_name", tr("选择开时图片"));
        pObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "showNoScale");
        pObj->setAttribute("show_name", tr("原尺寸显示"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("String");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "resetText");
        pObj->setAttribute("show_name", tr("关时文本")); // tr("Text")
        pObj->setAttribute("group", "Attributes");
        pObj->setValue(QString(tr("关")));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("String");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "setText");
        pObj->setAttribute("show_name", tr("开时文本")); // tr("Text")
        pObj->setAttribute("group", "Attributes");
        pObj->setValue(QString(tr("开")));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Font");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "font");
        pObj->setAttribute("show_name", tr("字体"));
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

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "szHAlign");
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
        pObj->setObjectProperty("name", "szVAlign");
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

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "resetBackgroundColor");
        pObj->setAttribute("show_name", tr("关时背景颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "setBackgroundColor");
        pObj->setAttribute("show_name", tr("开时背景颜色"));
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
        pObj->setObjectProperty("name", "transparent");
        pObj->setAttribute("show_name", tr("透明"));
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

    setPropertyValue("geometry", QRect(0, 0, 120, 60));
}

/**
 * @brief QSwitchButtonHost::supportFuncEvents
 * @details 控件支持的功能事件
 * @return
 */
QStringList QSwitchButtonHost::supportFuncEvents()
{
    QStringList supportFuncEvents;
    supportFuncEvents << QString("%1-%2").arg("OffToOn").arg(tr("切换到开事件"));
    supportFuncEvents << QString("%1-%2").arg("OnToOff").arg(tr("切换到关事件"));
    return supportFuncEvents;
}
