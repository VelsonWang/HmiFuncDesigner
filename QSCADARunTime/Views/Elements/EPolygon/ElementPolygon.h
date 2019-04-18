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
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void addNodePoint();
    void removeNodePoint();
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {Type = PolygonItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementPolygon &rect);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void createPath();
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
