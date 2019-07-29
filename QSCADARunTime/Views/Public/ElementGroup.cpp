#include "ElementGroup.h"
#include <QPainter>
#include <QCursor>

ElementGroup::ElementGroup()
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void ElementGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);
    painter->drawRect(boundingRect());

    if (isSelected()) {
        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::NoPen);
        painter->drawRect(QRectF(boundingRect().topLeft() - QPointF(5,5),boundingRect().topLeft() + QPointF(5,5)));
        painter->drawRect(QRectF(boundingRect().bottomRight() - QPointF(5,5),boundingRect().bottomRight() + QPointF(5,5)));
    }
}

void ElementGroup::setClickPosition(QPointF position) {

    prepareGeometryChange();
    setX(position.x());
    setY(position.y());
}
