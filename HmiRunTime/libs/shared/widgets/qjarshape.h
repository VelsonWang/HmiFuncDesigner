#ifndef QJARSHAPE_H
#define QJARSHAPE_H

#include <QWidget>
#include <QPaintEvent>

class QJarShape : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)
    Q_PROPERTY(QString jarShape READ getJarShape WRITE setJarShape)
    Q_PROPERTY(QFont font READ getFont WRITE setFont)
    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)
    Q_PROPERTY(QColor lessThanLowerLimitColor READ getLessThanLowerLimitColor WRITE setLessThanLowerLimitColor)
    Q_PROPERTY(QColor normalColor READ getNormalColor WRITE setNormalColor)
    Q_PROPERTY(QColor greaterThanUpperLimitColor READ getGreaterThanUpperLimitColor WRITE setGreaterThanUpperLimitColor)
    Q_PROPERTY(double upperLimitValue READ getUpperLimitValue WRITE setUpperLimitValue)
    Q_PROPERTY(double lowerLimitValue READ getLowerLimitValue WRITE setLowerLimitValue)
    Q_PROPERTY(double maxValue READ getMaxValue WRITE setMaxValue)
    Q_PROPERTY(int scaleNum READ getScaleNum WRITE setScaleNum)
    Q_PROPERTY(bool showOnInitial READ getShowOnInitial WRITE setShowOnInitial)

public:
    explicit QJarShape(QWidget *parent = nullptr);

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

    QString getJarShape() const;
    void setJarShape(const QString &value);

    QFont getFont() const;
    void setFont(const QFont &value);

    QColor getBorderColor() const;
    void setBorderColor(const QColor &value);

    QColor getLessThanLowerLimitColor() const;
    void setLessThanLowerLimitColor(const QColor &value);

    QColor getNormalColor() const;
    void setNormalColor(const QColor &value);

    QColor getGreaterThanUpperLimitColor() const;
    void setGreaterThanUpperLimitColor(const QColor &value);

    double getUpperLimitValue() const;
    void setUpperLimitValue(double value);

    double getLowerLimitValue() const;
    void setLowerLimitValue(double value);

    double getMaxValue() const;
    void setMaxValue(double value);

    int getScaleNum() const;
    void setScaleNum(int value);

    bool getShowOnInitial() const;
    void setShowOnInitial(bool value);

private:
    void setPropertyInner();
    void drawJarShape(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    // 关联的变量
    QString tagSelected;
    // 罐形容器标题
    QString jarShape;
    // 字体
    QFont font;
    // 罐体颜色
    QColor borderColor;
    // 低于下限颜色
    QColor lessThanLowerLimitColor;
    // 正常液面颜色
    QColor normalColor;
    // 高于上限颜色
    QColor greaterThanUpperLimitColor;
    // 上限值
    double upperLimitValue;
    // 下限值
    double lowerLimitValue;
    // 满量程值-刻度最大值。
    double maxValue;
    // 刻度个数
    int scaleNum;
    // 初始可见
    bool showOnInitial;
};

#endif // QJARSHAPE_H
