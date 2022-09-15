#ifndef QVALUESTICK_H
#define QVALUESTICK_H

#include <QWidget>
#include <QPaintEvent>
#include "iloader.h"
#include "realtimedb.h"

class QValueStick : public QWidget, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)
    Q_PROPERTY(double maxValue READ getMaxValue WRITE setMaxValue)
    Q_PROPERTY(double minValue READ getMinValue WRITE setMinValue)
    Q_PROPERTY(int scaleNum READ getScaleNum WRITE setScaleNum)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor foregroundColor READ getForegroundColor WRITE setForegroundColor)
    Q_PROPERTY(QColor scaleColor READ getScaleColor WRITE setScaleColor)
    Q_PROPERTY(QString scaleDir READ getScaleDir WRITE setScaleDir)
    Q_PROPERTY(QString scalePos READ getScalePos WRITE setScalePos)
    Q_PROPERTY(QFont font READ getFont WRITE setFont)
    Q_PROPERTY(bool showRuler READ getShowRuler WRITE setShowRuler)
    Q_PROPERTY(bool showScale READ getShowScale WRITE setShowScale)
    Q_PROPERTY(bool showOnInitial READ getShowOnInitial WRITE setShowOnInitial)

public:
    Q_INVOKABLE QValueStick(QWidget *parent = nullptr);

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

    double getMaxValue() const;
    void setMaxValue(double value);

    double getMinValue() const;
    void setMinValue(double value);

    int getScaleNum() const;
    void setScaleNum(int value);

    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &value);

    QColor getForegroundColor() const;
    void setForegroundColor(const QColor &value);

    QColor getScaleColor() const;
    void setScaleColor(const QColor &value);

    QString getScaleDir() const;
    void setScaleDir(const QString &value);

    QString getScalePos() const;
    void setScalePos(const QString &value);

    QFont getFont() const;
    void setFont(const QFont &value);

    bool getShowRuler() const;
    void setShowRuler(bool value);

    bool getShowScale() const;
    void setShowScale(bool value);

    bool getShowOnInitial() const;
    void setShowOnInitial(bool value);

    QColor getTextColor() const;
    void setTextColor(const QColor &value);

public:
    void fromObject(XMLObject* xml) override;

private:
    void setPropertyInner();
    void drawScalarStick(QPainter *painter,
                         QRect rect,
                         QRect textRect,
                         QRect scaleRect,
                         double dMinValue,
                         double dMaxValue,
                         int iScaleNum,
                         QColor fillColor,
                         bool scaleVisible,
                         QColor scaleColor,
                         QString scaleDirect,
                         QString scalePosition);
    void drawValueStick(QPainter *painter);
    QString dirString(const QString& szDir) const;
    QString posString(const QString& szPos) const;

protected:
    void paintEvent(QPaintEvent *event) override;;
    void resizeEvent(QResizeEvent *event) override;;

private:
    // 关联的变量
    QString tagSelected;
    // 刻度最大值。
    double maxValue;
    // 刻度最小值。
    double minValue;
    // 刻度个数
    int scaleNum;
    // 文本颜色
    QColor textColor;
    // 背景颜色
    QColor backgroundColor;
    // 前景颜色
    QColor foregroundColor;
    // 标尺颜色
    QColor scaleColor;
    // 标尺方向
    QString scaleDir;
    // 标尺位置
    QString scalePos;
    // 字体
    QFont font;
    // 显示标尺
    bool showRuler;
    // 显示刻度
    bool showScale;
    // 初始可见
    bool showOnInitial;

private:
    RunTimeTag *m_tag;
};

#endif // QVALUESTICK_H
