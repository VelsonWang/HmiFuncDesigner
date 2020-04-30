#include "GraphPageView.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

bool isColorDark(QColor color)
{
    qreal darkness = 1 - (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) / 255;
    return (darkness >= 0.5);
}

Ruler::Ruler(RulerType type, QWidget* parent)
    : QWidget(parent),
      m_pageItem(Q_NULLPTR),
      m_type(type)
{

}

void Ruler::setPage(QGraphicsObject* page)
{
    m_pageItem = page;
}

void Ruler::setMousePos(QPoint mousePos)
{
    m_mousePos = mousePos;
}

void Ruler::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setBrush(palette().background());
    painter.setPen(Qt::NoPen);
    painter.drawRect(event->rect());
    //    painter.setPen(palette().foreground().color());

    QGraphicsView* view = qobject_cast<QGraphicsView*>(parent());
    qreal rulerWidth = 0;
    qreal rulerHeight = 0;

    // FIXME [宽，高]加入缩放比例
    if (m_pageItem) {
        rulerWidth = m_pageItem->boundingRect().width();
        rulerHeight = m_pageItem->boundingRect().height();
    } else  {
        rulerWidth = view->scene()->sceneRect().width();
        rulerHeight = view->scene()->sceneRect().height();
    }

    int hStartPos = view->mapFromScene(0,0).x();
    int vStartPos = view->mapFromScene(0,0).y();

    QFont font = painter.font();
    font.setPointSize(7);
    painter.setFont(font);

    switch (m_type) {
    case Horizontal:
        painter.setPen(Qt::NoPen);

        if (isColorDark(palette().background().color()))
            painter.setBrush(QColor("#64893d"));
        else
            painter.setBrush(QColor("#b5da91"));

        if (m_pageItem)
            drawItemWithChildren(&painter, m_pageItem);

        painter.setPen(palette().foreground().color());

        for (int i = 0; i < rulerWidth / 10; ++i){
            int hs10 = 0;
            int hs5 = 0;
            // FIXME 加入缩放比例
            if (m_pageItem) {
                hs10 = view->mapFromScene(QPointF(m_pageItem->boundingRect().x() + i * 10, 0)).x();
                hs5 = view->mapFromScene(QPointF(m_pageItem->boundingRect().x() + i * 10 + 5, 0)).x();
            } else {
                hs10 = view->mapFromScene(QPointF(i * 10, 0)).x();
                hs5 = view->mapFromScene(QPointF(i * 10 + 5, 0)).x();
            }

            if (hs10 > 0){
                if (hStartPos > 0){
                    hs10 -= hStartPos;
                    hs5  -= hStartPos;
                }
                painter.drawLine(hs10, 15, hs10, 20);
                painter.drawLine(hs5, 10, hs5, 20);
                if ( i > 0)
                    painter.drawText(QPoint(hs10 - (painter.fontMetrics().width(QString::number(i))/2), 12),
                                     QString::number(i));
            }
        }
        painter.setPen(palette().foreground().color());
        painter.drawLine(m_mousePos.x() - (hStartPos > 0 ? hStartPos : 0) , 0,
                         m_mousePos.x() - (hStartPos > 0 ? hStartPos : 0) , 20);
        break;
    case Vertical:
        painter.setPen(Qt::NoPen);

        if (isColorDark(palette().background().color()))
            painter.setBrush(QColor("#64893d"));
        else
            painter.setBrush(QColor("#b5da91"));

        if (m_pageItem)
            drawItemWithChildren(&painter, m_pageItem);

        painter.setPen(palette().foreground().color());
        for (int i = 0; i < rulerHeight / 10; ++i){
            int vs10 = 0;
            int vs5  = 0;
            // FIXME 加入缩放比例
            if (m_pageItem) {
                vs10 = view->mapFromScene(QPointF(0, m_pageItem->boundingRect().y() + i * 10)).y();
                vs5  = view->mapFromScene(QPointF(0, m_pageItem->boundingRect().y() + i * 10 + 5)).y();
            } else {
                vs10 = view->mapFromScene(QPointF(0, i * 10)).y();
                vs5  = view->mapFromScene(QPointF(0, i * 10 + 5)).y();
            }
            if (vs10 > 0){
                if (vStartPos > 0){
                    vs10 -= vStartPos;
                    vs5 -= vStartPos;
                }
                painter.drawLine(15, vs10, 20, vs10);
                if ( i > 0 )
                    painter.drawText(QPoint( (15 - painter.fontMetrics().width(QString::number(i))) / 2 ,
                                             vs10 + (painter.fontMetrics().height()/2)), QString::number(i));
                painter.drawLine(10, vs5, 20, vs5);
            }
        }
        painter.setPen(palette().foreground().color());
        painter.drawLine(0, m_mousePos.y() - (vStartPos > 0 ? vStartPos : 0),
                         20, m_mousePos.y() - (vStartPos > 0 ? vStartPos : 0));
        break;
    }

}

