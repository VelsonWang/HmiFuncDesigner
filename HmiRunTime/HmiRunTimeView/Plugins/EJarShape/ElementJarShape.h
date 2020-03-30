#ifndef ELEMENTJARSHAPE_H
#define ELEMENTJARSHAPE_H

#include "../../Public/Element.h"
#include <QPainter>
#include <QDataStream>
#include <QRect>


class ElementJarShape : public Element
{
    Q_OBJECT

public:
    explicit ElementJarShape();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return JarShapeItemType;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementJarShape &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawJarShape(QPainter *painter);

private:
    static int iLastIndex_;
    QRectF elementRect_;

    // 关联的变量
    QString szTagSelected_;
    // 罐形容器标题
    QString jarShape_;
    // 字体
    QFont font_;
    // 罐体颜色
    QColor borderColor_;
    // 低于下限颜色
    QColor lessThanLowerLimitColor_;
    // 正常液面颜色
    QColor normalColor_;
    // 高于上限颜色
    QColor greaterThanUpperLimitColor_;
    // 上限值
    double upperLimitValue_;
    // 下限值
    double lowerLimitValue_;
    // 满量程值-刻度最大值。
    double maxValue_;
    // 刻度个数
    int scaleNum_;
    // 初始可见性
    bool showOnInitial_;

};


#endif // ELEMENTJARSHAPE_H
