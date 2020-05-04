#ifndef ELEMENTJARSHAPE_H
#define ELEMENTJARSHAPE_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementJarShape : public Element
{
    Q_OBJECT

public:
    explicit ElementJarShape(const QString &szProjPath,
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
        return JarShapeItemType;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementJarShape &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementJarShape &textItem);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    void drawJarShape(QPainter *painter);

private:
    QStringList tagNames_;

private:
    static int iLastIndex_;
	// 关联的变量
	QString szTagSelected_;
    // 罐形容器标题
    QString jarShape_;
    // 字体
    QFont font_;
    // 罐体颜色
    QColor borderColor_;
    // 低于下限颜色
    QColor lessThanLowerLimitColor_;
    // 正常液面颜色
    QColor normalColor_;
    // 高于上限颜色
    QColor greaterThanUpperLimitColor_;
    // 上限值
    double upperLimitValue_;
    // 下限值
    double lowerLimitValue_;
    // 满量程值-刻度最大值。
    double maxValue_;
    // 刻度个数
    int scaleNum_;
    // 初始可见性
    bool showOnInitial_;
};


#endif // ELEMENTJARSHAPE_H
