#ifndef PUSHBUTTONITEM_HPP
#define PUSHBUTTONITEM_HPP

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>


class ElementPushButton : public Element
{
    Q_OBJECT

public:
    explicit ElementPushButton();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {Type = PushButtonItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementPushButton &textItem);

protected:
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
    bool isSelected_;
};

#endif // PUSHBUTTONITEM_HPP
