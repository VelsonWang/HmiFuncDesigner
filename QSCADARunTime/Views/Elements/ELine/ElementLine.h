#ifndef ELEMENTLINE_H
#define ELEMENTLINE_H

#include <QMouseEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementLine : public Element
{
    Q_OBJECT
public:
    ElementLine();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {Type = LineItemType};
    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementLine &line);

protected:
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
