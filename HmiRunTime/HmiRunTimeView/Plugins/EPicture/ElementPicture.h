#ifndef ELEMENTPICTURE_H
#define ELEMENTPICTURE_H

#include <QMouseEvent>
#include <QPainter>
#include <QDataStream>
#include "../../Public/Element.h"

class ElementPicture : public Element
{
    Q_OBJECT
public:
    ElementPicture();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int type() const {
        return PictureItemType;
    }

    friend QDataStream &operator>>(QDataStream &in,ElementPicture &Picture);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    QRectF elementRect;
    QString filePicture_;
    // 原尺寸显示
    bool showNoScale_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;
};



#endif // ELEMENTPICTURE_H
