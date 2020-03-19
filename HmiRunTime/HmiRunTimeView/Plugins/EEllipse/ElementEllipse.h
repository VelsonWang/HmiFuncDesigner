#ifndef ELEMENTELLIPSE_H
#define ELEMENTELLIPSE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "../../Public/Element.h"

class ElementEllipse : public Element
{
    Q_OBJECT
public:
    ElementEllipse();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return EllipseItemType;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementEllipse &rect);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    QRectF elementRect;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagColorList_;
    // 填充颜色
    QColor fillColor_;
    // 上一次填充颜色
    QColor lastFillColor_;
    // 是否填充颜色
    bool isFill_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;
};

#endif // ELEMENTELLIPSE_H
