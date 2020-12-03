#ifndef SIZEHANDLERECT_H
#define SIZEHANDLERECT_H

#include <QWidget>

class SizeHandleRect : public QWidget
{
    Q_OBJECT
public:
    enum Direction{LeftTop,Top,RightTop,Right,RightBottom,Bottom,LeftBottom,Left};
    explicit SizeHandleRect(QWidget *parent,Direction d,QWidget *resizable);

    Direction dir()const{return m_dir;}

protected:
    void updateCuroser();
    void tryResize(const QSize &delta);
signals:
    void mouseButtonReleased(const QRect &,const QRect &);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);


public slots:
protected:
    const Direction m_dir;
    QPoint m_startPos;
    QPoint m_curPos;
    QSize m_startSize;
    QSize m_curSize;
    QWidget *m_resizable;
};

#endif // SIZEHANDLERECT_H
