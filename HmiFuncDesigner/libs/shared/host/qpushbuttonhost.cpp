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
    return tr("弹出按钮");//tr("Push Button")
}

QString QPushButtonHost::getShowGroup()
{
    return tr("按钮");//tr("Buttons")
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
        pObj->setObjectProperty("name", "clickedDownFuncs");
        pObj->setAttribute("show_name", tr("按下时执行功能"));
        pObj->setAttribute("group", "HMI");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "clickedUpFuncs");
        pObj->setAttribute("show_name", tr("抬起时执行功能"));
        pObj->setAttribute("group", "HMI");
        insertProperty(pObj);
    }

    /*
    pObj = QPropertyFactory::create_property("Script");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "script");
        pObj->setAttribute("show_name", tr("执行脚本"));
        pObj->setAttribute("group", "HMI");
        insertProperty(pObj);
    }
    */

    QWidgetHost::initProperty();

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "showContent");
        pObj->setAttribute("show_name", tr("显示内容"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);

        ComboItems items;
        tagComboItem item;

        item.m_text = tr("文本");
        item.m_value = QPushButtonEx::Text;
        items.append(item);

        item.m_text = tr("图片");
        item.m_value = QPushButtonEx::Image;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(QPushButtonEx::Text);
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
        tagComboItem item;

        item.m_text = tr("左对齐");
        item.m_value = QPushButtonEx::Left;
        items.append(item);

        item.m_text = tr("居中对齐");
        item.m_value = QPushButtonEx::HCenter;
        items.append(item);

        item.m_text = tr("右对齐");
        item.m_value = QPushButtonEx::Right;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(QPushButtonEx::HCenter);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "vAlign");
        pObj->setAttribute("show_name", tr("垂直对齐"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);

        ComboItems items;
        tagComboItem item;

        item.m_text = tr("上对齐");
        item.m_value = QPushButtonEx::Top;
        items.append(item);

        item.m_text = tr("居中对齐");
        item.m_value = QPushButtonEx::VCenter;
        items.append(item);

        item.m_text = tr("下对齐");
        item.m_value = QPushButtonEx::Bottom;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(QPushButtonEx::VCenter);
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
