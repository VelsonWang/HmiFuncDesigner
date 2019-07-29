#include "PubTool.h"
#include "Public.h"

#include <QList>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QDebug>


PubTool::PubTool()
{

}

/*
* 拉升四边形
*/
void PubTool::SpreadRect(QRect *rc, int cx, int cy)
{
    rc->setLeft(rc->left() - cx);
    rc->setRight(rc->right() + cx);
    rc->setTop(rc->top() - cy);
    rc->setBottom(rc->bottom() + cy);
}

/*
* 移动四边形
*/
void PubTool::FloatRect(QRect *rc, int cx, int cy)
{
    rc->setLeft(rc->left() + cx);
    rc->setRight(rc->right() + cx);
    rc->setTop(rc->top() + cy);
    rc->setBottom(rc->bottom() + cy);
}

/*
* 设置四边型的值
*/
void PubTool::SetRect(QRect *rc, int left, int top, int right, int bottom)
{
    rc->setLeft(left);
    rc->setRight(right);
    rc->setTop(top);
    rc->setBottom(bottom);
}
