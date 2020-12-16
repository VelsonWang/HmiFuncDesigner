#include "qabstractsliderhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QAbstractSlider>

QAbstractSliderHost::QAbstractSliderHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
}

void QAbstractSliderHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty* pro;

    pro=QPropertyFactory::create_property("Number");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","value");
        pro->setAttribute("show_name",tr("Value"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro,1);
    }
    pro=QPropertyFactory::create_property("Number");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","minimum");
        pro->setAttribute("show_name",tr("Minimum"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","maximum");
        pro->setAttribute("show_name",tr("Maximum"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Number");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","singleStep");
        pro->setAttribute("show_name",tr("SingleStep"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","orientation");
        pro->setAttribute("show_name",tr("Orientation"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
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
        m_object->setProperty("orientation",Qt::Horizontal);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","invertedAppearance");
        pro->setAttribute("show_name",tr("InvertedAppearance"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","valueChanged");
        pro->setAttribute("show_name",tr("ValueChanged"));
        pro->setAttribute("group","Events");
        m_object->setProperty("valueChanged","");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    QAbstractSlider *slider=(QAbstractSlider*)m_object;
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void QAbstractSliderHost::setValue(int value)
{
    setPropertyValue("value",value);
}

int QAbstractSliderHost::value()
{
    return getPropertyValue("value").toInt();
}

void QAbstractSliderHost::setSingleStep(int singleStep)
{
    setPropertyValue("singleStep",singleStep);
}

int QAbstractSliderHost::singleStep()
{
    return getPropertyValue("singleStep").toInt();
}

void QAbstractSliderHost::setOrientation(int orientation)
{
    setPropertyValue("orientation",orientation);
}

int QAbstractSliderHost::orientation()
{
    return getPropertyValue("orientation").toInt();
}

void QAbstractSliderHost::setInvertedAppearance(bool invertedAppearance)
{
    setPropertyValue("invertedAppearance",invertedAppearance);
}

bool QAbstractSliderHost::invertedAppearance()
{
    return getPropertyValue("invertedAppearance").toBool();
}

void QAbstractSliderHost::valueChanged(int value)
{
    QString code=getPropertyValue("valueChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_value",QString::number(value));
        exec(code,param);
    }
}