void Ruler::drawItemWithChildren(QPainter *painter, QGraphicsObject* item)
{
    if(!item) return;

    foreach(QGraphicsItem* child, item->childItems()){
        if (!child->childItems().isEmpty())
            drawItemWithChildren(painter, dynamic_cast<QGraphicsObject*>(child));
        else drawItem(painter, dynamic_cast<QGraphicsObject*>(child));

    }
    drawItem(painter, item);
}

void Ruler::drawItem(QPainter *painter, QGraphicsObject* item)
{
    if (!item->isSelected()) return;

    QGraphicsView* view = qobject_cast<QGraphicsView*>(parent());
    int hStartPos = view->mapFromScene(0,0).x();
    int vStartPos = view->mapFromScene(0,0).y();

    int itemWidth = view->mapFromScene(item->mapToScene(m_pageItem->boundingRect().width(), 0).x() - item->mapToScene(0, 0).x(), 0).x() - hStartPos;
    int itemHeight = view->mapFromScene(0, item->mapToScene(0, m_pageItem->boundingRect().height()).y() - item->mapToScene(0, 0).y()).y() - vStartPos;

    switch (m_type) {
    case Horizontal:
        if (item->isSelected())
         painter->drawRect(view->mapFromScene(item->mapToScene(0,0)).x() - (hStartPos > 0 ? hStartPos : 0)  , 0,
                          itemWidth, 20);
        break;
    case Vertical:
        if (item->isSelected())
            painter->drawRect(0, view->mapFromScene(item->mapToScene(0, 0)).y() - (vStartPos > 0 ? vStartPos : 0),
                             20, itemHeight);
        break;
    }
}


//------------------------------------------------------------------------------

GraphPageView::GraphPageView(QWidget *parent)
    : QGraphicsView(parent),
      m_horizontalRuller(Q_NULLPTR),
      m_verticalRuller(Q_NULLPTR)
{
    this->setViewportMargins(20, 20, 0, 0);
    this->setBackgroundBrush(QBrush(Qt::lightGray));
    this->setFrameShape(QFrame::NoFrame);
    //this->centerOn(0, 0);
    setPageItem(Q_NULLPTR);
}

GraphPageView::GraphPageView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent),
      m_horizontalRuller(Q_NULLPTR),
      m_verticalRuller(Q_NULLPTR)
{
    this->setViewportMargins(20, 20, 0, 0);
    this->setBackgroundBrush(QBrush(Qt::lightGray));
    this->setFrameShape(QFrame::NoFrame);
    //this->centerOn(0, 0);
    setPageItem(Q_NULLPTR);
}

void GraphPageView::setPageItem(QGraphicsObject* pageItem)
{
    m_pageItem = pageItem;
    if (!m_horizontalRuller){
        m_horizontalRuller = new Ruler(Ruler::Horizontal, this);
        m_horizontalRuller->setPage(pageItem);
    }
    if (!m_verticalRuller){
        m_verticalRuller = new Ruler(Ruler::Vertical, this);
        m_verticalRuller->setPage(pageItem);
    }
}

bool GraphPageView::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseMove:
        if (m_horizontalRuller && m_verticalRuller){
            m_horizontalRuller->setMousePos(dynamic_cast<QMouseEvent*>(event)->pos());
            m_verticalRuller->setMousePos(dynamic_cast<QMouseEvent*>(event)->pos());
            m_horizontalRuller->update();
            m_verticalRuller->update();
        }
        break;
    //case QEvent::Resize:
    case QEvent::Paint:
        if (m_horizontalRuller && m_verticalRuller){
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;
            if(m_pageItem) {
                x = mapFromScene(m_pageItem->boundingRect().x(),m_pageItem->boundingRect().y()).x();
                y = mapFromScene(m_pageItem->boundingRect().x(),m_pageItem->boundingRect().y()).y();
                width = mapFromScene(m_pageItem->boundingRect().bottomRight().x(),m_pageItem->boundingRect().bottomRight().y()).x();
                height = mapFromScene(m_pageItem->boundingRect().bottomRight().x(),m_pageItem->boundingRect().bottomRight().y()).y();
                x = x < 0 ? 0 : x;
                y = y < 0 ? 0 : y;
            } else {
                width = static_cast<int>(scene()->sceneRect().width());
                height = static_cast<int>(scene()->sceneRect().height());
            }
            m_horizontalRuller->setGeometry(x + this->viewportMargins().left(), 0, (width - x), this->viewportMargins().top());
            m_verticalRuller->setGeometry(0, y + this->viewportMargins().top(), this->viewportMargins().left(), (height - y));
            m_verticalRuller->update();
            m_horizontalRuller->update();
        }
        break;
    default:
        break;
    }

    return QGraphicsView::viewportEvent(event);
}

void GraphPageView::setFixedSizeEx(int w, int h)
{
    this->setFixedSize(w + this->viewportMargins().left(), h + this->viewportMargins().top());
}

