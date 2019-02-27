#ifndef ELEMENTSIMPLELISTWIDGET_H
#define ELEMENTSIMPLELISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>

class ElementSimpleListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ElementSimpleListWidget(QListWidget *parent = 0);
    
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void startDrag();
    void addElements();

    QPoint startPos;
    
};

#endif // ELEMENTSIMPLELISTWIDGET_H
