#ifndef ELEMENTARROW_H
#define ELEMENTARROW_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementArrow : public Element
{
    Q_OBJECT
public:
    ElementArrow();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void readFromXml(const QXmlStreamAttributes &);
    void readData(QDataStream &in);

    enum {Type = ArrowItemType};
    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementArrow &line);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QLineF elementLine;
    qreal arrowSize;
};

#endif // ELEMENTARROW_H
