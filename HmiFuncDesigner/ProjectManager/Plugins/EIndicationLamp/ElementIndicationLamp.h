#ifndef ELEMENTPICTURE_H
#define ELEMENTPICTURE_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementIndicationLamp : public Element
{
    Q_OBJECT
public:
    ElementIndicationLamp(const QString &szProjPath,
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
        return IndicationLampItemType;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementIndicationLamp &IndicationLamp);
    friend QDataStream &operator>>(QDataStream &in,ElementIndicationLamp &IndicationLamp);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    QStringList tagNames_;

private:
    static int iLastIndex_;
    // 关联的变量
    QString szTagSelected_;
    // 初始状态
    bool stateOnInitial_;
    // 复位图片
    QString resetFileIndicationLamp_;
    // 置位图片
    QString setFileIndicationLamp_;
    // 原尺寸显示
    bool showNoScale_;
    // 初始可见性
    bool showOnInitial_;
};



#endif // ELEMENTPICTURE_H
