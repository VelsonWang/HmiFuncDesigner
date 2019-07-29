#ifndef ELEMENTMOVINGTEXT_H
#define ELEMENTMOVINGTEXT_H

#include "PublicDefine.h"
#include "Element.h"
#include "functionproperty.h"
#include "tagtextlistproperty.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementMovingText : public Element
{
    Q_OBJECT

public:
    explicit ElementMovingText(const QString &szProjPath, const QString &szProjName);
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

    QString getMoveDirString(const QString& szAlign) const;
    void setMoveDirString(const QString& szAlign, QString& szAlignSet);

    enum {
        Type = MovingTextItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementMovingText &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementMovingText &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    void drawMovingText(QPainter *painter);

private:
    static int iLastIndex_;
    QRectF elementRect_;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagTextList_;
    // 移动方向
    QString szMoveDir_;
    // 移动字符数
    int iMoveCharNum_;
    // 移动间隔
    double period_;
    // 背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparentBackground_;
    // 字体
    QFont font_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;

    // ID
    TextProperty *idProperty_;
    // 标题
    EmptyProperty *titleProperty_;
    // 选择变量
    ListProperty *tagSelectProperty_;
    // 变量文本列表
    TagTextListProperty *tagTextListProperty_;
    // 文本
    TextProperty *elementTextProperty_;
    // 移动方向
    ListProperty *moveDirProperty_;
    // 移动字符数
    IntegerProperty *moveCharNumProperty_;
    // 移动间隔
    DoubleProperty *periodProperty_;
    // 背景颜色
    ColorProperty *backgroundColorProperty_;
    // 透明背景颜色
    BoolProperty *transparentBackgroundProperty_;
    // 字体
    FontProperty *fontProperty_;
    // 文本颜色
    ColorProperty *textColorProperty;
    // 边框宽度
    IntegerProperty *borderWidthProperty_;
    // 边框颜色
    ColorProperty *borderColorProperty_;
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


#endif // ELEMENTMOVINGTEXT_H
