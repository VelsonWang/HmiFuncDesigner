#ifndef KEYBOARDNUM_H
#define KEYBOARDNUM_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QTimer>
#include <QMutexLocker>
#include <QMutex>
#include <stdlib.h>

namespace Ui
{
    class KeyBoardNumber;
}

class KeyBoardNumber : public QWidget
{
    Q_OBJECT

public:
    explicit KeyBoardNumber(QWidget *parent = 0);
    ~KeyBoardNumber();

    static KeyBoardNumber *instance() {
        static KeyBoardNumber instance;
        return &instance;
    }

    void installInputMethod();
    void unInstallInputMethod();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void focusChanged(QWidget *oldWidget, QWidget *nowWidget);
    void btnClicked();
    void reClicked();

private:
    void initForm();                //初始化窗体数据
    void initProperty();            //初始化属性
    void insertValue(QString value);//插入值到当前焦点控件
    void deleteValue();             //删除当前焦点控件的一个字符
    bool checkPress();              //校验当前长按的按钮

private:
    Ui::KeyBoardNumber *ui;
    bool isPress;                   //是否长按退格键
    QPushButton *btnPress;          //长按按钮
    QTimer *timerPress;             //退格键定时器
    bool isFirst;                   //是否首次加载
    QWidget *currentWidget;         //当前焦点的对象
    QLineEdit *currentLineEdit;     //当前焦点的单行文本框
    QTextEdit *currentTextEdit;     //当前焦点的多行文本框
    QPlainTextEdit *currentPlain;   //当前焦点的富文本框
    QTextBrowser *currentBrowser;   //当前焦点的文本浏览框
    QString currentEditType;        //当前焦点控件的类型
};

#endif
