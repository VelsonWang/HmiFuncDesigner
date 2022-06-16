#ifndef QBLINKWIDGETTIMER_H
#define QBLINKWIDGETTIMER_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QWidget>

class QBlinkWidgetTimer : public QObject
{
    Q_OBJECT
public:
    explicit QBlinkWidgetTimer(int interval = 500, QObject *parent = nullptr);
    ~QBlinkWidgetTimer();

    void addWidget(QWidget* pObj);
    void removeWidget(QWidget* pObj);

private slots:
    void timeout();

private:
    QTimer m_timer;
    int m_interval;
    QVector<QWidget*> m_vectWidgets;
};

#endif // QBLINKWIDGETTIMER_H
