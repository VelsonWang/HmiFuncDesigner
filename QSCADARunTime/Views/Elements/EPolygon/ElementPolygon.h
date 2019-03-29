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

    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void addNodePoint();
    void removeNodePoint();
    void writeAsXml(QXmlStreamWriter &);
    void readFromXml(const QXmlStreamAttributes &);

    void writeData(QDataStream &out);
    void readData(QDataStream &in);

    enum {Type = PolygonItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementPolygon &rect);
    friend QDataStream &operator>>(QDataStream &in,ElementPolygon &rect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
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

    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    IntegerProperty *xCoordProperty;
    IntegerProperty *yCoordProperty;
    IntegerProperty *zValueProperty;
    ColorProperty *backColorProperty;
    ColorProperty *borderColorProperty;
    IntegerProperty *borderWidthProperty;
    IntegerProperty *angleProperty;
    BoolProperty *blockedProperty;

};

#endif // ELEMENTPOLYGON_H
