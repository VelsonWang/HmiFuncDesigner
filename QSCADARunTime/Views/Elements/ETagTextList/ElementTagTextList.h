#ifndef ELEMENTTAGTEXTLIST_H
#define ELEMENTTAGTEXTLIST_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QRect>


class ElementTagTextList : public Element
{
    Q_OBJECT

public:
    explicit ElementTagTextList();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {
        Type = TagTextListItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementTagTextList &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawTagTextList(QPainter *painter);

private:
    QRectF elementRect_;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagTextList_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparentBackground_;
    // 字体
    QFont font_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;
};


#endif // ELEMENTTAGTEXTLIST_H
