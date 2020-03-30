#ifndef ELEMENTVALUESTICK_H
#define ELEMENTVALUESTICK_H

#include "../../Public/Element.h"
#include <QPainter>
#include <QDataStream>
#include <QRect>


class ElementValueStick : public Element
{
    Q_OBJECT

public:
    explicit ElementValueStick();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return ValueStickItemType;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementValueStick &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawValueStick(QPainter *painter);
    void drawScalarStick(QPainter *painter,
                         QRect rect,
                         QRect textRect,
                         QRect scaleRect,
                         double dMinValue,
                         double dMaxValue,
                         int wScaleNum,
                         QColor fillColor,
                         bool scaleVisible,
                         QColor scaleColor,
                         QString scaleDirect,
                         QString scalePosition);

private:
    QRectF elementRect_;

    // 关联的变量
    QString szTagSelected_;
    // 刻度最大值。
    double maxValue_;
    // 刻度最小值。
    double minValue_;
    // 刻度个数
    int scaleNum_;
    // 背景颜色
    QColor backgroundColor_;
    // 前景颜色
    QColor foregroundColor_;
    // 标尺颜色
    QColor scaleColor_;
    // 标尺方向
    QString scaleDir_;
    // 标尺位置
    QString scalePos_;
    // 字体
    QFont font_;
    // 显示标尺
    bool showRuler_;
    // 显示刻度
    bool showScale_;
    // 初始可见性
    bool showOnInitial_;
};


#endif // ELEMENTVALUESTICK_H
