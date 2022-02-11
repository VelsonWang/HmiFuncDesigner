#include "qborderitem.h"

#include "qgradientitem.h"

#include "stylesheetstruct.h"
#include "qgradientmaker.h"

QBorderItem::QBorderItem(QAbstractStylesheetItem *parent):
    QAbstractStylesheetItem(parent)
{
    m_left_color=new QGradientItem(this);
    m_left_color->setProperty("title", "left_color");
    m_left_color->setDefault(0x00000000);

    m_top_color=new QGradientItem(this);
    m_top_color->setProperty("title", "top_color");
    m_top_color->setDefault(0x00000000);

    m_right_color=new QGradientItem(this);
    m_right_color->setProperty("title", "right_color");
    m_right_color->setDefault(0x00000000);

    m_bottom_color=new QGradientItem(this);
    m_bottom_color->setProperty("title", "bottom_color");
    m_bottom_color->setDefault(0x00000000);


    m_left_width=new QAbstractStylesheetItem(this);
    m_left_width->setProperty("title", "left_width");
    m_left_width->setDefault(0);

    m_right_width=new QAbstractStylesheetItem(this);
    m_right_width->setProperty("title", "right_width");
    m_right_width->setDefault(0);

    m_bottom_width=new QAbstractStylesheetItem(this);
    m_bottom_width->setProperty("title", "bottom_width");
    m_bottom_width->setDefault(0);

    m_top_width=new QAbstractStylesheetItem(this);
    m_top_width->setProperty("title", "top_width");
    m_top_width->setDefault(0);

    m_left_style=new QAbstractStylesheetItem(this);
    m_left_style->setProperty("title", "left_style");
    m_left_style->setDefault("none");

    m_right_style=new QAbstractStylesheetItem(this);
    m_right_style->setProperty("title", "right_style");
    m_right_style->setDefault("none");

    m_bottom_style=new QAbstractStylesheetItem(this);
    m_bottom_style->setProperty("title", "bottom_style");
    m_bottom_style->setDefault("none");

    m_top_style=new QAbstractStylesheetItem(this);
    m_top_style->setProperty("title", "top_style");
    m_top_style->setDefault("none");

    m_image=new QAbstractStylesheetItem(this);
    m_image->setProperty("title", "image");

    m_color_same=new QAbstractStylesheetItem(this);
    m_color_same->setProperty("title", "color_same");
    m_color_same->setDefault("true");

    m_style_same=new QAbstractStylesheetItem(this);
    m_style_same->setProperty("title", "style_same");
    m_style_same->setDefault("true");

    m_width_same=new QAbstractStylesheetItem(this);
    m_width_same->setProperty("title", "width_same");
    m_width_same->setDefault("true");
}

