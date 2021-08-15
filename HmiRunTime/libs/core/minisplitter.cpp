
#include "minisplitter.h"

#include "stylehelper.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSplitterHandle>

class MiniSplitterHandle : public QSplitterHandle//自定义分割线
{
public:
    MiniSplitterHandle(Qt::Orientation orientation, QSplitter *parent)
            : QSplitterHandle(orientation, parent)
    {
        setMask(QRegion(contentsRect()));
        setAttribute(Qt::WA_MouseNoMask, true);
    }
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
};

void MiniSplitterHandle::resizeEvent(QResizeEvent *event)
{
    if (orientation() == Qt::Horizontal)
        setContentsMargins(2, 0, 2, 0);
    else
        setContentsMargins(0, 2, 0, 2);
    setMask(QRegion(contentsRect()));
    QSplitterHandle::resizeEvent(event);
}

void MiniSplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(),StyleHelper::borderColor());//绘制分割线颜色
}

QSplitterHandle *MiniSplitter::createHandle()
{
    return new MiniSplitterHandle(orientation(), this);//创建分割线
}

MiniSplitter::MiniSplitter(QWidget *parent)
    : QSplitter(parent)
{
    setHandleWidth(1);
    setChildrenCollapsible(false);
    setProperty("minisplitter", true);
}

MiniSplitter::MiniSplitter(Qt::Orientation orientation)
    : QSplitter(orientation)
{
    setHandleWidth(1);//设置分割线宽度为1
    setChildrenCollapsible(false);
    setProperty("minisplitter", true);
}
