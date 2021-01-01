#include "qlineedithost.h"


#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"


#include <QLineEdit>

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
    m_object=new QLineEdit();
    m_object->setObjectName("lineedit");
}

void QLineEditHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","text");
        pro->setAttribute("show_name",tr("Text"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Align");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","alignment");
        pro->setAttribute("show_name",tr("Alignment"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }


    pro=QPropertyFactory::create_property("Bool");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","readOnly");
        pro->setAttribute("show_name",tr("ReadOnly"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","frame");
        pro->setAttribute("show_name",tr("Frame"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","textChanged");
        pro->setAttribute("show_name",tr("TextChanged"));
        pro->setAttribute("group","Events");
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=Q_NULLPTR)
    {
        pro->setObjectProperty("name","editFinish");
        pro->setAttribute("show_name",tr("EditFinish"));
        pro->setAttribute("group","Events");
        insertProperty(pro);
    }

    setPropertyValue("geometry",QRect(0,0,80,20));

    QLineEdit* e=(QLineEdit*)m_object;
    connect(e,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(e,SIGNAL(editingFinished()),this,SLOT(editFinish()));
}

void QLineEditHost::setText(const QString text)
{
    setPropertyValue("text",text);
}

QString QLineEditHost::text()
{
    return getPropertyValue("text").toString();
}

void QLineEditHost::setAlignment(int alignment)
{
    setPropertyValue("alignment",alignment);
}

int QLineEditHost::alignment()
{
    return getPropertyValue("alignment").toInt();
}

void QLineEditHost::setReadOnly(bool readOnly)
{
    setPropertyValue("readOnly",readOnly);
}

bool QLineEditHost::readOnly()
{
    return getPropertyValue("readOnly").toBool();
}

void QLineEditHost::setFrame(bool frame)
{
    setPropertyValue("frame",frame);
}

bool QLineEditHost::frame()
{
    return getPropertyValue("frame").toBool();
}

void QLineEditHost::textChanged(const QString &text)
{
    QString code=getPropertyValue("textChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_text",text);
        exec(code,param);
    }
}

void QLineEditHost::editFinish()
{
    QString code=getPropertyValue("editFinish").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        exec(code,param);
    }
}
