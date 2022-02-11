#include "qbackgrounditem.h"
#include "qgradientitem.h"
#include "stylesheetstruct.h"
#include "qgradientmaker.h"

QBackgroundItem::QBackgroundItem(QAbstractStylesheetItem *parent):
    QAbstractStylesheetItem(parent)
{
    m_color = new QGradientItem(this);
    m_color->setProperty("title", "color");
    m_color->setDefault(0x000000);

    m_image = new QAbstractStylesheetItem(this);
    m_image->setProperty("title", "image");

    m_repeat = new QAbstractStylesheetItem(this);
    m_repeat->setProperty("title", "repeat");
    m_repeat->setDefault("repeat-xy");

    m_position = new QAbstractStylesheetItem(this);
    m_position->setProperty("title", "position");
    m_position->setDefault("left top");
}

void QBackgroundItem::setValue(const QVariant &value)
{
    if(m_value != value) {
        tagStylesheetItem item = value.value<tagStylesheetItem>();

        disconnect(m_color, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        disconnect(m_image, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        disconnect(m_repeat, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        disconnect(m_position, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));

        m_color->setValue(item.m_attributes.value("color"));
        m_image->setValue(item.m_attributes.value("image"));
        m_repeat->setValue(item.m_attributes.value("repeat"));
        m_position->setValue(item.m_attributes.value("position"));

        m_value = value;
        connect(m_color, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        connect(m_image, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        connect(m_repeat, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        connect(m_position, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
    }
}

void QBackgroundItem::subValueChanged()
{
    tagStylesheetItem item;
    if(m_color->isUsed()) {
        item.m_attributes.insert("color", m_color->value());
    }
    if(m_image->isUsed()) {
        item.m_attributes.insert("image", m_image->value());
    }

    if(m_repeat->isUsed()) {
        item.m_attributes.insert("repeat", m_repeat->value());
    }

    if(m_position->isUsed()) {
        item.m_attributes.insert("position", m_position->value());
    }

    m_value.setValue<tagStylesheetItem>(item);
}

QString QBackgroundItem::makeStylesheet()
{
    QString str;

    str = "{\n";

    if(m_image->isUsed()) {
        str += QString("    background-image:url(:/%1);\n").arg(m_image->value().toString());
        if(m_repeat->isUsed()) {
            str += QString("    background-repeat:%1;\n").arg(m_repeat->value().toString());
        }
        if(m_position->isUsed()) {
            str += QString("    background-position:%1;\n").arg(m_position->value().toString());
        }
    }

    if(m_color->isUsed()) {
        QGradient g = m_color->value().value<QGradient>();
        if(g.type() != QGradient::NoGradient) {
            str += QString("    background-color:");
            str += QGradientMaker::styleSheetCode(g);
            str += ";\n";
        }
    }

    str += "}";
    if(str == "{\n}") {
        str = "";
    }
    return str;
}
