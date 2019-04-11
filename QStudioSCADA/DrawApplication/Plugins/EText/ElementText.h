#ifndef TEXTITEM_HPP
#define TEXTITEM_HPP

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementText : public Element
{
    Q_OBJECT

public:
    explicit ElementText(const QString &projPath);
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void writeAsXml(QXmlStreamWriter &);
    void readFromXml(const QXmlStreamAttributes &);

    void writeData(QDataStream &out);
    void readData(QDataStream &in);

    // 获取分配的索引值
    int getIndexFromIDString(const QString &szID);
    QString getHAlignString() const;
    void setHAlignString(const QString& szAlign);
    QString getVAlignString() const;
    void setVAlignString(const QString& szAlign);

    enum {Type = TextItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementText &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementText &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    void drawText(QPainter *painter);


private:
    static int iLastIndex_;
    QRectF elementRect;

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

    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    IntegerProperty *xCoordProperty;
    IntegerProperty *yCoordProperty;
    IntegerProperty *zValueProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
    TextProperty *elementTextProperty;
    // 水平对齐
    ListProperty *hAlignProperty_;
    // 垂直对齐
    ListProperty *vAlignProperty_;
    // 背景颜色
    ColorProperty *backgroundColorProperty_;
    // 透明背景颜色
    BoolProperty *transparentBackgroundProperty_;
    // 字体
    FontProperty *fontProperty_;
    // 文本颜色
    ColorProperty *textColorProperty;
    // 边框宽度
    IntegerProperty *borderWidthProperty_;
    // 边框颜色
    ColorProperty *borderColorProperty_;
    // 点击隐藏
    BoolProperty *hideOnClickProperty_;
    // 初始可见性
    BoolProperty *showOnInitialProperty_;
    // 旋转
    IntegerProperty *angleProperty;

};

#endif // TEXTITEM_HPP
