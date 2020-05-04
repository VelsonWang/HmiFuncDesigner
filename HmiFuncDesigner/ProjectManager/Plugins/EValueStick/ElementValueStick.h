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
    explicit ElementValueStick(const QString &szProjPath,
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

    QString getDirString(const QString& szDir) const;
    void setDirString(const QString& szDir, QString& szDirSet);

    QString getPosString(const QString& szPos) const;
    void setPosString(const QString& szPos, QString& szPosSet);

    int type() const override {
        return ValueStickItemType;
    }

    friend QDataStream &operator<<(QDataStream &out, const ElementValueStick &textItem);
    friend QDataStream &operator>>(QDataStream &in, ElementValueStick &textItem);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

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
    QStringList tagNames_;
    QStringList scaleDirList_;
    QStringList scalePosList_;

private:
    static int iLastIndex_;
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
};

#endif // ELEMENTVALUESTICK_H
