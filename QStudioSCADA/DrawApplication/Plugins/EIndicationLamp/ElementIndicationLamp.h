#ifndef ELEMENTPICTURE_H
#define ELEMENTPICTURE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementIndicationLamp : public Element
{
    Q_OBJECT
public:
    ElementIndicationLamp(const QString &szProjPath, const QString &szProjName);
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

    enum {Type = IndicationLampItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementIndicationLamp &IndicationLamp);
    friend QDataStream &operator>>(QDataStream &in,ElementIndicationLamp &IndicationLamp);

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
    // 初始状态
    bool stateOnInitial_;
    // 复位图片
    QString resetFileIndicationLamp_;
    // 置位图片
    QString setFileIndicationLamp_;

    QRectF elementRect;

    // 原尺寸显示
    bool showNoScale_;
    // 初始可见性
    bool showOnInitial_;

    // ID
    TextProperty *idProperty;
    // 标题
    EmptyProperty *titleProperty;
    // 选择变量
    ListProperty *tagSelectProperty_;
    // 初始状态
    BoolProperty *stateOnInitialProperty_;
    // 选择复位图片
    FileProperty *resetFileProperty_;
    // 选择置位图片
    FileProperty *setFileProperty_;

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
    // 原尺寸显示
    BoolProperty *showNoScaleProperty_;
    // 初始可见性
    BoolProperty *showOnInitialProperty_;

};



#endif // ELEMENTPICTURE_H
