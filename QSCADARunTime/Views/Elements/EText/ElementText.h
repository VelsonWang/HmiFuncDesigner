#ifndef TEXTITEM_HPP
#define TEXTITEM_HPP

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementText : public Element
{
    Q_OBJECT

public:
    explicit ElementText();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void readFromXml(const QXmlStreamAttributes &);
    void readData(QDataStream &in);

    enum {Type = TextItemType};

    int type() const {
        return Type;
    }

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

};

#endif // TEXTITEM_HPP
