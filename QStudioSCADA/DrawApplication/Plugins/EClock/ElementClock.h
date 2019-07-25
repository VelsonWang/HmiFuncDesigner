#ifndef ELEMENTCLOCK_H
#define ELEMENTCLOCK_H

#include "PublicDefine.h"
#include "Element.h"
#include "functionproperty.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementClock : public Element
{
    Q_OBJECT

public:
    explicit ElementClock(const QString &szProjPath, const QString &szProjName);
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

    enum {
        Type = ClockItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementClock &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementClock &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    void drawClock(QPainter *painter);
    void getSupportEvents(QStringList &listValue);

private:
    static int iLastIndex_;
    QRectF elementRect_;
    // 时钟间隔
    double period_;
    // 选择功能
    QStringList funcs_;
    // 背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparentBackground_;
    // 字体
    QFont font_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 显示日期
    bool showDate_;
    // 显示星期
    bool showWeek_;
    // 初始可见性
    bool showOnInitial_;

    // ID
    TextProperty *idProperty_;
    // 标题
    EmptyProperty *titleProperty_;
    // 时钟间隔
    DoubleProperty *periodProperty_;
    // 选择功能
    FunctionProperty *funcProperty_;
    // 背景颜色
    ColorProperty *backgroundColorProperty_;
    // 透明背景颜色
    BoolProperty *transparentBackgroundProperty_;
    // 字体
    FontProperty *fontProperty_;
    // 文本颜色
    ColorProperty *textColorProperty;
    // 水平对齐
    ListProperty *hAlignProperty_;
    // 垂直对齐
    ListProperty *vAlignProperty_;
    // 边框宽度
    IntegerProperty *borderWidthProperty_;
    // 边框颜色
    ColorProperty *borderColorProperty_;
    // 显示日期
    BoolProperty *showDateProperty_;
    // 显示星期
    BoolProperty *showWeekProperty_;
    // 初始可见性
    BoolProperty *showOnInitialProperty_;
    // X坐标
    IntegerProperty *xCoordProperty_;
    // Y坐标
    IntegerProperty *yCoordProperty_;
    // Z坐标
    IntegerProperty *zValueProperty_;
    // 宽度
    IntegerProperty *widthProperty_;
    // 高度
    IntegerProperty *heightProperty_;
};

#endif // ELEMENTCLOCK_H
