#ifndef QDRIVERTABVIEW_H
#define QDRIVERTABVIEW_H

#include <QWidget>
#include <QToolButton>

class QNewPageTable : public QWidget
{
    Q_OBJECT
public:
    explicit QNewPageTable(QWidget *parent = 0);

    void    insert_data(QString data,int index=-1);
    void    remove(QString data);


    int     current_index() const;
    void    set_current_index(int index);

    void    set_stop(bool stop);

signals:
    void    current_index_changed(int index);

protected slots:
    void    button_clicked();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    QSize minimumSizeHint() const;
    void   resizeEvent(QResizeEvent *);

    enum HitArea { HITNOTHING, HITOVERFLOW, HITTAB, HITSUBTAB };
    QPair<QNewPageTable::HitArea, int> convertPosToTab(QPoint pos);

    void    calc();
protected:
    QStringList         m_tabs;
    int                 m_currentIndex;
    int                 m_lastVisibleIndex;
    QVector<int>        m_currentTabIndices;
    QToolButton         *m_button;
    bool                m_stop;
};
#endif // QDRIVERTABVIEW_H
