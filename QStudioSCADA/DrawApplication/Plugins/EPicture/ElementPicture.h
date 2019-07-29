#ifndef ELEMENTPICTURE_H
#define ELEMENTPICTURE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementPicture : public Element
{
    Q_OBJECT
public:
    ElementPicture(const QString &szProjPath, const QString &szProjName);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(uint id, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    void writeAsXml(QXmlStreamWriter &writer) override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;
    void release() override; // 释放占用的资源

    enum {Type = PictureItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementPicture &Picture);
    friend QDataStream &operator>>(QDataStream &in,ElementPicture &Picture);

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
    QRectF elementRect;
    QString filePicture_;
    // 原尺寸显示
    bool showNoScale_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;

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
    // 选择图片
    FileProperty *fileProperty;
    // 原尺寸显示
    BoolProperty *showNoScaleProperty_;
    // 边框宽度
    IntegerProperty *borderWidthProperty_;
    // 边框颜色
    ColorProperty *borderColorProperty_;
    // 初始可见性
    BoolProperty *showOnInitialProperty_;
    // 旋转角度
    IntegerProperty *angleProperty;


};



#endif // ELEMENTPICTURE_H
