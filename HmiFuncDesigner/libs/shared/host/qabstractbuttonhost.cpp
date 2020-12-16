#include "qabstractbuttonhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QAbstractButton>

QAbstractButtonHost::QAbstractButtonHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
}

void QAbstractButtonHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("String");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","text");
        pro->setAttribute("show_name",tr("Text"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro,1);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","checkable");
        pro->setAttribute("show_name",tr("Checkable"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","checked");
        pro->setAttribute("show_name",tr("Checked"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","clicked");
        pro->setAttribute("show_name",tr("Clicked"));
        pro->setAttribute("group","Events");
        insertProperty(pro);
    }



    QAbstractButton *button=(QAbstractButton*)m_object;
    connect(button,SIGNAL(clicked()),this,SLOT(clicked_slot()));
}

void QAbstractButtonHost::setText(const QString &text)
{
    setPropertyValue("text",text);
}

QString QAbstractButtonHost::text()
{
    return getPropertyValue("text").toString();
}

void QAbstractButtonHost::setCheckable(bool checkable)
{
    setPropertyValue("checkable",checkable);
}

bool QAbstractButtonHost::checkable()
{
    return getPropertyValue("checkable").toBool();
}

void QAbstractButtonHost::setChecked(bool checked)
{
    setPropertyValue("checked",checked);
}

bool QAbstractButtonHost::checked()
{
    return getPropertyValue("checked").toBool();
}

void QAbstractButtonHost::clicked_slot()
{
    QString code=getPropertyValue("clicked").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        exec(code,param);
    }
}
