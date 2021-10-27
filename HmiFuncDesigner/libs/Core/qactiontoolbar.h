#ifndef QACTIONTOOLBAR_H
#define QACTIONTOOLBAR_H

#include "corelibglobal.h"

#include <QWidget>

#include <QAction>
#include <QList>

class QToolBarButton;

class CORELIB_EXPORT QActionToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit QActionToolBar(Qt::Alignment align = Qt::AlignLeft, QWidget *parent = 0);

    void    addButtonActions(const QList<QAction*>    &actions);//添加按钮
    void    setLabelWidget(QWidget* widget);//添加扩展控件

protected:
    QList<QToolBarButton*>      m_buttons;//按钮

    QWidget                    *m_emptyWidget;//扩展控件载体
    Qt::Alignment               m_align;//按钮排列方位
};

#endif // QACTIONTOOLBAR_H
