#include "qfontproperty.h"
#include "qintproperty.h"
#include "qboolproperty.h"
#include "qenumproperty.h"
#include "../qcommonstruct.h"
#include <QFontDatabase>
#include <QApplication>
#include <QFont>
#include <QImage>
#include <QPainter>

QFontProperty::QFontProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    static QFontDatabase fontDatabase;
    setObjectProperty("type", "Font");

    m_family = new QEnumProperty(this);
    m_family->setAttribute("show_name", tr("字体")); // tr("Family")
    m_family->setObjectProperty("name", "Family");
    QStringList list = fontDatabase.families();
    tagComboItem item;
    ComboItems  items;
    foreach(QString str, list) {
        item.m_text = str;
        item.m_value = str;
        items.append(item);
    }
    QVariant v;
    v.setValue<ComboItems>(items);
    m_family->setAttribute("items", v);

    m_pointSize = new QIntProperty(this);
    m_pointSize->setAttribute("show_name", tr("大小")); // tr("Size")
    m_pointSize->setObjectProperty("name", "Size");

    m_bold = new QBoolProperty(this);
    m_bold->setAttribute("show_name", tr("加粗")); // tr("Bold")
    m_bold->setObjectProperty("name", "Bold");


    m_italic = new QBoolProperty(this);
    m_italic->setAttribute("show_name", tr("斜体")); // tr("Italic")
    m_italic->setObjectProperty("name", "Italic");

    m_underline = new QBoolProperty(this);
    m_underline->setAttribute("show_name", tr("下划线")); // tr("Underline")
    m_underline->setObjectProperty("name", "Underline");

    m_strikeout = new QBoolProperty(this);
    m_strikeout->setAttribute("show_name", tr("删除线")); // tr("Strikeout")
    m_strikeout->setObjectProperty("name", "Strikeout");
}

void QFontProperty::setValue(const QVariant &value)
{
    if(!value.isValid()) {
        return;
    }
    disconnect_children();
    QFont f = value.value<QFont>();
    m_family->setValue(f.family());
    m_bold->setValue(f.bold());
    m_pointSize->setValue(f.pointSize());
    m_italic->setValue(f.italic());
    m_underline->setValue(f.underline());
    m_strikeout->setValue(f.strikeOut());
    QAbstractProperty::setValue(value);
    connect_children();
}

QString QFontProperty::getValueText()
{
    QFont f = getValue().value<QFont>();
    return f.family();
}

QIcon QFontProperty::getValueIcon()
{
    QFont f = getValue().value<QFont>();
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);
    QPainter p(&img);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::Antialiasing, true);
    f.setPointSize(13);
    p.setFont(f);
    QTextOption t;
    t.setAlignment(Qt::AlignCenter);
    p.drawText(QRect(0, 0, 16, 16), QString(QLatin1Char('A')), t);
    return QPixmap::fromImage(img);
}

void QFontProperty::child_value_changed(const QVariant &, const QVariant &)
{
    if(m_pointSize->getValue().toInt() <= 0) {
        return;
    }
    QFont f = getValue().value<QFont>();
    f.setBold(m_bold->getValue().toBool());
    f.setPointSize(m_pointSize->getValue().toInt());
    f.setItalic(m_italic->getValue().toBool());
    f.setUnderline(m_underline->getValue().toBool());
    f.setStrikeOut(m_strikeout->getValue().toBool());
    f.setFamily(m_family->getValue().toString());
    QVariant v;
    v.setValue<QFont>(f);
    QAbstractProperty::setValue(v);
}

void QFontProperty::makeValue()
{
    QFont f = getValue().value<QFont>();
    f.setBold(m_bold->getValue().toBool());
    f.setPointSize(m_pointSize->getValue().toInt());
    f.setItalic(m_italic->getValue().toBool());
    f.setUnderline(m_underline->getValue().toBool());
    f.setStrikeOut(m_strikeout->getValue().toBool());
    f.setFamily(m_family->getValue().toString());
    m_value.setValue<QFont>(f);
}

void QFontProperty::writeValue()
{

}

