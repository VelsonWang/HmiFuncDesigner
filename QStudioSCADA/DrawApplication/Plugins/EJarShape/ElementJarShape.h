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
    explicit ElementJarShape(const QString &szProjPath, const QString &szProjName);
    void setClickPosition(QPointF) override;
    void updateBoundingElement() override;
    void updateElementProperty(uint id, const QVariant &value) override;
    void updatePropertyModel() override;
    void createPropertyList() override;
    void writeAsXml(QXmlStreamWriter &) override;
    void readFromXml(const QXmlStreamAttributes &) override;
    void writeData(QDataStream &out) override;
    void readData(QDataStream &in) override;
    void regenerateElementId() override;
    void release() override; // 释放占用的资源

    enum {
        Type = JarShapeItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementJarShape &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementJarShape &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)  override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    void drawJarShape(QPainter *painter);

private:
    static int iLastIndex_;
    QRectF elementRect;

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


    // ID
    TextProperty *idProperty_;
    // 标题
    EmptyProperty *titleProperty_;
	// 选择变量
	ListProperty *tagSelectProperty_;
    // 罐形容器标题
    TextProperty *jarShapeProperty_;
    // 字体
    FontProperty *fontProperty_;
    // 文本颜色
    ColorProperty *textColorProperty_;
    // 罐体颜色
    ColorProperty *borderColorProperty_;
    // 低于下限颜色
    ColorProperty *lessThanLowerLimitColorProperty_;
    // 正常液面颜色
    ColorProperty *normalColorProperty_;
    // 高于上限颜色
    ColorProperty *greaterThanUpperLimitColorProperty_;
    // 上限值
    DoubleProperty *upperLimitValueProperty_;
    // 下限值
    DoubleProperty *lowerLimitValueProperty_;
    // 满量程值-刻度最大值
    DoubleProperty *maxValueProperty_;
    // 刻度个数
    IntegerProperty *scaleNumProperty_;
    // 初始可见性
    BoolProperty *showOnInitialProperty_;
    // X坐标
    IntegerProperty *xCoordProperty;
    // Y坐标
    IntegerProperty *yCoordProperty;
    // Z坐标
    IntegerProperty *zValueProperty;
    // 宽度
    IntegerProperty *widthProperty;
    // 高度
    IntegerProperty *heightProperty;

};


#endif // ELEMENTJARSHAPE_H
