#ifndef INPUTMETHODNUMBER_H
#define INPUTMETHODNUMBER_H

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
    class InputMethodNumber;
}

class InputMethodNumber : public QWidget
{
    Q_OBJECT

public:
    explicit InputMethodNumber(QWidget *parent = 0);
    ~InputMethodNumber();

    static InputMethodNumber *instance() {
        QMutexLocker locker(&mutex_);
        if (instance_ == nullptr) {
            instance_ = new InputMethodNumber;
            ::atexit(deleteInstance);
        }
        return instance_;
    }


    static void deleteInstance() {
        if(instance_ != nullptr) {
            delete instance_;
            instance_ = nullptr;
        }
    }

    void init(QString style, int fontSize);
    void installInputMethod();
    void unInstallInputMethod();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void focusChanged(QWidget *oldWidget, QWidget *nowWidget);
    void btn_clicked();
    void changeStyle(QString topColor,
                     QString bottomColor,
                     QString borderColor,
                     QString textColor);
    void reClicked();

signals:
    void enterPressed();
    void closePressed();

private:
    void initForm();                //初始化窗体数据
    void initProperty();            //初始化属性
    void changeStyle();             //改变样式
    void insertValue(QString value);//插入值到当前焦点控件
    void deleteValue();             //删除当前焦点控件的一个字符
    bool checkPress();              //校验当前长按的按钮

private:
    Ui::InputMethodNumber *ui_;
    static InputMethodNumber *instance_;
    static QMutex mutex_;

    bool isPress_;                   //是否长按退格键
    QPushButton *btnPress_;          //长按按钮
    QTimer *timerPress_;             //退格键定时器
    bool isFirst_;                   //是否首次加载
    QWidget *currentWidget_;         //当前焦点的对象
    QLineEdit *currentLineEdit_;     //当前焦点的单行文本框
    QTextEdit *currentTextEdit_;     //当前焦点的多行文本框
    QPlainTextEdit *currentPlain_;   //当前焦点的富文本框
    QTextBrowser *currentBrowser_;   //当前焦点的文本浏览框
    QString currentEditType_;        //当前焦点控件的类型
    QString currentStyle_;           //当前输入法面板样式
    int currentFontSize_;            //当前输入法面板字体大小


};

#endif // INPUTMETHODNUMBER_H
