#ifndef ELEMENTLINE_H
#define ELEMENTLINE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementLine : public Element
{
    Q_OBJECT
public:
    ElementLine();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void writeAsXml(QXmlStreamWriter &);
    void readFromXml(const QXmlStreamAttributes &);

    void writeData(QDataStream &out);
    void readData(QDataStream &in);

    enum {Type = LineItemType};
    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementLine &line);
    friend QDataStream &operator>>(QDataStream &in,ElementLine &line);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QLineF elementLine;
    QPointF p1;
    QPointF p2;

    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    IntegerProperty *xCoordProperty;
    IntegerProperty *yCoordProperty;
    IntegerProperty *zValueProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
    ColorProperty *backColorProperty;
    IntegerProperty *borderWidthProperty;
    IntegerProperty *angleProperty;
    BoolProperty *blockedProperty;

};

#endif // ELEMENTLINE_H
