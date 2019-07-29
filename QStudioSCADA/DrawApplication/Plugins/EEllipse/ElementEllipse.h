#ifndef ELEMENTELLIPSE_H
#define ELEMENTELLIPSE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"
#include "tagcolorlistproperty.h"

class ElementEllipse : public Element
{
    Q_OBJECT
public:
    ElementEllipse(const QString &szProjPath, const QString &szProjName);
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
    void release() override; // 释放占用的资源

    enum {Type = EllipseItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementEllipse &rect);
    friend QDataStream &operator>>(QDataStream &in,ElementEllipse &rect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    static int iLastIndex_;
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

    QRectF elementRect;

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
    // 选择变量
    ListProperty *tagSelectProperty_;
    // 填充颜色列表
    TagColorListProperty *tagColorListProperty_;
    // 填充颜色
    ColorProperty *fillColorProperty_;
    // 是否填充颜色
    BoolProperty *isFillProperty_;
    // 边框宽度
    IntegerProperty *borderWidthProperty_;
    // 边框颜色
    ColorProperty *borderColorProperty_;
    // 初始可见性
    BoolProperty *showOnInitialProperty_;
    // 旋转角度
    IntegerProperty *angleProperty;

};

#endif // ELEMENTELLIPSE_H
