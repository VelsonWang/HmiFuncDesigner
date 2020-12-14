#include "qcheckboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QCheckBox>

QCheckBoxHost::QCheckBoxHost(QAbstractHost *parent):
    QAbstractButtonHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QCheckBoxHost::get_show_name()
{
    return tr("Check Box");
}

QString QCheckBoxHost::get_show_group()
{
    return tr("Buttons");
}


QString QCheckBoxHost::get_show_icon()
{
    return ":/images/checkbox.png";
}

void QCheckBoxHost::create_object()
{
    m_object=new QCheckBox();
    m_object->setObjectName("checkbox");
}

void QCheckBoxHost::init_property()
{
    QAbstractButtonHost::init_property();

    QAbstractProperty *pro;

    SheetItems items=property("sheet_state").value<SheetItems>();

    tagSheetItem item;

    item.m_item_id=SSID_On;
    items.insert(item.m_item_id,item);
    item.m_item_id=SSID_Off;
    items.insert(item.m_item_id,item);

    QVariant v;
    v.setValue<SheetItems>(items);
    setProperty("sheet_state",v);

    QStringList list;
    list<<"::indicator";
    setProperty("sub_control",list);

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","tristate");
        pro->set_attribute("show_name",tr("Tristate"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","stateChanged");
        pro->set_attribute("show_name",tr("StateChanged"));
        pro->set_attribute("group","Events");
        insert_property(pro);
    }

    QCheckBox *b=(QCheckBox*)m_object;
    connect(b,SIGNAL(stateChanged(int)),this,SLOT(stateChanged(int)));

    set_property_value("geometry",QRect(0,0,100,20));
    set_property_value("text","checkbox");
    remove_property("checkable");
}

void QCheckBoxHost::setTristate(bool tristate)
{
    set_property_value("tristate",tristate);
}

bool QCheckBoxHost::tristate()
{
    return get_property_value("tristate").toBool();
}

void QCheckBoxHost::stateChanged(int state)
{
    QString code=get_property_value("stateChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_state",QString::number(state));
        exec(code,param);
    }
}
