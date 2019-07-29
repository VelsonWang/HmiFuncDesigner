#include "QQuickInputLineEdit.h"
#include <QCompleter>
#include <QDebug>

#if 0

usage:

QMap<QString, QString> wordMap;
wordMap["ys"] = "医生";
wordMap["hs"] = "护士";
wordMap["lw"] = "老王";
wordMap["ls"] = "李四";
wordMap["zs"] = "张三";
QQuickInputLineEdit::setQuickInputWordMap(wordMap);

#endif

QQuickInputLineEdit::QQuickInputLineEdit(QWidget *parent)
    : QLineEdit(parent)
{   

}


/**
 * @brief QQuickInputLineEdit::setQuickInputWordMap
 * @details 设置支持快捷输入的字符串集合
 * @param wordMap 支持快捷输入的字符串集合
 */
void QQuickInputLineEdit::setQuickInputWordMap(const QMap<QString, QString> &wordMap)
{
    wordMap_ = wordMap;
    QList<QString> wordList = wordMap.keys();
    QCompleter *completer = new QCompleter(wordList, this);
    completer->setFilterMode(Qt::MatchContains);
    this->setCompleter(completer);
}


void QQuickInputLineEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
    foreach (QString word, wordMap_.keys()) {
        if(this->text() == word)
        {
            this->setText(wordMap_[word]);
            break;
        }
    }
}
