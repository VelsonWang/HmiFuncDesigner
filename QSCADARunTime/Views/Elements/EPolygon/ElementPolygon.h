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
    void createPath();
    bool hasClickedOn(QPointF pressPoint,QPointF point) const;
    void createPoints();
    QString createPointsXmlString() const;

private:
    QPolygonF polygon;
    QVector <QPointF> points;   
    int clickPoint;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagColorList_;
    // 填充颜色
    QColor fillColor_;
    // 是否填充颜色
    bool isFill_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;
};

#endif // ELEMENTPOLYGON_H
