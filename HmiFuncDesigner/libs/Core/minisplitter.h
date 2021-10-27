#ifndef MINISPLITTER_H
#define MINISPLITTER_H

#include "corelibglobal.h"
#include <QSplitter>

/*
 *这个类是分割窗口，主要是实现了宽度为1的分割线
 */

class CORELIB_EXPORT MiniSplitter : public QSplitter
{
public:
    MiniSplitter(QWidget *parent = NULL);
    MiniSplitter(Qt::Orientation orientation);

protected:
    QSplitterHandle *createHandle(); // 创建分割线

protected:
};

#endif // MINISPLITTER_H
