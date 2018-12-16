#ifndef DRAWLISTUTILS_H
#define DRAWLISTUTILS_H

#include <QString>
#include <QStringList>

class DrawListUtils
{

public:
    DrawListUtils();

public:
    /*
    * 加载画面名称列表
    */
    static void LoadDrawList(QString path, QStringList &slist);

    /*
    * 保存画面名称列表
    */
    static void SaveDrawList(QString path, QStringList &slist);

    /*
    * 获取画面名最大数值
    */
    static int GetMaxDrawPageNum(QString name, QStringList &slist);

};

#endif // DRAWLISTUTILS_H
