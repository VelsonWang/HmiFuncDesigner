#include "PubTool.h"

PubTool::PubTool()
{

}

/*
* 拉升四边形
*/
void PubTool::SpreadRect(QRect *rc, int cx, int cy)
{
    rc->setLeft(rc->left() - cx);
    rc->setRight(rc->right() + cx);
    rc->setTop(rc->top() - cy);
    rc->setBottom(rc->bottom() + cy);
}

/*
* 移动四边形
*/
void PubTool::FloatRect(QRect *rc, int cx, int cy)
{
    rc->setLeft(rc->left() + cx);
    rc->setRight(rc->right() + cx);
    rc->setTop(rc->top() + cy);
    rc->setBottom(rc->bottom() + cy);
}

/*
* 设置四边型的值
*/
void PubTool::SetRect(QRect *rc, int left, int top, int right, int bottom)
{
    rc->setLeft(left);
    rc->setRight(right);
    rc->setTop(top);
    rc->setBottom(bottom);
}



void PubTool::Draw3DLowerRect(QPainter *painter, QRect rct)
{
    QPen pen;
    pen.setColor(Qt::lightGray);
    painter->setPen(pen);

    painter->drawLine(rct.left(), rct.bottom(), rct.left(), rct.top());
    painter->drawLine(rct.left(), rct.top(), rct.right(), rct.top());

    pen.setColor(QColor(255, 255, 255));
    painter->setPen(pen);

    painter->drawLine(rct.right(), rct.top(), rct.right(), rct.bottom());
    painter->drawLine(rct.right(), rct.bottom(), rct.left(), rct.bottom());
}

void PubTool::Draw3DRaiseRect(QPainter *painter, QRect rct)
{
    QPen pen;
    pen.setColor(QColor(255, 255, 255));
    painter->setPen(pen);

    painter->drawLine(rct.left(), rct.bottom(), rct.left(), rct.top());
    painter->drawLine(rct.left(), rct.top(), rct.right(), rct.top());

    pen.setColor(Qt::lightGray);
    painter->setPen(pen);

    painter->drawLine(rct.right(), rct.top(), rct.right(), rct.bottom());
    painter->drawLine(rct.right(), rct.bottom(), rct.left(), rct.bottom());
}

void PubTool::DrawFrameRect(QPainter *painter, int left, int top, int right, int bottom, QColor cl)
{
    QRect rct(left, top, right, bottom);
    DrawFrameRect(painter, rct, cl);
}

void PubTool::DrawFrameRect(QPainter *painter, QRect rct, QColor cl)
{
    QPen pen;
    pen.setColor(cl);
    painter->setPen(pen);

    painter->drawLine(rct.left(), rct.bottom(), rct.left(), rct.top());
    painter->drawLine(rct.left(), rct.top(), rct.right(), rct.top());
    painter->drawLine(rct.right(), rct.top(), rct.right(), rct.bottom());
    painter->drawLine(rct.right(), rct.bottom(), rct.left(), rct.bottom());
}

void PubTool::FillFullRect(QPainter *painter, QRect rct, QColor cl)
{
    QBrush brush(cl);
    rct.setRight(rct.right() + 1);
    rct.setBottom(rct.bottom() + 1) ;
    painter->fillRect(rct, brush);
}


void PubTool::DrawLineInRect(QPainter *painter, int sx, int sy, int dx, int dy, QColor cl, QRect rct)
{
    //只能处理垂直线和水平线
    if((sx != dx) && (sy != dy))
        return;

    QPen pen;
    pen.setColor(cl);
    painter->setPen(pen);

    QPoint tempPt;

    if(IsInRect(rct, sx, sy, true) && IsInRect(rct, dx, dy, true))
    {
        painter->drawLine(sx, sy, dx, dy);
    }
    else if(IsInRect(rct, sx, sy, true))
    {
        tempPt = GetCrossOfOuterPt(dx, dy, rct);
        painter->drawLine(tempPt.x(), tempPt.y(), sx, sy);
    }
    else if(IsInRect(rct, dx, dy, true))
    {
        tempPt = GetCrossOfOuterPt(sx, sy, rct);
        painter->drawLine(tempPt.x(), tempPt.y(), dx, dy);
    }
    else
    {
        bool bCrossed = false;

        if((sx >= rct.left()) && (sx <= rct.right()))
        {
            if(((sy < rct.top()) && (dy > rct.bottom())) || ((dy < rct.top()) && (sy > rct.bottom())))
                bCrossed = true;
        }

        if((sy >= rct.top()) && (sy <= rct.bottom()))
        {
            if(((sx < rct.left()) && (dx > rct.right())) || ((dx < rct.left()) && (sx > rct.right())))
                bCrossed = true;
        }

        if(bCrossed)
        {
            tempPt = GetCrossOfOuterPt(sx, sy, rct);
            QPoint dPt = GetCrossOfOuterPt(dx, dy, rct);

            painter->drawLine(tempPt.x(), tempPt.y(), dPt.x(), dPt.y());
        }
    }
}


void PubTool::DrawFrameRectInRect(QPainter *painter, QRect rct, QColor cl, QRect pRct)
{
    DrawLineInRect(painter, rct.left(), rct.bottom(), rct.left(), rct.top(), cl, pRct);
    DrawLineInRect(painter, rct.left(), rct.top(), rct.right(), rct.top(), cl, pRct);
    DrawLineInRect(painter, rct.right(), rct.top(), rct.right(), rct.bottom(), cl, pRct);
    DrawLineInRect(painter, rct.right(), rct.bottom(), rct.left(), rct.bottom(), cl, pRct);
}

