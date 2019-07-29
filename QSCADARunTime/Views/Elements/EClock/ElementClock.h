#ifndef ELEMENTCLOCK_H
#define ELEMENTCLOCK_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>

class ElementClock : public Element
{
    Q_OBJECT

public:
    explicit ElementClock();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {
        Type = ClockItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementClock &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawClock(QPainter *painter);

private:
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

    qint64 iLastMSecs_;
};

#endif // ELEMENTCLOCK_H
