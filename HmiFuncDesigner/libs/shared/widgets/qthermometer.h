#ifndef QTHERMOMETER_H
#define QTHERMOMETER_H

#include <qwidget.h>
#include <qpainter.h>

class QThermometer : public QWidget
{
    Q_OBJECT
    Q_ENUMS(BorderStyle)
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)
    Q_PROPERTY(double min READ getMinValue WRITE setMinValue)
    Q_PROPERTY(double max READ getMaxValue WRITE setMaxValue)
    Q_PROPERTY(int precision READ getPrecision WRITE setPrecision)
    Q_PROPERTY(int numTicks READ getNumTicks WRITE setNumTicks)
    Q_PROPERTY(double threshold READ getThreshold WRITE setThreshold)
    Q_PROPERTY(BorderStyle borderStyle READ getBorderStyle WRITE setBorderStyle)

public:
    enum BorderStyle {
        Flat,
        Frame,
        Raised,
        Sunken
    };

    QThermometer(QWidget *parent = 0);

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

    double getMinValue() const;
    double getMaxValue() const;
    int getPrecision() const;
    int getNumTicks() const;
    double getThreshold() const;
    BorderStyle getBorderStyle() const;
    QSize sizeHint() const override;
    QSize minimumSizeHint( ) const override;

public slots:
    void setMinValue(double newMinValue);
    void setMaxValue(double newMaxValue) ;
    void setPrecision(int newPrecision);
    void setNumTicks(int newNumTicks) ;
    void setThreshold(double newThreshold);
    void setBorderStyle(BorderStyle newBorderStyle) ;

signals:
    void OutOfRange(double value);
    void ThresholdEvent(double value);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    // 关联的变量
    QString tagSelected;
    double min, max;
    double threshold ;
    int precision;
    int numTicks;
    BorderStyle borderStyle;
    // 默认变量值
    double value;
};

#endif
