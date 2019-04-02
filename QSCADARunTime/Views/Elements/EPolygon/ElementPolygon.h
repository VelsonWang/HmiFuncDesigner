#ifndef ELEMENTPOLYGON_H
#define ELEMENTPOLYGON_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementPolygon : public Element
{
public:
    ElementPolygon();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void addNodePoint();
    void removeNodePoint();
    void readFromXml(const QXmlStreamAttributes &);
    void readData(QDataStream &in);

    enum {Type = PolygonItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementPolygon &rect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    QPolygonF polygon;
    QVector <QPointF> points;
    void createPath();
    bool hasClickedOn(QPointF pressPoint,QPointF point) const;
    void createPoints();
    QString createPointsXmlString() const;
    int clickPoint;
};

#endif // ELEMENTPOLYGON_H
