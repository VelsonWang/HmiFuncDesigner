#ifndef TEXTITEM_HPP
#define TEXTITEM_HPP

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>

class ElementText : public Element
{
    Q_OBJECT

public:
    explicit ElementText();
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void readData(QDataStream &in) override;
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    enum {Type = TextItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator>>(QDataStream &in, ElementText &textItem);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawText(QPainter *painter);
    void refreshTagValue();

private:
    QRectF elementRect;
	// 关联的变量
	QString szTagSelected_;
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
    // 点击隐藏
    bool hideOnClick_;
    // 初始可见性
    bool showOnInitial_;
    // 隐藏
    bool bHide_;
};

#endif // TEXTITEM_HPP
