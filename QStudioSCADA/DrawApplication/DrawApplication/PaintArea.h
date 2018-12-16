#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QImage>
#include <QRgb>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include "GraphPage.h"

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0);
    void setImageSize(int width, int height);
    QSize getImageSize();
    void setImageColor(QColor color);
    bool isModified() const{return modified;}
    void doClear();  // 清空

    void setPenStyle(Qt::PenStyle style); //设置画笔风格
    void setPenWidth(int width); //设置画笔宽度
    void setPenColor(QColor color); //设置画笔颜色
    void setBrushColor(QColor color); //设置填充颜色
    enum ShapeType //枚举变量，几个图形的选择
    {
        None,
        Line,
        Rectangle,
        Ellipse
    };
    void setShape(ShapeType shape); //设置要绘制的图形


public:
    // 绘制页面
    void DrawGraphPage(GraphPage &page);

protected:
    void paintEvent(QPaintEvent *);  //重绘事件

private:
    void paint(QImage &theImage);

private:
    QRect m_showRect;
    QImage image;  // 用于在其上绘图
    QRgb backColor;  //存储image的背景颜色
    QPoint lastPoint,endPoint;  //鼠标指针的前后两个坐标
    bool modified;  // 标志画布是否被更改过
    QColor penColor; //画笔颜色
    QColor brushColor; //填充颜色
    int penWidth; //画笔宽度
    Qt::PenStyle penStyle; //画笔风格
    ShapeType curShape; //当前图形

public slots:

};

#endif // PAINTAREA_H
