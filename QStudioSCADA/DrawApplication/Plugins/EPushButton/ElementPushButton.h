#ifndef PUSHBUTTONITEM_HPP
#define PUSHBUTTONITEM_HPP

#include "PublicDefine.h"
#include "Element.h"
#include "functionproperty.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>


class ElementPushButton : public Element
{
    Q_OBJECT

public:
    explicit ElementPushButton(const QString &projPath);
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void writeAsXml(QXmlStreamWriter &);
    void readFromXml(const QXmlStreamAttributes &);

    void writeData(QDataStream &out);
    void readData(QDataStream &in);

    enum {Type = PushButtonItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementPushButton &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementPushButton &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawPushButton(QPainter *painter);
    void getSupportEvents(QStringList &listValue);

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

    ColorProperty *textColorProperty;
    IntegerProperty *fontSizeProperty;
    IntegerProperty *angleProperty;
    FunctionProperty *funcProperty;
};

#endif // PUSHBUTTONITEM_HPP
