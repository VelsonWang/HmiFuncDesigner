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
    void readFromXml(const QXmlStreamAttributes &);
    void readData(QDataStream &in);

    enum {Type = LineItemType};
    int type() const {
        return Type;
    }

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
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
};

#endif // ELEMENTLINE_H
