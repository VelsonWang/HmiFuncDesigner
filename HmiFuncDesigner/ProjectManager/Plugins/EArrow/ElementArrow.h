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
    ElementArrow(const QString &szProjPath,
                 const QString &szProjName,
                 QtVariantPropertyManager *propertyMgr);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(QtProperty *property, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    void writeAsXml(QXmlStreamWriter &) override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;
    void release() override; // 释放占用的资源

    int type() const override {
        return ArrowItemType;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementArrow &line);
    friend QDataStream &operator>>(QDataStream &in,ElementArrow &line);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    static int iLastIndex_;
    QLineF elementLine;
    qreal arrowSize;

    // 线条宽度
    int borderWidth_;
    // 线条颜色
    QColor borderColor_;
};

#endif // ELEMENTARROW_H
