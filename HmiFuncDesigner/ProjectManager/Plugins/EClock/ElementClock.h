#ifndef ELEMENTCLOCK_H
#define ELEMENTCLOCK_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementClock : public Element
{
    Q_OBJECT

public:
    explicit ElementClock(const QString &szProjPath,
                          const QString &szProjName,
                          QtVariantPropertyManager *propertyMgr);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(QtProperty *property, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    void writeAsXml(QXmlStreamWriter &) override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;
    void release() override; // 释放占用的资源

    int type() const override {
        return ClockItemType;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementClock &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementClock &textItem);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    void drawClock(QPainter *painter);
    void getSupportEvents(QStringList &listValue);

private:
    QStringList tagNames_;
    QStringList hAlignList_;
    QStringList vAlignList_;

private:
    static int iLastIndex_;
    // 时钟间隔
    double period_;
    // 选择功能
    QStringList funcs_;
    // 背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparentBackground_;
    // 字体
    QFont font_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
    // 显示日期
    bool showDate_;
    // 显示星期
    bool showWeek_;
    // 初始可见性
    bool showOnInitial_;
};

#endif // ELEMENTCLOCK_H
