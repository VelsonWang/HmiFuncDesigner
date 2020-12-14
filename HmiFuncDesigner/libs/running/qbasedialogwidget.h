#ifndef QBASEDIALOGWIDGET_H
#define QBASEDIALOGWIDGET_H

#include <QWidget>

class QBaseDialogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QBaseDialogWidget(QWidget *parent = 0);

    void    addWidget(QWidget* wid);
    QWidget* getWidget();
protected:
    void    paintEvent(QPaintEvent *);
signals:

public slots:
protected:
    QWidget *m_widget;
};

#endif // QBASEDIALOGWIDGET_H
