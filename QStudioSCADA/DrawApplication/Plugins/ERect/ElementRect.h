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
    ElementRect(const QString &projPath);
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void writeAsXml(QXmlStreamWriter &writer);
    void readFromXml(const QXmlStreamAttributes &);

    void writeData(QDataStream &out);
    void readData(QDataStream &in);

    enum {Type = RectItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementRect &rect);
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
    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    IntegerProperty *xCoordProperty;
    IntegerProperty *yCoordProperty;
    IntegerProperty *zValueProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
    ColorProperty *backColorProperty;
    ColorProperty *borderColorProperty;
    IntegerProperty *borderWidthProperty;
    IntegerProperty *angleProperty;
    BoolProperty *blockedProperty;

};



#endif // ELEMENTRECT_H
