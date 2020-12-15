#include "qprogressbarhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QProgressBar>

QProgressBarHost::QProgressBarHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QProgressBarHost::getShowName()
{
    return tr("Progress Bar");
}

QString QProgressBarHost::getShowGroup()
{
    return tr("Display Widgets");
}

QString QProgressBarHost::getShowIcon()
{
    return ":/images/progress.png";
}

void QProgressBarHost::createObject()
{
    m_object=new QProgressBar();
    m_object->setObjectName("progressbar");
}

void QProgressBarHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->setProperty("name","value");
        pro->setAttribute("show_name",tr("Value"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro,1);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->setProperty("name","maximum");
        pro->setAttribute("show_name",tr("Maximum"));
        pro->setAttribute(ATTR_CAN_SAME,true);
        pro->setAttribute("group","Attributes");
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->setProperty("name","minimum");
        pro->setAttribute("show_name",tr("Minimum"));
        pro->setAttribute(ATTR_CAN_SAME,true);
        pro->setAttribute("group","Attributes");
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->setProperty("name","alignment");
        pro->setAttribute("show_name",tr("Alignment"));
        pro->setAttribute(ATTR_CAN_SAME,true);
        pro->setAttribute("group","Attributes");
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("AlignLeft");
        item.m_value=Qt::AlignLeft;
        items.append(item);

        item.m_text=tr("AlignHCenter");
        item.m_value=Qt::AlignHCenter;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);
        pro->set_value(Qt::AlignLeft);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->setProperty("name","orientation");
        pro->setAttribute("show_name",tr("Orientation"));
        pro->setAttribute(ATTR_CAN_SAME,true);
        pro->setAttribute("group","Attributes");
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("Horizontal");
        item.m_value=Qt::Horizontal;
        items.append(item);

        item.m_text=tr("Vertical");
        item.m_value=Qt::Vertical;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items",v);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->setProperty("name","format");
        pro->setAttribute("show_name",tr("Format"));
        pro->setAttribute(ATTR_CAN_SAME,true);
        pro->setAttribute("group","Attributes");
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->setProperty("name","invertedAppearance");
        pro->setAttribute("show_name",tr("InvertedAppearance"));
        pro->setAttribute(ATTR_CAN_SAME,true);
        pro->setAttribute("group","Attributes");
        insertProperty(pro);
    }


    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->setAttribute("name","textVisible");
        pro->setAttribute("show_name",tr("TextVisible"));
        pro->setAttribute(ATTR_CAN_SAME,true);
        pro->setAttribute("group","Attributes");
        insertProperty(pro);
    }


    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->setAttribute("name","valueChanged");
        pro->setAttribute("show_name",tr("ValueChanged"));
        pro->setAttribute("group","Events");
        m_object->setProperty("valueChanged","");
        insertProperty(pro);
    }
    setPropertyValue("geometry",QRect(0,0,100,20));


    QProgressBar* gre=(QProgressBar*)m_object;
    connect(gre,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void QProgressBarHost::setValue(int value)
{
    setPropertyValue("value",value);
}

int QProgressBarHost::value()
{
    return getPropertyValue("value").toInt();
}

void QProgressBarHost::setAlignment(int alignment)
{
    setPropertyValue("alignment",alignment);
}

int QProgressBarHost::alignment()
{
    return getPropertyValue("alignment").toInt();
}

void QProgressBarHost::setOrientation(int orientation)
{
    setPropertyValue("orientation",orientation);
}

int QProgressBarHost::orientation()
{
    return getPropertyValue("orientation").toInt();
}

void QProgressBarHost::setFormat(const QString &format)
{
    setPropertyValue("format",format);
}

QString QProgressBarHost::format()
{
    return getPropertyValue("format").toString();
}

void QProgressBarHost::setInvertedAppearance(bool invertedAppearance)
{
    setPropertyValue("invertedAppearance",invertedAppearance);
}

bool QProgressBarHost::invertedAppearance()
{
    return getPropertyValue("invertedAppearance").toBool();
}

void QProgressBarHost::setTextVisible(bool textVisible)
{
    setPropertyValue("textVisible",textVisible);
}

bool QProgressBarHost::textVisible()
{
    return getPropertyValue("textVisible").toBool();
}

void QProgressBarHost::valueChanged(int value)
{
    QString code=getPropertyValue("valueChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_value",QString::number(value));
        exec(code,param);
    }
}
