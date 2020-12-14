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

QString QProgressBarHost::get_show_name()
{
    return tr("Progress Bar");
}

QString QProgressBarHost::get_show_group()
{
    return tr("Display Widgets");
}

QString QProgressBarHost::get_show_icon()
{
    return ":/images/progress.png";
}

void QProgressBarHost::create_object()
{
    m_object=new QProgressBar();
    m_object->setObjectName("progressbar");
}

void QProgressBarHost::init_property()
{
    QWidgetHost::init_property();

    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","value");
        pro->set_attribute("show_name",tr("Value"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","maximum");
        pro->set_attribute("show_name",tr("Maximum"));
        pro->set_attribute(ATTR_CAN_SAME,true);
        pro->set_attribute("group","Attributes");
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=NULL)
    {
        pro->set_property("name","minimum");
        pro->set_attribute("show_name",tr("Minimum"));
        pro->set_attribute(ATTR_CAN_SAME,true);
        pro->set_attribute("group","Attributes");
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","alignment");
        pro->set_attribute("show_name",tr("Alignment"));
        pro->set_attribute(ATTR_CAN_SAME,true);
        pro->set_attribute("group","Attributes");
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
        pro->set_attribute("items",v);
        pro->set_value(Qt::AlignLeft);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","orientation");
        pro->set_attribute("show_name",tr("Orientation"));
        pro->set_attribute(ATTR_CAN_SAME,true);
        pro->set_attribute("group","Attributes");
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
        pro->set_attribute("items",v);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("ByteArray");
    if(pro!=NULL)
    {
        pro->set_property("name","format");
        pro->set_attribute("show_name",tr("Format"));
        pro->set_attribute(ATTR_CAN_SAME,true);
        pro->set_attribute("group","Attributes");
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","invertedAppearance");
        pro->set_attribute("show_name",tr("InvertedAppearance"));
        pro->set_attribute(ATTR_CAN_SAME,true);
        pro->set_attribute("group","Attributes");
        insert_property(pro);
    }


    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_attribute("name","textVisible");
        pro->set_attribute("show_name",tr("TextVisible"));
        pro->set_attribute(ATTR_CAN_SAME,true);
        pro->set_attribute("group","Attributes");
        insert_property(pro);
    }


    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_attribute("name","valueChanged");
        pro->set_attribute("show_name",tr("ValueChanged"));
        pro->set_attribute("group","Events");
        m_object->setProperty("valueChanged","");
        insert_property(pro);
    }
    set_property_value("geometry",QRect(0,0,100,20));


    QProgressBar* gre=(QProgressBar*)m_object;
    connect(gre,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void QProgressBarHost::setValue(int value)
{
    set_property_value("value",value);
}

int QProgressBarHost::value()
{
    return get_property_value("value").toInt();
}

void QProgressBarHost::setAlignment(int alignment)
{
    set_property_value("alignment",alignment);
}

int QProgressBarHost::alignment()
{
    return get_property_value("alignment").toInt();
}

void QProgressBarHost::setOrientation(int orientation)
{
    set_property_value("orientation",orientation);
}

int QProgressBarHost::orientation()
{
    return get_property_value("orientation").toInt();
}

void QProgressBarHost::setFormat(const QString &format)
{
    set_property_value("format",format);
}

QString QProgressBarHost::format()
{
    return get_property_value("format").toString();
}

void QProgressBarHost::setInvertedAppearance(bool invertedAppearance)
{
    set_property_value("invertedAppearance",invertedAppearance);
}

bool QProgressBarHost::invertedAppearance()
{
    return get_property_value("invertedAppearance").toBool();
}

void QProgressBarHost::setTextVisible(bool textVisible)
{
    set_property_value("textVisible",textVisible);
}

bool QProgressBarHost::textVisible()
{
    return get_property_value("textVisible").toBool();
}

void QProgressBarHost::valueChanged(int value)
{
    QString code=get_property_value("valueChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_value",QString::number(value));
        exec(code,param);
    }
}
