#include "qboolproperty.h"
#include <QStyleOptionButton>
#include <QPixmap>
#include <QApplication>
#include <QPainter>


QBoolProperty::QBoolProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Bool");
}

QString QBoolProperty::getValueText()
{
    return getValue().toString();
}

QIcon QBoolProperty::getValueIcon()
{
    QStyleOptionButton opt;
    opt.state |= getValue().toBool() ? QStyle::State_On : QStyle::State_Off;

    opt.state |= QStyle::State_Enabled;

    const QStyle* style = QApplication::style();
    const int indicatiorWidth = style->pixelMetric(QStyle::PM_IndicatorWidth, &opt);
    const int indicatiorHeight = style->pixelMetric(QStyle::PM_IndicatorHeight, &opt);
    const int listViewIconSize = indicatiorWidth;
    const int pixmapWidth = indicatiorWidth;
    const int pixmapHeight = qMax(indicatiorHeight, listViewIconSize);

    opt.rect = QRect(0, 0, indicatiorWidth, indicatiorHeight);
    QPixmap pixmap = QPixmap(pixmapWidth, pixmapHeight);
    pixmap.fill(Qt::transparent);
    {
        const int xoff = (pixmapWidth > indicatiorWidth) ? (pixmapWidth - indicatiorWidth) / 2 : 0;
        const int yoff = (pixmapHeight > indicatiorHeight) ? (pixmapHeight - indicatiorHeight) / 2 : 0;
        QPainter p(&pixmap);
        p.translate(xoff, yoff);
        style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &opt, &p);
    }
    return QIcon(pixmap);
}

