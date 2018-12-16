#include "PaintArea.h"
#include "PubTool.h"

#include <QDebug>


#define PANEL_WIDTH    (50)
#define PANEL_HEIGHT    (42)

#define GRID_WIDTH    (18)
#define GRID_HEIGHT    (18)

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent)
{
    //画布初始化大小设置为600*400,使用32位颜色
    image = QImage(800, 480, QImage::Format_RGB32);
    //背景颜色白色
    backColor = qRgb(255,255,255);
    image.fill(backColor);
    modified = false;
    penColor = Qt::black;
    brushColor = Qt::black;
    penWidth = 1;
    penStyle = Qt::SolidLine;
    curShape = None;
}


/*
* 绘制页面
*/
void PaintArea::DrawGraphPage(GraphPage &page)
{
    QPainter painter(&this->image);
    image.fill(backColor);

    ////////////////////////////////////////////////
    //栅格显示
    QRect gridRect(0, 0, m_showRect.width(), m_showRect.height());
    PubTool::DrawGrid(&painter, GRID_WIDTH, GRID_HEIGHT, gridRect);

    for (int i = 0; i < page.m_ElementList.size(); ++i)
    {
        StdElement *pStdElement = page.m_ElementList.at(i);
        if(pStdElement)
        {
            IElement *pIElement = pStdElement->GetInterface();
            if(pIElement)
            {
                pIElement->darw(&painter);
            }
        }
    }
    update(); //重绘窗口
}


void PaintArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.drawImage(m_showRect.left(), m_showRect.top(), image);
}


void PaintArea::setImageSize(int width, int height)
{
    QImage newImage(width, height, QImage::Format_ARGB32);
    image = newImage;
    update();
}

QSize PaintArea::getImageSize()
{
    return image.size();
}

void PaintArea::setImageColor(QColor color)
{
    backColor = color.rgb(); // QColor to QRgb
    image.fill(backColor);
    update();
}

void PaintArea::doClear()
{
    image.fill(backColor);
    update();
}

void PaintArea::setPenStyle(Qt::PenStyle style)
{
    penStyle = style;
}

void PaintArea::setPenWidth(int width)
{
    penWidth = width;
}

void PaintArea::setPenColor(QColor color)
{
    penColor = color;
}

void PaintArea::setBrushColor(QColor color)
{
    brushColor = color;
}

void PaintArea::setShape(ShapeType shape)
{
    curShape = shape;
}
