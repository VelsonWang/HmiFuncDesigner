#ifndef MANHATTANSTYLE_H
#define MANHATTANSTYLE_H

#include "corelibglobal.h"

#include <QProxyStyle>
#include <QImage>

class ManhattanStylePrivate;

class CORELIB_EXPORT ManhattanStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ManhattanStyle(const QString &baseStyleName);

    ~ManhattanStyle();
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    void    polish(QWidget *widget);
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
protected:
    void    drawButtonSeparator(QPainter *painter,const QRect &rect,bool reverse)const;
protected:
    QImage  m_lineeditImage;
    QImage  m_lineeditImage_disabled;
};

#endif // MANHATTANSTYLE_H
