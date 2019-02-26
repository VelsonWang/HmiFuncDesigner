#ifndef TEXTITEM_HPP
#define TEXTITEM_HPP

#include "itemtypes.h"
#include "graphicselements/figure.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementText : public Figure
{
    Q_OBJECT

public:
    explicit ElementText();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void writeAsXml(QXmlStreamWriter &);
    void readFromXml(const QXmlStreamAttributes &);

    enum {Type = TextItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementText &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementText &textItem);

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
    void drawText(QPainter *painter);
    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    IntegerProperty *xCoordProperty;
    IntegerProperty *yCoordProperty;
    IntegerProperty *zValueProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
    TextProperty *elementTextProperty;
    ColorProperty *textColorProperty;
    IntegerProperty *fontSizeProperty;
    IntegerProperty *angleProperty;
    BoolProperty *blockedProperty;
    EmptyProperty *serviceProperty;
    TextProperty *messageTypeProperty;
};

#endif // TEXTITEM_HPP