void QBorderItem::setValue(const QVariant &value)
{
    if(m_value != value) {
        tagStylesheetItem item = value.value<tagStylesheetItem>();
        foreach(QAbstractStylesheetItem* it, m_children) {
            disconnect(it, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        }

        m_left_color->setValue(item.m_attributes.value("left_color"));
        m_top_color->setValue(item.m_attributes.value("top_color"));
        m_right_color->setValue(item.m_attributes.value("right_color"));
        m_bottom_color->setValue(item.m_attributes.value("bottom_color"));

        m_left_width->setValue(item.m_attributes.value("left_width"));
        m_top_width->setValue(item.m_attributes.value("top_width"));
        m_bottom_width->setValue(item.m_attributes.value("bottom_width"));
        m_right_width->setValue(item.m_attributes.value("right_width"));

        m_left_style->setValue(item.m_attributes.value("left_style"));
        m_top_style->setValue(item.m_attributes.value("top_style"));
        m_bottom_style->setValue(item.m_attributes.value("bottom_style"));
        m_right_style->setValue(item.m_attributes.value("right_style"));

        m_color_same->setValue(item.m_attributes.value("color_same"));
        m_style_same->setValue(item.m_attributes.value("style_same"));
        m_width_same->setValue(item.m_attributes.value("width_same"));

        m_image->setValue(item.m_attributes.value("image"));

        m_value = value;

        foreach(QAbstractStylesheetItem* it, m_children) {
            connect(it, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        }
    }
}

void QBorderItem::subValueChanged()
{
    tagStylesheetItem item;

    if(m_left_color->isUsed()) {
        item.m_attributes.insert("left_color", m_left_color->value());
    }

    if(m_top_color->isUsed()) {
        item.m_attributes.insert("top_color", m_top_color->value());
    }

    if(m_right_color->isUsed()) {
        item.m_attributes.insert("right_color", m_right_color->value());
    }

    if(m_bottom_color->isUsed()) {
        item.m_attributes.insert("bottom_color", m_bottom_color->value());
    }

    if(m_left_width->isUsed()) {
        item.m_attributes.insert("left_width",m_left_width->value());
    }

    if(m_top_width->isUsed()) {
        item.m_attributes.insert("top_width",m_top_width->value());
    }

    if(m_bottom_width->isUsed()) {
        item.m_attributes.insert("bottom_width",m_bottom_width->value());
    }

    if(m_right_width->isUsed()) {
        item.m_attributes.insert("right_width",m_right_width->value());
    }


    if(m_left_style->isUsed()) {
        item.m_attributes.insert("left_style",m_left_style->value());
    }

    if(m_top_style->isUsed()) {
        item.m_attributes.insert("top_style",m_top_style->value());
    }

    if(m_bottom_style->isUsed()) {
        item.m_attributes.insert("bottom_style",m_bottom_style->value());
    }

    if(m_right_style->isUsed()) {
        item.m_attributes.insert("right_style",m_right_style->value());
    }

    if(m_image->isUsed()) {
        item.m_attributes.insert("image",m_image->value());
    }

    if(m_color_same->isUsed()) {
        item.m_attributes.insert("color_same",m_color_same->value());
    }

    if(m_style_same->isUsed()) {
        item.m_attributes.insert("style_same",m_style_same->value());
    }

    if(m_width_same->isUsed()) {
        item.m_attributes.insert("width_same",m_width_same->value());
    }

    m_value.setValue<tagStylesheetItem>(item);
}

QString QBorderItem::makeStylesheet()
{
    QString str;
    str = "{\n";

    QGradient g;
    if(!m_image->isUsed()) {
        if(m_left_width->isUsed() && m_left_color->isUsed() && m_left_style->isUsed()) {
            str += QString("    border-left-width:%1;\n").arg(m_left_width->value().toString());
            g = m_left_color->value().value<QGradient>();
            str += QString("    border-left-color:");
            str += QGradientMaker::styleSheetCode(g);
            str += ";\n";
            str += QString("    border-left-style:%1;\n").arg(m_left_style->value().toString());
        }

        if(m_bottom_width->isUsed() && m_bottom_color->isUsed() && m_bottom_style->isUsed()) {
            str += QString("    border-bottom-width:%1;").arg(m_bottom_width->value().toString());
            g = m_bottom_color->value().value<QGradient>();
            str += QString("    border-bottom-color:");
            str += QGradientMaker::styleSheetCode(g);
            str += ";\n";
            str += QString("    border-bottom-style:%1;\n").arg(m_bottom_style->value().toString());
        }

        if(m_top_width->isUsed() && m_top_color->isUsed() && m_top_style->isUsed()) {
            str += QString("    border-top-width:%1;").arg(m_top_width->value().toString());
            g = m_top_color->value().value<QGradient>();
            str += QString("    border-top-color:");
            str += QGradientMaker::styleSheetCode(g);
            str += ";\n";
            str += QString("    border-top-style:%1;\n").arg(m_top_style->value().toString());
        }

        if(m_right_width->isUsed() && m_right_color->isUsed() && m_right_style->isUsed()) {
            str += QString("    border-right-width:%1;").arg(m_right_width->value().toString());
            g = m_right_color->value().value<QGradient>();
            str += QString("    border-right-color:");
            str += QGradientMaker::styleSheetCode(g);
            str += ";\n";
            str += QString("    border-right-style:%1;\n").arg(m_right_style->value().toString());
        }
    }

    if(m_image->isUsed()) {
        int left,top,right,bottom;
        left = m_left_width->value().toInt();
        top = m_top_width->value().toInt();
        bottom = m_bottom_width->value().toInt();
        right = m_right_width->value().toInt();
        str += QString("    border-image:url(:/%1)").arg(m_image->value().toString());
        if(left != 0 || top != 0 || right != 0 || bottom != 0) {
            str += QString("%1 %2 %3 %4").arg(top).arg(right).arg(bottom).arg(left);
        }
        str += ";\n";
    }

    str += "}";
    if(str == "{\n}") {
        str = "";
    }
    return str;
}
