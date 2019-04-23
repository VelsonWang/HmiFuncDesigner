#ifndef ELEMENTARROW_H
#define ELEMENTARROW_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementArrow : public Element
{
    Q_OBJECT
public:
    ElementArrow(const QString &projPath);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(uint id, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    void writeAsXml(QXmlStreamWriter &) override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;

    enum {Type = ArrowItemType};
    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementArrow &line);
    friend QDataStream &operator>>(QDataStream &in,ElementArrow &line);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    static int iLastIndex_;
    QLineF elementLine;
    qreal arrowSize;

    // 线条宽度
    int borderWidth_;
    // 线条颜色
    QColor borderColor_;

    // ID
    TextProperty *idProperty;
    // 标题
    EmptyProperty *titleProperty;
    // X坐标
    IntegerProperty *xCoordProperty;
    // Y坐标
    IntegerProperty *yCoordProperty;
    // Z坐标
    IntegerProperty *zValueProperty;
    // 宽度
    IntegerProperty *widthProperty_;
    // 高度
    IntegerProperty *heightProperty_;
    // 边框宽度
    IntegerProperty *borderWidthProperty_;
    // 边框颜色
    ColorProperty *borderColorProperty_;
    // 旋转角度
    IntegerProperty *angleProperty;
};

#endif // ELEMENTARROW_H