void PubTool::DrawLine(QPainter *painter, QPoint ptStart, QPoint ptEnd, QColor cl)
{
    QPen pen;
    pen.setColor(cl);
    painter->setPen(pen);
    painter->drawLine(ptStart.x(), ptStart.y(), ptEnd.x(), ptEnd.y());
}

void PubTool::DrawGrid(QPainter *painter,int w_grid,int h_grid, QRect rcSize)
{
    QPen pen;

    // 是否需要xor模式 ??
    QColor   OldPenColor;

    int iWGridCount,iHGridCount;

    OldPenColor = painter->pen().color();

    pen.setColor(OldPenColor);
    painter->setPen(QColor(64, 64, 64));

    iWGridCount = (rcSize.right() - rcSize.left())/ w_grid + 1;
    iHGridCount = (rcSize.bottom() - rcSize.top())/ h_grid + 1;

    for(int i=1; i<iHGridCount; i++)
    {
        for(int j=1; j<iWGridCount; j++)
        {
            painter->drawLine(j*w_grid, i*h_grid, j*w_grid+1, i*h_grid);
        }
    }

    pen.setColor(OldPenColor);
    painter->setPen(pen);
}


///////////////////////////////////////////////
//获取四边型外边结点和其边内侧的交点
QPoint PubTool::GetCrossOfOuterPt(int x, int y, QRect aRct)
{
    QPoint pt;

    if((x >= aRct.left()) && (x <= aRct.right()) && ((y <= aRct.top()) || (y >= aRct.bottom())))
    {
        pt.setX(x);

        if(y <= aRct.top())
            pt.setY(aRct.top());
        else
            pt.setY(aRct.bottom());
    }

    if((y >= aRct.top()) && (y <= aRct.bottom()) && ((x <= aRct.left()) || (x >= aRct.right())))
    {
        pt.setY(y);

        if(x <= aRct.left())
            pt.setX(aRct.left());
        else
            pt.setX(aRct.right());
    }

    return pt;
}


bool PubTool::IsInRect(QRect rct, QPoint pt, bool bBorder)
{
    return  IsInRect(rct, pt.x(), pt.y(), bBorder);
}

bool PubTool::IsInRect(QRect rct, int X, int Y, bool bBorder)
{
    bool bRet = false;

    if(bBorder)
        bRet = ((X >= rct.left()) && (X <= rct.right())
            && (Y >= rct.top()) && (Y <= rct.bottom()));
    else
        bRet = ((X > rct.left()) && (X < rct.right())
            && (Y > rct.top()) && (Y < rct.bottom()));

    return bRet;
}

bool PubTool::IsInRect(QRect rct, QRect subRct, bool bBorder)
{
    bool bRet = false;

    if(bBorder)
        bRet = ((subRct.left() >= rct.left()) && (subRct.right() <= rct.right())
            && (subRct.top() >= rct.top()) && (subRct.bottom() <= rct.bottom()));
    else
        bRet = ((subRct.left() > rct.left()) && (subRct.right() < rct.right())
            && (subRct.top() > rct.top()) && (subRct.bottom() < rct.bottom()));

    return bRet;
}

bool PubTool::IsCrossRect(QRect aRct, QRect bRct, bool bBorder)
{
    //////////////////////////////////////////////////
    //   *****************
    //   *               *
    //   *          *****************
    //   *          *    *          *
    //   *****************          *
    //			    *               *
    //              *****************
    //////////////////////////////////////////////////
    if(IsInRect(bRct, aRct.left(), aRct.top(), bBorder))
        return true;

    if(IsInRect(bRct, aRct.right(), aRct.top(), bBorder))
        return true;

    if(IsInRect(bRct, aRct.right(), aRct.bottom(), bBorder))
        return true;

    if(IsInRect(bRct, aRct.left(), aRct.bottom(), bBorder))
        return true;

    //////////////////////////////////////////////////
    if(IsInRect(aRct, bRct.left(), bRct.top(), bBorder))
        return true;

    if(IsInRect(aRct, bRct.right(), bRct.top(), bBorder))
        return true;

    if(IsInRect(aRct, bRct.right(), bRct.bottom(), bBorder))
        return true;

    if(IsInRect(aRct, bRct.left(), bRct.bottom(), bBorder))
        return true;
    //////////////////////////////////////////////////


    //////////////////////////////////////////////////
    //			*************
    //			*			*
    //	*****************************
    //	*		*			*		*
    //	*		*			*		*
    //	*****************************
    //			*			*
    //			*************
    //////////////////////////////////////////////////
    if(!bBorder)
    {
        if((aRct.left() > bRct.left()) && (aRct.right() < bRct.right())
                && (aRct.top() < bRct.top()) && (aRct.bottom() > bRct.bottom()))
            return true;

        if((aRct.left() < bRct.left()) && (aRct.right() > bRct.right())
                && (aRct.top() > bRct.top()) && (aRct.bottom() < bRct.bottom()))
            return true;
    }
    else
    {
        if((aRct.left() >= bRct.left()) && (aRct.right() <= bRct.right())
                && (aRct.top() <= bRct.top()) && (aRct.bottom() >= bRct.bottom()))
            return true;

        if((aRct.left() <= bRct.left()) && (aRct.right() >= bRct.right())
                && (aRct.top() >= bRct.top()) && (aRct.bottom() <= bRct.bottom()))
            return true;
    }
    //////////////////////////////////////////////////

    return false;
}




