#ifndef QTOOLBARBUTTON_H
#define QTOOLBARBUTTON_H

#include "corelibglobal.h"

#include <QToolButton>
#include <QAction>

/*
 *这个类定义了工具条上的按钮
 */

class CORELIB_EXPORT QToolBarButton: public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(float fader READ fader WRITE setFader)//鼠标移动上的时候阴影透明度
public:
    QToolBarButton(QAction *action, QWidget *parent = 0);

    void    paintEvent(QPaintEvent *event);//绘制
    bool    event(QEvent *e);//事件处理
    QSize   sizeHint() const;//控件大小
    QSize   minimumSizeHint() const;//控件最小大小


    float   fader();//透明因素
    void    setFader(float value);//设置透明因素

private slots:
    void    actionChanged();

private:
    float       m_fader;//透明因素
};

#endif // QTOOLBARBUTTON_H
