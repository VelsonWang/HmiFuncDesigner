#ifndef GRAPHPAGEVIEW_H
#define GRAPHPAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsObject>


class Ruler: public QWidget
{
public:
    enum RulerType {
        Horizontal,
        Vertical
    };

    Ruler(RulerType type, QWidget* parent = Q_NULLPTR);
    void setPage(QGraphicsObject* page);
    void setMousePos(QPoint mousePos);

protected:
    void paintEvent(QPaintEvent* event);
    void drawItemWithChildren(QPainter *painter, QGraphicsObject* item);
    void drawItem(QPainter *painter, QGraphicsObject* item);

private:
    QGraphicsObject* m_pageItem;
    RulerType m_type;
    QPoint m_mousePos;
};


class GraphPageView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphPageView(QWidget *parent = Q_NULLPTR);
    GraphPageView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);

    void setPageItem(QGraphicsObject* pageItem);
    void setFixedSizeEx(int w, int h);

protected:
    bool viewportEvent(QEvent *event);
private:
    QGraphicsObject* m_pageItem;
    Ruler* m_horizontalRuller;
    Ruler* m_verticalRuller;
};

#endif
