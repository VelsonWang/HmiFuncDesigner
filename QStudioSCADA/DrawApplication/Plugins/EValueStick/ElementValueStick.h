#ifndef ELEMENTVALUESTICK_H
#define ELEMENTVALUESTICK_H

#include "PublicDefine.h"
#include "Element.h"
#include <QPainter>
#include <QDataStream>
#include <QGraphicsSceneMouseEvent>

class ElementValueStick : public Element
{
    Q_OBJECT

public:
    explicit ElementValueStick(const QString &szProjPath, const QString &szProjName);
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

    QString getDirString(const QString& szDir) const;
    void setDirString(const QString& szDir, QString& szDirSet);

    QString getPosString(const QString& szPos) const;
    void setPosString(const QString& szPos, QString& szPosSet);

    enum {
        Type = ValueStickItemType
    };

    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementValueStick &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementValueStick &textItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)  override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    void drawValueStick(QPainter *painter);
    void drawScalarStick(QPainter *painter,
                         QRect rect,
                         QRect textRect,
                         QRect scaleRect,
                         double dMinValue,
                         double dMaxValue,
                         int wScaleNum,
                         QColor fillColor,
                         bool scaleVisible,
                         QColor scaleColor,
                         QString scaleDirect,
                         QString scalePosition);

private:
    static int iLastIndex_;
    QRectF elementRect;

	// 关联的变量
	QString szTagSelected_;
    // 刻度最大值。
    double maxValue_;
    // 刻度最小值。
    double minValue_;
    // 刻度个数
    int scaleNum_;
    // 背景颜色
    QColor backgroundColor_;
    // 前景颜色
    QColor foregroundColor_;
    // 标尺颜色
    QColor scaleColor_;
    // 标尺方向
    QString scaleDir_;
    // 标尺位置
    QString scalePos_;
    // 字体
    QFont font_;
    // 显示标尺
    bool showRuler_;
    // 显示刻度
    bool showScale_;
    // 初始可见性
    bool showOnInitial_;

    // ID
    TextProperty *idProperty_;
    // 标题
    EmptyProperty *titleProperty_;
	// 选择变量
	ListProperty *tagSelectProperty_;
    // 刻度最大值
    DoubleProperty *maxValueProperty_;
    // 刻度最小值
    DoubleProperty *minValueProperty_;
    // 刻度个数
    IntegerProperty *scaleNumProperty_;
    // 背景颜色
    ColorProperty *backgroundColorProperty_;
    // 前景颜色
    ColorProperty *foregroundColorProperty_;
    // 标尺颜色
    ColorProperty *scaleColorProperty_;
    // 标尺方向
    ListProperty *scaleDirProperty_;
    // 标尺位置
    ListProperty *scalePosProperty_;
    // 字体
    FontProperty *fontProperty_;
    // 文本颜色
    ColorProperty *textColorProperty_;
    // 显示标尺
    BoolProperty *showRulerProperty_;
    // 显示刻度
    BoolProperty *showScaleProperty_;
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

#endif // ELEMENTVALUESTICK_H
