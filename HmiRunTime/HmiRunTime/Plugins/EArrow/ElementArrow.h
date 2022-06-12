#ifndef ELEMENTARROW_H
#define ELEMENTARROW_H

#include <QMouseEvent>
#include <QPainter>
#include <QDataStream>
#include "../../Public/Element.h"

class ElementArrow : public Element
{
    Q_OBJECT
public:
    ElementArrow();
    ~ElementArrow() override;
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return ArrowItemType;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementArrow &line);

protected:
    QRectF boundingRect() const;

private:
    QLineF elementLine;
    qreal arrowSize;
    // 线条宽度
    int borderWidth_;
    // 线条颜色
    QColor borderColor_;
};

#endif // ELEMENTARROW_H
