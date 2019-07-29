#ifndef ELEMENTMOVINGTEXT_H
#define ELEMENTMOVINGTEXT_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QRect>


class ElementMovingText : public Element
{
    Q_OBJECT

public:
    explicit ElementMovingText();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {
        Type = MovingTextItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementMovingText &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawMovingText(QPainter *painter);

private:
    QRectF elementRect_;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagTextList_;
    // 移动方向
    QString szMoveDir_;
    // 移动字符数
    int iMoveCharNum_;
    // 移动间隔
    double period_;
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

    qint64 iLastMSecs_;
    // 文本矩形区域x
    int iPos_;
    // 文本矩形区域
    QRect textRect_;
    // 单个字符移动宽度
    int iOneCharWidth_;
    // 当前移动的字符串
    QString szMovingText_;
};


#endif // ELEMENTMOVINGTEXT_H
