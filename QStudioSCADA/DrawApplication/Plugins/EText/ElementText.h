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
    QString *szHAlign_;
    // 垂直对齐
    QString *szVAlign_;


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


    ColorProperty *textColorProperty;
    IntegerProperty *fontSizeProperty;
    IntegerProperty *angleProperty;
    BoolProperty *blockedProperty;

};

#endif // TEXTITEM_HPP
