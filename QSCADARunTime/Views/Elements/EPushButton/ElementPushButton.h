#ifndef PUSHBUTTONITEM_HPP
#define PUSHBUTTONITEM_HPP

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementPushButton : public Element
{
    Q_OBJECT

public:
    explicit ElementPushButton();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void readFromXml(const QXmlStreamAttributes &);
    void readData(QDataStream &in);

    enum {Type = PushButtonItemType};

    int type() const {
        return Type;
    }

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

private:
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
};

#endif // PUSHBUTTONITEM_HPP
