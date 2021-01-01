#include "qcheckboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QCheckBox>

QCheckBoxHost::QCheckBoxHost(QAbstractHost *parent):
    QAbstractButtonHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QCheckBoxHost::getShowName()
{
    return tr("Check Box");
}

QString QCheckBoxHost::getShowGroup()
{
    return tr("Buttons");
}


QString QCheckBoxHost::getShowIcon()
{
    return ":/images/checkbox.png";
}

void QCheckBoxHost::createObject()
{
    m_object=new QCheckBox();
    m_object->setObjectName("checkbox");
}

void QCheckBoxHost::initProperty()
{
    QAbstractButtonHost::initProperty();

    QAbstractProperty *pro;

    SheetItems items=property("sheet_state").value<SheetItems>();

    tagSheetItem item;

    item.m_item_id=SSID_On;
    items.insert(item.m_item_id,item);
    item.m_item_id=SSID_Off;
    items.insert(item.m_item_id,item);

    QVariant v;
    v.setValue<SheetItems>(items);
    setProperty("sheet_state", v);

    QStringList list;
    list<<"::indicator";
    setProperty("sub_control", list);

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","tristate");
        pro->setAttribute("show_name",tr("Tristate"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro,1);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","stateChanged");
        pro->setAttribute("show_name",tr("StateChanged"));
        pro->setAttribute("group","Events");
        insertProperty(pro);
    }

    QCheckBox *b=(QCheckBox*)m_object;
    connect(b,SIGNAL(stateChanged(int)),this,SLOT(stateChanged(int)));

    setPropertyValue("geometry", QRect(0,0,100,20));
    setPropertyValue("text", "checkbox");
    removeProperty("checkable");
}

void QCheckBoxHost::setTristate(bool tristate)
{
    setPropertyValue("tristate", tristate);
}

bool QCheckBoxHost::tristate()
{
    return getPropertyValue("tristate").toBool();
}

void QCheckBoxHost::stateChanged(int state)
{
    QString code=getPropertyValue("stateChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_state",QString::number(state));
        exec(code,param);
    }
}
