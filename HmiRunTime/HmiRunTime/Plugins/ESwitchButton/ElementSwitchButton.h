#ifndef SWITCHBUTTONITEM_HPP
#define SWITCHBUTTONITEM_HPP

#include "../../Public/Element.h"
#include <QPainter>
#include <QDataStream>


class ElementSwitchButton : public Element
{
    Q_OBJECT

public:
    explicit ElementSwitchButton();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return SwitchButtonItemType;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementSwitchButton &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawSwitchButton(QPainter *painter);
    void execScriptFunction(bool bVal);

private:
    QRectF elementRect;
    QStringList funcs_;
    // 关联的变量
    QString szTagSelected_;
    // 初始状态
    bool stateOnInitial_;
    // 显示内容
    QString showContent_;
    bool bShowContentText_;

    // 复位图片
    QString resetPictureFile_;
    // 置位图片
    QString setPictureFile_;
    // 原尺寸显示
    bool showNoScale_;

    // 复位显示文本
    QString resetText_;
    // 置位显示文本
    QString setText_;

    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 复位按钮背景颜色
    QColor resetBackgroundColor_;
    // 置位按钮背景颜色
    QColor setBackgroundColor_;
    // 透明背景颜色
    bool transparent_;
    // 字体
    QFont font_;
    // 初始有效性
    bool enableOnInitial_;
    // 初始可见性
    bool showOnInitial_;
    // 是否选中
    bool isSelected_;
    // 上一次读取的变量值
    bool bLastTagVal_ = false;
};

#endif // SWITCHBUTTONITEM_HPP
