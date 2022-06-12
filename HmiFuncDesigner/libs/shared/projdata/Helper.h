#ifndef HELPER_H
#define HELPER_H

#include <QTextCodec>
#include <QString>
#include <QWidget>

class Helper
{
private:
    Helper();

public:
    // 取得程序所在目录
    static QString AppDir();
    // 拷贝目录
    static bool CopyDir(const QString &source, const QString &destination, bool override);
    // 拷贝目录
    static bool CopyRecursively(const QString &srcFilePath, const QString &tgtFilePath);
    // 删除目录
    static bool DeleteDir(const QString &dirName);
    // 桌面居中窗口
    static void WidgetMoveCenter(QWidget *w);

    static QString GBK2UTF8(const QString &inStr);
    static QString UTF82GBK(const QString &inStr);
    static std::string gbk2utf8(const QString &inStr);
    static QString utf82gbk(const std::string &inStr);

    // 写指定编码的string到文件
    static void writeString(const QString& filePath, QString str, QTextCodec* codec = QTextCodec::codecForName("UTF-8"));
    // 从文件读指定编码的string
    static QString readString(const QString& filePath, QTextCodec* codec = QTextCodec::codecForName("UTF-8"));
};


#endif // HELPER_H
