#ifndef ELEMENTANALOGCLOCK_H
#define ELEMENTANALOGCLOCK_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>

class ElementAnalogClock : public Element
{
    Q_OBJECT

public:
    explicit ElementAnalogClock();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {
        Type = AnalogClockItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementAnalogClock &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawAnalogClock(QPainter *painter);

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
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;

    qint64 iLastMSecs_;
};

#endif // ELEMENTANALOGCLOCK_H
