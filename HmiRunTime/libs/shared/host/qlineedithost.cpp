#include "qlineedithost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qlineeditex.h"

QLineEditHost::QLineEditHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QLineEditHost::getShowName()
{
    return tr("Line Edit");
}

QString QLineEditHost::getShowGroup()
{
    return tr("Input Widgets");
}

QString QLineEditHost::getShowIcon()
{
    return ":/images/lineedit.png";
}

void QLineEditHost::createObject()
{
    m_object = new QLineEditEx();
    m_object->setObjectName("lineedit");
}

void QLineEditHost::initProperty()
{
    QAbstractProperty *pPropObj;

    pPropObj = QPropertyFactory::create_property("Tag");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "tag");
        pPropObj->setAttribute("show_name", tr("选择变量"));
        pPropObj->setAttribute("group", "HMI");
        pPropObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pPropObj);
    }

    QWidgetHost::initProperty();

    pPropObj = QPropertyFactory::create_property("ByteArray");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "text");
        pPropObj->setAttribute("show_name", tr("文本")); // tr("Text")
        pPropObj->setAttribute("group", "Attributes");
        pPropObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Align");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "alignment");
        pPropObj->setAttribute("show_name", tr("对齐")); // tr("Alignment")
        pPropObj->setAttribute("group", "Attributes");
        pPropObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Bool");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "readOnly");
        pPropObj->setAttribute("show_name", tr("只读")); // tr("ReadOnly")
        pPropObj->setAttribute("group", "Attributes");
        pPropObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Bool");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "frame");
        pPropObj->setAttribute("show_name", tr("边框")); // tr("Frame")
        pPropObj->setAttribute("group", "Attributes");
        pPropObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Script");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "textChanged");
        pPropObj->setAttribute("show_name", tr("文本变化")); // tr("TextChanged")
        pPropObj->setAttribute("group", "Events");
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Script");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "editFinish");
        pPropObj->setAttribute("show_name", tr("编辑完成")); // tr("EditFinish")
        pPropObj->setAttribute("group", "Events");
        insertProperty(pPropObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 80, 20));

    QLineEditEx* e = (QLineEditEx*)m_object;
    connect(e, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(e, SIGNAL(editingFinished()), this, SLOT(editFinish()));
}

void QLineEditHost::setText(const QString text)
{
    setPropertyValue("text", text);
}

QString QLineEditHost::text()
{
    return getPropertyValue("text").toString();
}

void QLineEditHost::setAlignment(int alignment)
{
    setPropertyValue("alignment", alignment);
}

int QLineEditHost::alignment()
{
    return getPropertyValue("alignment").toInt();
}

void QLineEditHost::setReadOnly(bool readOnly)
{
    setPropertyValue("readOnly", readOnly);
}

bool QLineEditHost::readOnly()
{
    return getPropertyValue("readOnly").toBool();
}

void QLineEditHost::setFrame(bool frame)
{
    setPropertyValue("frame", frame);
}

bool QLineEditHost::frame()
{
    return getPropertyValue("frame").toBool();
}

void QLineEditHost::textChanged(const QString &text)
{
    QString code = getPropertyValue("textChanged").toString();
    if(code != "") {
        QMap<QString, QString> param;
        param.insert("_text", text);
        exec(code, param);
    }
}

void QLineEditHost::editFinish()
{
    QString code = getPropertyValue("editFinish").toString();
    if(code != "") {
        QMap<QString, QString> param;
        exec(code, param);
    }
}
