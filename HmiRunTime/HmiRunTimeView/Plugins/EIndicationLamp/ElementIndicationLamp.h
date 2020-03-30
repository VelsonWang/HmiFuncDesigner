#ifndef ELEMENTINDICATIONLAM_H
#define ELEMENTINDICATIONLAM_H

#include <QMouseEvent>
#include <QPainter>
#include <QDataStream>
#include "../../Public/Element.h"

class ElementIndicationLamp : public Element
{
    Q_OBJECT
public:
    ElementIndicationLamp();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return IndicationLampItemType;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementIndicationLamp &IndicationLamp);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    // 关联的变量
    QString szTagSelected_;
    // 初始状态
    bool stateOnInitial_;
    // 复位图片
    QString resetFileIndicationLamp_;
    // 置位图片
    QString setFileIndicationLamp_;
    // 矩形区域
    QRectF elementRect;
    // 原尺寸显示
    bool showNoScale_;
    // 初始可见性
    bool showOnInitial_;

};



#endif // ELEMENTINDICATIONLAM_H
