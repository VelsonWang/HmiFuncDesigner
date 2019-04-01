#ifndef ELEMENTRECT_H
#define ELEMENTRECT_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementRect : public Element
{
    Q_OBJECT
public:
    ElementRect();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void readFromXml(const QXmlStreamAttributes &);
    void readData(QDataStream &in);

    enum {Type = RectItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementRect &rect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    QRectF elementRect;

};



#endif // ELEMENTRECT_H
