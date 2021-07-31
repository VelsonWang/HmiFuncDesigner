#include "qvaluestickhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qvaluestick.h"

QValueStickHost::QValueStickHost(QAbstractHost *parent) : QWidgetHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
    setProperty("scaledContents", true);
}

QString QValueStickHost::getShowName()
{
    return tr("ValueStick");
}

QString QValueStickHost::getShowGroup()
{
    return tr("Display Widgets");
}

QString QValueStickHost::getShowIcon()
{
    return ":/images/valuestick.png";
}

void QValueStickHost::createObject()
{
    m_object = new QValueStick();
    m_object->setObjectName("valuestick");
}

void QValueStickHost::initProperty()
{
    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "tag");
        pObj->setAttribute("show_name", tr("选择变量"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    QWidgetHost::initProperty();

    pObj=QPropertyFactory::create_property("Float");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "maxValue");
        pObj->setAttribute("show_name", tr("最大值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Float");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "minValue");
        pObj->setAttribute("show_name", tr("最小值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Number");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "scaleNum");
        pObj->setAttribute("show_name", tr("刻度个数"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "textColor");
        pObj->setAttribute("show_name", tr("文本颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "backgroundColor");
        pObj->setAttribute("show_name", tr("背景颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "foregroundColor");
        pObj->setAttribute("show_name", tr("前景颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "scaleColor");
        pObj->setAttribute("show_name", tr("标尺颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name","scaleDir");
        pObj->setAttribute("show_name", tr("标尺方向"));
        pObj->setAttribute("group","Attributes");
        pObj->setAttribute(ATTR_CAN_SAME,true);
        ComboItems items;
        QStringList scaleDirList;
        scaleDirList << tr("从左到右") << tr("从右到左") << tr("从上到下") << tr("从下到上");
        foreach(QString szEv, scaleDirList) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->set_value(tr("从左到右"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name","scalePos");
        pObj->setAttribute("show_name", tr("标尺位置"));
        pObj->setAttribute("group","Attributes");
        pObj->setAttribute(ATTR_CAN_SAME,true);
        ComboItems items;
        QStringList scalePosList;
        scalePosList << tr("右下方") << tr("左上方");
        foreach(QString szEv, scalePosList) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->set_value(tr("左上方"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Font");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "font");
        pObj->setAttribute("show_name", tr("字体"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Bool");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "showRuler");
        pObj->setAttribute("show_name", tr("显示标尺"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Bool");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "showScale");
        pObj->setAttribute("show_name", tr("显示刻度"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Bool");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "showOnInitial");
        pObj->setAttribute("show_name", tr("初始可见"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 120, 45));
}
