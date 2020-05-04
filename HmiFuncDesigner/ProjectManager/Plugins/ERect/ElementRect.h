#ifndef ELEMENTRECT_H
#define ELEMENTRECT_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "Element.h"

class ElementRect : public Element
{
    Q_OBJECT
public:
    ElementRect(const QString &szProjPath,
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
        return RectItemType;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementRect &rect);
    friend QDataStream &operator>>(QDataStream &in,ElementRect &rect);

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
    // 填充颜色列表
    QStringList tagColorList_;
    // 填充颜色
    QColor fillColor_;
    // 是否填充颜色
    bool isFill_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 初始可见性
    bool showOnInitial_;
};



#endif // ELEMENTRECT_H
