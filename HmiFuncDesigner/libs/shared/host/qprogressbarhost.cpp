#include "qprogressbarhost.h"
#include "../widgets/qprogressbarex.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QProgressBar>

QProgressBarHost::QProgressBarHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QProgressBarHost::getShowName()
{
    return tr("进度条");//tr("Progress Bar")
}

QString QProgressBarHost::getShowGroup()
{
    return tr("显示部件");//tr("Display Widgets")
}

QString QProgressBarHost::getShowIcon()
{
    return ":/images/progress.png";
}

void QProgressBarHost::createObject()
{
    m_object = new QProgressBarEx();
    m_object->setObjectName("progressbar");
}

void QProgressBarHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "tag");
        pObj->setAttribute("show_name", tr("选择变量"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Number");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "maximum");
        pObj->setAttribute("show_name", tr("最大值"));
        pObj->setAttribute(ATTR_CAN_SAME, true);
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Number");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "minimum");
        pObj->setAttribute("show_name", tr("最小值"));
        pObj->setAttribute(ATTR_CAN_SAME, true);
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "alignment");
        pObj->setAttribute("show_name", tr("文本对齐"));
        pObj->setAttribute(ATTR_CAN_SAME, true);
        pObj->setAttribute("group", "Attributes");
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("靠左");
        item.m_value = Qt::AlignLeft;
        items.append(item);

        item.m_text = tr("居中");
        item.m_value = Qt::AlignHCenter;
        items.append(item);

        item.m_text = tr("靠右");
        item.m_value = Qt::AlignRight;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(Qt::AlignLeft);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "invertedAppearance");
        pObj->setAttribute("show_name", tr("反向显示")); //InvertedAppearance
        pObj->setAttribute(ATTR_CAN_SAME, true);
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "textVisible");
        pObj->setAttribute("show_name", tr("文本可见")); //TextVisible
        pObj->setAttribute(ATTR_CAN_SAME, true);
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 100, 20));
}

void QProgressBarHost::setValue(int value)
{
    setPropertyValue("value", value);
}

int QProgressBarHost::value()
{
    return getPropertyValue("value").toInt();
}

void QProgressBarHost::setAlignment(int alignment)
{
    setPropertyValue("alignment", alignment);
}

int QProgressBarHost::alignment()
{
    return getPropertyValue("alignment").toInt();
}

void QProgressBarHost::setOrientation(int orientation)
{
    setPropertyValue("orientation", orientation);
}

int QProgressBarHost::orientation()
{
    return getPropertyValue("orientation").toInt();
}

void QProgressBarHost::setFormat(const QString &format)
{
    setPropertyValue("format", format);
}

QString QProgressBarHost::format()
{
    return getPropertyValue("format").toString();
}

void QProgressBarHost::setInvertedAppearance(bool invertedAppearance)
{
    setPropertyValue("invertedAppearance", invertedAppearance);
}

bool QProgressBarHost::invertedAppearance()
{
    return getPropertyValue("invertedAppearance").toBool();
}

void QProgressBarHost::setTextVisible(bool textVisible)
{
    setPropertyValue("textVisible", textVisible);
}

bool QProgressBarHost::textVisible()
{
    return getPropertyValue("textVisible").toBool();
}
