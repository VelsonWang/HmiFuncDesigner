#ifndef SWITCHBUTTONITEM_H
#define SWITCHBUTTONITEM_H

#include "PublicDefine.h"
#include "Element.h"
#include "functionproperty.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>


class ElementSwitchButton : public Element
{
    Q_OBJECT

public:
    explicit ElementSwitchButton(const QString &szProjPath, const QString &szProjName);
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

    enum {Type = SwitchButtonItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementSwitchButton &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementSwitchButton &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawSwitchButton(QPainter *painter);
    void getSupportEvents(QStringList &listValue);

    void reloadPropertyList();
    void updatePropertyTableView();

private:
    static int iLastIndex_;
    QRectF elementRect;
    QStringList funcs_;
    // 显示内容
    QString showContent_;
    bool bShowContentText_;
    // 图片名
    QString filePicture_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 按钮背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparent_;
    // 字体
    QFont font_;
    // 初始有效性
    bool enableOnInitial_;
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
    // 显示内容
    ListProperty *showContentProperty_;
    // 选择图片
    FileProperty *fileProperty;
    // 显示文本
    TextProperty *elementTextProperty;
    // 水平对齐
    ListProperty *hAlignProperty_;
    // 垂直对齐
    ListProperty *vAlignProperty_;
    // 按钮背景颜色
    ColorProperty *backgroundColorProperty_;
    // 透明
    BoolProperty *transparentProperty_;
    // 字体
    FontProperty *fontProperty_;
    // 文本颜色
    ColorProperty *textColorProperty;
    // 旋转角度
    IntegerProperty *angleProperty;
    // 初始有效性
    BoolProperty *enableOnInitialProperty_;
    // 初始可见性
    BoolProperty *showOnInitialProperty_;
    // 选择功能
    FunctionProperty *funcProperty;
};

#endif // SWITCHBUTTONITEM_H
