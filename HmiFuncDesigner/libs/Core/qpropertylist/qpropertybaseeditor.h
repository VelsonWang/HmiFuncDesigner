#ifndef QRESETWIDGET_H
#define QRESETWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QVariant>

class QAbstractProperty;
class QUndoStack;

class QPropertyBaseEditor : public QWidget
{
    Q_OBJECT
public:
    explicit QPropertyBaseEditor(QAbstractProperty *pro,QUndoStack* stack,QWidget *parent = 0);
protected:
    void    paintEvent(QPaintEvent *);
    bool    eventFilter(QObject *, QEvent *);
    signals:

public slots:
    void    reset();
protected slots:
    void    property_refresh();
protected:
    QToolButton                     *m_resetButton;
    QAbstractProperty               *m_property;
    QWidget*                        m_widget;
};

#endif // QRESETWIDGET_H
