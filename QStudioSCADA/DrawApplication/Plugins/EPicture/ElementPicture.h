#ifndef ELEMENTPICTURE_H
#define ELEMENTPICTURE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementPicture : public Element
{
    Q_OBJECT
public:
    ElementPicture(const QString &projPath);
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void writeAsXml(QXmlStreamWriter &writer);
    void readFromXml(const QXmlStreamAttributes &);

    void writeData(QDataStream &out);
    void readData(QDataStream &in);

    enum {Type = PictureItemType};

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementPicture &Picture);
    friend QDataStream &operator>>(QDataStream &in,ElementPicture &Picture);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    QRectF elementRect;
    QString filePicture_;
    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    IntegerProperty *xCoordProperty;
    IntegerProperty *yCoordProperty;
    IntegerProperty *zValueProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
    FileProperty *fileProperty;
    IntegerProperty *angleProperty;


};



#endif // ELEMENTPICTURE_H
