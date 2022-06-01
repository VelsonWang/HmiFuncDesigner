#include "qlabelhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include <QLabel>

QLabelHost::QLabelHost(QAbstractHost *parent):
    QFrameHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QLabelHost::getShowName()
{
    return tr("标签");//tr("Label")
}

QString QLabelHost::getShowGroup()
{
    return tr("显示部件");//tr("Display Widgets");
}

QString QLabelHost::getShowIcon()
{
    return ":/images/label.png";
}

void QLabelHost::createObject()
{
    m_object = new QLabel();
    m_object->setObjectName("label");
}

void QLabelHost::initProperty()
{
    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "tag");
        pObj->setAttribute("show_name", tr("选择变量"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    QFrameHost::initProperty();

    pObj = QPropertyFactory::create_property("String");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "text");
        pObj->setAttribute("show_name", tr("文本")); // tr("Text")
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj, 2);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "wordWrap");
        pObj->setAttribute("show_name", tr("自动换行")); // tr("WordWrap")
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Alignment");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "alignment");
        pObj->setAttribute("show_name", tr("对齐")); // tr("Alignment")
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("StyleSheet");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "text_sheet");
        pObj->setAttribute("show_name", tr("文本")); // tr("Text")
        pObj->setAttribute("group", "Style Sheet");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 80, 20));
    setPropertyValue("text", "label");
}

void QLabelHost::setText(const QString &text)
{
    setPropertyValue("text", text);
}

QString QLabelHost::text()
{
    return getPropertyValue("text").toString();
}

void QLabelHost::setWordWrap(bool wordWrap)
{
    setPropertyValue("wordWrap", wordWrap);
}

bool QLabelHost::wordWrap()
{
    return getPropertyValue("wordWrap").toBool();
}

void QLabelHost::setAlignment(int alignment)
{
    setPropertyValue("alignment", (Qt::AlignmentFlag)alignment);
}

int QLabelHost::alignment()
{
    return getPropertyValue("alignment").toInt();
}
