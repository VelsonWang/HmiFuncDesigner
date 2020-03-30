#ifndef INPUTMETHODALPHABET_H
#define INPUTMETHODALPHABET_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QtSql>
#include <QPushButton>
#include <QTimer>
#include <QMutexLocker>
#include <QMutex>
#include <stdlib.h>

namespace Ui
{
    class InputMethodAlphabet;
}

class InputMethodAlphabet : public QWidget
{
    Q_OBJECT

public:
    explicit InputMethodAlphabet(QWidget *parent = 0);
    ~InputMethodAlphabet();

    static InputMethodAlphabet *instance() {
        QMutexLocker locker(&mutex_);
        if (instance_ == nullptr) {
            instance_ = new InputMethodAlphabet;
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

    void init(QString position,
              QString style,
              int btnFontSize,
              int labFontSize);
    void installInputMethod();
    void unInstallInputMethod();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

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
    bool checkPress();              //校验当前长按的按钮
    void initForm();                //初始化窗体数据
    void initProperty();            //初始化属性
    void changeStyle();             //改变样式
    void changeFont();              //改变字体大小
    void showPanel();               //显示输入法面板
    void insertValue(QString value);//插入值到当前焦点控件
    void deleteValue();             //删除当前焦点控件的一个字符
    void changeType(QString type);  //改变输入法类型
    void changeLetter(bool isUpper);//改变字母大小写
    void selectChinese();           //查询汉字
    void showChinese();             //显示查询到的汉字
    void setChinese(int index);     //设置当前汉字
    void clearChinese();            //清空当前汉字信息

private:
    Ui::InputMethodAlphabet *ui_;
    static InputMethodAlphabet *instance_;
    static QMutex mutex_;

    int deskWidth_;
    int deskHeight_;
    int frmWidth_;
    int frmHeight_;
    QPoint mousePoint_;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed_;              //鼠标是否按下
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
    QString currentPosition_;        //当前输入法面板位置类型
    QString currentStyle_;           //当前输入法面板样式
    int btnFontSize_;                //当前输入法面板按钮字体大小
    int labFontSize_;                //当前输入法面板标签字体大小
    QString currentType_;            //当前输入法类型
    QList<QLabel *>labCh_;           //汉字标签数组
    QStringList allPY_;              //所有拼音链表
    QStringList currentPY_;          //当前拼音链表
    int currentPY_index_;            //当前拼音索引
    int currentPY_count_;            //当前拼音数量
};

#endif // INPUTMETHODALPHABET_H
