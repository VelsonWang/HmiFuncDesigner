#ifndef DRAWLISTUTILS_H
#define DRAWLISTUTILS_H

#include <QString>
#include <QStringList>

class DrawListUtils
{

public:
    DrawListUtils();

public:
    // 加载画面名称列表
    static void loadDrawList(const QString &path);

    // 保存画面名称列表
    static void saveDrawList(const QString &path);

    // 获取画面名最大数值
    static int getMaxDrawPageNum(const QString &name);

    // 设置工程路径
    static void setProjectPath(const QString &path);

    // 取得工程路径
    static QString getProjectPath();

    // 画面列表
    static QStringList drawList_;

    // 工程路径
    static QString szProjectPath_;
};

#endif // DRAWLISTUTILS_H
