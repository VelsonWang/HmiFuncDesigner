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
    ElementPicture(const QString &szProjPath,
                   const QString &szProjName,
                   QtVariantPropertyManager *propertyMgr);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(QtProperty *property, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    void writeAsXml(QXmlStreamWriter &writer) override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;
    void release() override; // 释放占用的资源

    int type() const override {
        return PictureItemType;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementPicture &Picture);
    friend QDataStream &operator>>(QDataStream &in,ElementPicture &Picture);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    static int iLastIndex_;
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
