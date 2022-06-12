#ifndef ELEMENTLINE_H
#define ELEMENTLINE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

class ElementLine : public Element
{
    Q_OBJECT
public:
    ElementLine(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(QtProperty *property, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    bool openFromXml(XMLObject *pXmlObj) override;
    bool saveToXml(XMLObject *pXmlObj) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;
    void release() override; // 释放占用的资源

    int type() const override {
        return LineItemType;
    }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    static int iLastIndex_;
    QLineF elementLine;
    QPointF p1;
    QPointF p2;

    // 线条宽度
    int borderWidth_;
    // 线条颜色
    QColor borderColor_;
};

#endif // ELEMENTLINE_H
