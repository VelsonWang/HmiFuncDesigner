#include "StyleSheetHelper.h"
#include <QFile>
#include <QApplication>

#if 0

Usage:

// 加载QSS样式
//StyleSheetHelper::setStyle(QApplication::applicationDirPath() + "/style.qss");
StyleSheetHelper::setStyle(":/style.qss");

//--------------------------------------------------------------

file style.qss contents:

#DialogLog
{
    background: rgba(32, 80, 96, 100);
}

#topPanel
{
    background-color:
    qlineargradient(spread:reflect, x1:0.5, y1:0, x2:0, y2:0,
    stop:0 rgba(91, 204, 233, 100), stop:1 rgba(32, 80, 96,
    100));
}

#loginForm
{
    background: rgba(0, 0, 0, 80);
    border-radius: 8px;
}

QLabel { color: white; }
QLineEdit { border-radius: 3px; }

QPushButton
{
    color: white;
    background-color: #27a9e3;
    border-width: 0px;
    border-radius: 3px;
}

QPushButton:hover { background-color: #66c011; }

#endif


StyleSheetHelper::StyleSheetHelper(QObject *parent)
    : QObject(parent)
{

}

StyleSheetHelper::~StyleSheetHelper()
{

}

void StyleSheetHelper::setStyle(const QString &style)
{
    QFile qss(style);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}
