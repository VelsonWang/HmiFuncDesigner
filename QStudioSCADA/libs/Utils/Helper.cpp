
#include <QString>
#include <QSettings>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include <QDesktopWidget>
#include <QRect>

#include "Helper.h"


Helper::Helper()
{

}

/*
* 取得程序所在目录
*/
QString Helper::AppDir()
{
    return QCoreApplication::applicationDirPath();
}


/*
* 拷贝目录
*/
bool Helper::CopyDir(const QString &source, const QString &destination, bool override)
{
    QDir directory(source);
    if (!directory.exists())
    {
        //qDebug()<< source << " not exists.";
        return false;
    }

    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();

    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!CopyDir(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }

    return !error;
}


/*
* 拷贝文件或目录
*/
bool Helper::CopyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!CopyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}


/*
* 删除目录
*/
bool Helper::DeleteDir(const QString &dirName)
{
    QDir directory(dirName);
    if (!directory.exists())
    {
        return true;
    }

    QString srcPath = QDir::toNativeSeparators(dirName);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();

    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    bool error = false;
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString filePath = srcPath + fileNames.at(i);
        QFileInfo fileInfo(filePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            QFile::setPermissions(filePath, QFile::WriteOwner);
            if (!QFile::remove(filePath))
            {
                //qDebug() << "remove file" << filePath << " faild!";
                error = true;
            }
        }
        else if (fileInfo.isDir())
        {
            if (!DeleteDir(filePath))
            {
                error = true;
            }
        }
    }

    if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))
    {
        //qDebug() << "remove dir" << directory.path() << " faild!";
        error = true;
    }

    return !error;
}

/*
* 桌面居中窗口
*/
void Helper::WidgetMoveCenter(QWidget *w)
{
    QDesktopWidget * pDesktopWidget = QApplication::desktop();
    QRect rect = pDesktopWidget->screenGeometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();

    int wWidth = w->geometry().width();
    int wHeight = w->geometry().height();
    w->move((screenWidth-wWidth)/2,(screenHeight-wHeight)/2);
}

QString Helper::GBK2UTF8(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8
    return g2u;
}

QString Helper::UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
    return utf2gbk;
}

std::string Helper::gbk2utf8(const QString &inStr)
{
    return GBK2UTF8(inStr).toStdString();
}

QString Helper::utf82gbk(const std::string &inStr)
{
    QString str = QString::fromStdString(inStr);

    return UTF82GBK(str);
}

/*
* 写指定编码的string到文件
*/
void Helper::writeString(const QString& filePath, QString str, QTextCodec* codec)
{
    QFile writeFile(filePath);
    if (!writeFile.open(QIODevice::WriteOnly))
        return;
    QTextStream out(&writeFile);
    out.setCodec(codec);
    out << str;
    writeFile.close();
}


/*
* 从文件读指定编码的string
*/
QString Helper::readString(const QString& filePath, QTextCodec* codec)
{
    QString ret = "";
    QFile readFile(filePath);
    if (!readFile.open(QIODevice::ReadOnly))
        return ret;
    QTextStream in(&readFile);
    in.setCodec(codec);
    ret = in.readAll();
    readFile.close();
    return ret;
}

