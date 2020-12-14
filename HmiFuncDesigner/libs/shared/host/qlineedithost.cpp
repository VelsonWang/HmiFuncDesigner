#include "qlineedithost.h"


#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"


#include <QLineEdit>

QLineEditHost::QLineEditHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QLineEditHost::get_show_name()
{
    return tr("Line Edit");
}

QString QLineEditHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QLineEditHost::get_show_icon()
{
    return ":/images/lineedit.png";
}

void QLineEditHost::create_object()
{
    m_object=new QLineEdit();
    m_object->setObjectName("lineedit");
}

void QLineEditHost::init_property()
{
    QWidgetHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->set_property("name","text");
        pro->set_attribute("show_name",tr("Text"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Align");
    if(pro!=NULL)
    {
        pro->set_property("name","alignment");
        pro->set_attribute("show_name",tr("Alignment"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }


    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","readOnly");
        pro->set_attribute("show_name",tr("ReadOnly"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","frame");
        pro->set_attribute("show_name",tr("Frame"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","textChanged");
        pro->set_attribute("show_name",tr("TextChanged"));
        pro->set_attribute("group","Events");
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","editFinish");
        pro->set_attribute("show_name",tr("EditFinish"));
        pro->set_attribute("group","Events");
        insert_property(pro);
    }

    set_property_value("geometry",QRect(0,0,80,20));

    QLineEdit* e=(QLineEdit*)m_object;
    connect(e,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(e,SIGNAL(editingFinished()),this,SLOT(editFinish()));
}

void QLineEditHost::setText(const QString text)
{
    set_property_value("text",text);
}

QString QLineEditHost::text()
{
    return get_property_value("text").toString();
}

void QLineEditHost::setAlignment(int alignment)
{
    set_property_value("alignment",alignment);
}

int QLineEditHost::alignment()
{
    return get_property_value("alignment").toInt();
}

void QLineEditHost::setReadOnly(bool readOnly)
{
    set_property_value("readOnly",readOnly);
}

bool QLineEditHost::readOnly()
{
    return get_property_value("readOnly").toBool();
}

void QLineEditHost::setFrame(bool frame)
{
    set_property_value("frame",frame);
}

bool QLineEditHost::frame()
{
    return get_property_value("frame").toBool();
}

void QLineEditHost::textChanged(const QString &text)
{
    QString code=get_property_value("textChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_text",text);
        exec(code,param);
    }
}

void QLineEditHost::editFinish()
{
    QString code=get_property_value("editFinish").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        exec(code,param);
    }
}
