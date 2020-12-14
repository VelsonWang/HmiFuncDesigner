#include "qtextitem.h"

#include "stylesheetstruct.h"
#include "qgradientmaker.h"


QTextItem::QTextItem(QAbstractStylesheetItem *parent):
    QAbstractStylesheetItem(parent)
{
    m_color=new QGradientItem(this);
    m_color->setProperty("title","color");
}

void QTextItem::setValue(const QVariant &value)
{
    if(m_value!=value)
    {
        tagStylesheetItem item=value.value<tagStylesheetItem>();

        disconnect(m_color,SIGNAL(valueChanged()),this,SLOT(subValueChanged()));

        m_color->setValue(item.m_attributes.value("color"));

        m_value=value;
        connect(m_color,SIGNAL(valueChanged()),this,SLOT(subValueChanged()));
    }
}

void QTextItem::subValueChanged()
{
    tagStylesheetItem item;
    if(m_color->isUsed())
    {
        item.m_attributes.insert("color",m_color->value());
    }
    m_value.setValue<tagStylesheetItem>(item);
}

QString QTextItem::makeStylesheet()
{
    QString str;

    str="{\n";

    if(m_color->isUsed())
    {
        QGradient g=m_color->value().value<QGradient>();
        if(g.type()!=QGradient::NoGradient)
        {
            str+=QString("    color:");
            str+=QGradientMaker::styleSheetCode(g);
            str+=";\n";
        }
    }

    str+="}";
    if(str=="{\n}")
    {
        str="";
    }
    return str;
}
