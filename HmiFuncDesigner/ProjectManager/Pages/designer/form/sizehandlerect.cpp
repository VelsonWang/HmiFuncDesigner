#include "sizehandlerect.h"
#include <QPainter>
#include <QMouseEvent>

SizeHandleRect::SizeHandleRect(QWidget *parent, Direction d, QWidget *resizable) :
    QWidget(parent),
    m_dir(d),
    m_resizable(resizable)
{
    setBackgroundRole(QPalette::Text);
    setAutoFillBackground(true);
    setFixedSize(6, 6);
    setMouseTracking(false);
    updateCursor();
}

void SizeHandleRect::updateCursor()
{
    switch(m_dir)
    {
    case Right:
    case RightTop:
        setCursor(Qt::SizeHorCursor);
        return;
    case RightBottom:
        setCursor(Qt::SizeFDiagCursor);
        return;
    case LeftBottom:
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        return;
    default:
        setCursor(Qt::ArrowCursor);
    }
    setCursor(Qt::ArrowCursor);
}

void SizeHandleRect::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::blue);
    p.drawRect(0, 0, width()-1, height()-1);
}

void SizeHandleRect::mousePressEvent(QMouseEvent *e)
{
    e->accept();

    if(e->button() != Qt::LeftButton) {
        return;
    }

    m_startSize = m_curSize = m_resizable->size();
    m_startPos = m_curPos = m_resizable->mapFromGlobal(e->globalPos());
    m_resizable->setFocus();
}

void SizeHandleRect::mouseMoveEvent(QMouseEvent *e)
{

    m_curPos = m_resizable->mapFromGlobal(e->globalPos());
    QSize delta = QSize(m_curPos.x() - m_startPos.x(), m_curPos.y() - m_startPos.y());

    switch(m_dir) {
    case Right:
    case RightTop:
        delta.setHeight(0);
        break;
    case RightBottom:
        break;
    case LeftBottom:
    case Bottom:
        delta.setWidth(0);
        break;
    default:
        delta = QSize(0, 0);
    }

    if(delta != QSize(0, 0)) {
        tryResize(delta);
    }
}

void SizeHandleRect::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton) {
        return;
    }
    e->accept();
    if(m_startSize != m_curSize) {
        const QRect startRect = QRect(0, 0, m_startSize.width(), m_startSize.height());
        const QRect newRect = QRect(0, 0, m_curSize.width(), m_curSize.height());
        emit mouseButtonReleased(startRect.adjusted(0, 0, -20, -20), newRect.adjusted(0, 0, -20, -20));
    }
}

void SizeHandleRect::tryResize(const QSize &delta)
{
    QSize newSize = m_startSize + delta;
    newSize = newSize.expandedTo(m_resizable->minimumSizeHint());
    newSize = newSize.expandedTo(m_resizable->minimumSize());
    newSize = newSize.boundedTo(m_resizable->maximumSize());
    if(newSize == m_resizable->size()) {
        return;
    }

    m_resizable->resize(newSize);
    m_curSize = m_resizable->size();
}
