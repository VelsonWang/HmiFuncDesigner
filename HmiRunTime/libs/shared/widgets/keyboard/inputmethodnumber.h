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

    static InputMethodNumber *instance()
    {
        static InputMethodNumber obj;
        return &obj;
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void setFocusWidget(QWidget *pObj);

private slots:
    void btn_clicked();
    void reClicked();

signals:
    void enterPressed();
    void closePressed();

private:
    void initForm();
    void initProperty();
    void insertValue(QString value);
    void deleteValue();
    bool checkPress();

private:
    Ui::InputMethodNumber *ui;
    bool isPress;                   //是否长按退格键
    QPushButton *btnPress;          //长按按钮
    QTimer *timerPress;             //退格键定时器
    QWidget *curWidget;         //当前焦点的对象
    QLineEdit *curLineEdit;     //当前焦点的单行文本框
    QTextEdit *curTextEdit;     //当前焦点的多行文本框
    QPlainTextEdit *curPlain;   //当前焦点的富文本框
    QTextBrowser *curBrowser;   //当前焦点的文本浏览框
    QString curEditType;        //当前焦点控件的类型
};

#endif // INPUTMETHODNUMBER_H
