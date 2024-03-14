#ifndef FORMRESIZER_H
#define FORMRESIZER_H

#include <QWidget>

class SizeHandleRect;
class QFrame;

class FormResizer : public QWidget
{
    Q_OBJECT
public:
    explicit FormResizer(QWidget *parent = 0);
    ~FormResizer();
    void updateGeometry();
    void updateFormGeometry();
    void update();
    void setWidget(QWidget* wid);

    void showHandle();
    void hideHandle();

    bool isHandleVisible();

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void size_changed(const QRect &,const QRect &);

protected:
    SizeHandleRect  *m_handles[8];
    QWidget *m_widget;
    QFrame *m_frame;
    bool m_handleVisible;
};

#endif // FORMRESIZER_H
