#ifndef ELEMENTTAGTEXTLIST_H
#define ELEMENTTAGTEXTLIST_H

#include "PublicDefine.h"
#include "Element.h"
#include "functionproperty.h"
#include "tagtextlistproperty.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementTagTextList : public Element
{
    Q_OBJECT

public:
    explicit ElementTagTextList(const QString &szProjPath, const QString &szProjName);
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
        Type = TagTextListItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementTagTextList &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementTagTextList &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    void drawTagTextList(QPainter *painter);

private:
    static int iLastIndex_;
    QRectF elementRect_;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagTextList_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
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
    // 水平对齐
    ListProperty *hAlignProperty_;
    // 垂直对齐
    ListProperty *vAlignProperty_;
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


#endif // ELEMENTTAGTEXTLIST_H
