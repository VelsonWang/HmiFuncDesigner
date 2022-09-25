#include "keyboardnumber.h"
#include "ui_keyboardnumber.h"
#include <QKeyEvent>
#include <QDebug>

KeyBoardNumber::KeyBoardNumber(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyBoardNumber)
{
    ui->setupUi(this);
    this->initForm();
    this->initProperty();
    qApp->setProperty("keyboardnum", false);
    this->hide();
}

KeyBoardNumber::~KeyBoardNumber()
{
    delete ui;
}


void KeyBoardNumber::initForm()
{
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint/* | Qt::FramelessWindowHint*/);
    isFirst = true;
    isPress = false;
    timerPress = new QTimer(this);
    connect(timerPress, SIGNAL(timeout()), this, SLOT(reClicked()));
    currentWidget = 0;

    QFont font = qApp->font();
    if(font.pixelSize() < 18) {
        font.setPixelSize(18);
    }

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(btnClicked()));
        b->setFont(font);
    }
}

void KeyBoardNumber::installInputMethod()
{
    if(!qApp->property("keyboardnum").toBool()) {
        connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(focusChanged(QWidget *, QWidget *)));
        qApp->installEventFilter(this);
        qApp->setProperty("keyboardnum", true);
    }
}

void KeyBoardNumber::unInstallInputMethod()
{
    if(qApp->property("keyboardnum").toBool()) {
        disconnect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(focusChanged(QWidget *, QWidget *)));
        qApp->removeEventFilter(this);
        qApp->setProperty("keyboardnum", false);
    }
}

void KeyBoardNumber::initProperty()
{
    ui->btn0->setProperty("btnNum", true);
    ui->btn1->setProperty("btnNum", true);
    ui->btn2->setProperty("btnNum", true);
    ui->btn3->setProperty("btnNum", true);
    ui->btn4->setProperty("btnNum", true);
    ui->btn5->setProperty("btnNum", true);
    ui->btn6->setProperty("btnNum", true);
    ui->btn7->setProperty("btnNum", true);
    ui->btn8->setProperty("btnNum", true);
    ui->btn9->setProperty("btnNum", true);

    ui->btnSub->setProperty("btnOther", true);
    ui->btnAdd->setProperty("btnOther", true);
    ui->btnDot->setProperty("btnOther", true);
    ui->btnDelete->setProperty("btnOther", true);
}

void KeyBoardNumber::focusChanged(QWidget *oldWidget, QWidget *nowWidget)
{ 
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget)) {
        currentWidget = nowWidget;
#ifndef arm
        if (oldWidget == 0x0 && !isFirst) {
            return;
        }
#endif
        isFirst = false;
        if (nowWidget->inherits("QLineEdit")) {
            currentLineEdit = (QLineEdit *)nowWidget;
            currentEditType = "QLineEdit";
            this->setVisible(true);
        } else if (nowWidget->inherits("QTextEdit")) {
            currentTextEdit = (QTextEdit *)nowWidget;
            currentEditType = "QTextEdit";
            this->setVisible(true);
        } else if (nowWidget->inherits("QPlainTextEdit")) {
            currentPlain = (QPlainTextEdit *)nowWidget;
            currentEditType = "QPlainTextEdit";
            this->setVisible(true);
        } else if (nowWidget->inherits("QTextBrowser")) {
            currentBrowser = (QTextBrowser *)nowWidget;
            currentEditType = "QTextBrowser";
            this->setVisible(true);
        } else if (nowWidget->inherits("QComboBox")) {
            QComboBox *cbox = (QComboBox *)nowWidget;
            //只有当下拉选择框处于编辑模式才可以输入
            if (cbox->isEditable()) {
                currentLineEdit = cbox->lineEdit() ;
                currentEditType = "QLineEdit";
                this->setVisible(true);
            }
        } else if (nowWidget->inherits("QSpinBox") ||
                   nowWidget->inherits("QDoubleSpinBox") ||
                   nowWidget->inherits("QDateEdit") ||
                   nowWidget->inherits("QTimeEdit") ||
                   nowWidget->inherits("QDateTimeEdit")) {
            currentWidget = nowWidget;
            currentEditType = "QWidget";
            this->setVisible(true);
        } else {
            currentWidget = 0;
            currentLineEdit = 0;
            currentTextEdit = 0;
            currentPlain = 0;
            currentBrowser = 0;
            currentEditType = "";
            this->setVisible(false);
        }
        int x = nowWidget->mapToGlobal(QPoint(0, 0)).x();
        int y = nowWidget->mapToGlobal(QPoint(0, 0)).y();
        this->move(x, y + nowWidget->height());
    }
    if(oldWidget)
    {
        if(oldWidget->inherits("QLineEdit") || oldWidget->inherits("QTextEdit") ||
                oldWidget->inherits("QSpinBox") || oldWidget->inherits("QPlainTextEdit") ||
                oldWidget->inherits("QDoubleSpinBox") || oldWidget->inherits("QTextBrowser") ||
                oldWidget->inherits("QComboBox") || oldWidget->inherits("QDateEdit") ||
                oldWidget->inherits("QTimeEdit") || oldWidget->inherits("QDateTimeEdit"))
            oldWidget->clearFocus();
    }
}

bool KeyBoardNumber::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (currentEditType != "") {
            if (obj != ui->btnClose) {
                this->setVisible(true);
            }
            btnPress = (QPushButton *)obj;
            if (checkPress()) {
                isPress = true;
                timerPress->start(500);
            }
        }
        return false;
    } else if (event->type() == QEvent::MouseButtonRelease) {
        btnPress = (QPushButton *)obj;
        if (checkPress()) {
            isPress = false;
            timerPress->stop();
        }
        return false;
    }
    return QWidget::eventFilter(obj, event);
}

bool KeyBoardNumber::checkPress()
{
    bool numok = btnPress->property("btnNum").toBool();
    bool otherok = btnPress->property("btnOther").toBool();
    if (numok || otherok) {
        return true;
    }
    return false;
}

void KeyBoardNumber::reClicked()
{
    if (isPress) {
        timerPress->setInterval(30);
        btnPress->click();
    }
}

void KeyBoardNumber::btnClicked()
{
    if (currentEditType == "") {
        return;
    }

    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();
    if (objectName == "btnDelete") {
        deleteValue();
    } else if (objectName == "btnClose") {
        this->setVisible(false);
        if(currentWidget) {
            QKeyEvent ev(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
            QApplication::sendEvent(currentWidget, &ev);
            currentWidget->clearFocus();
        }
    } else if (objectName == "btnEnter") {
        this->setVisible(false);
        if(currentWidget) {
            QKeyEvent ev(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
            QApplication::sendEvent(currentWidget, &ev);
            currentWidget->clearFocus();
        }
    } else if (objectName == "+") {
        insertValue("+");
    } else if (objectName == "-") {
        insertValue("-");
    } else {
        QString value = btn->text();
        insertValue(value);
    }
}

void KeyBoardNumber::insertValue(QString value)
{
    if(value == "+" || value == "-") {
        if (currentEditType == "QLineEdit") {
            QString text = currentLineEdit->text();
            if(text.startsWith("+") || text.startsWith("-")) text.remove(0, 1);
            text = value + text;
            currentLineEdit->setText(text);
        } else if (currentEditType == "QTextEdit") {
            QString text = currentTextEdit->toPlainText();
            if(text.startsWith("+") || text.startsWith("-")) text.remove(0, 1);
            text = value + text;
            currentTextEdit->setPlainText(text);
        } else if (currentEditType == "QPlainTextEdit") {
            QString text = currentPlain->toPlainText();
            if(text.startsWith("+") || text.startsWith("-")) text.remove(0, 1);
            text = value + text;
            currentPlain->setPlainText(value);
        } else if (currentEditType == "QTextBrowser") {
            QString text = currentBrowser->toPlainText();
            if(text.startsWith("+") || text.startsWith("-")) text.remove(0, 1);
            text = value + text;
            currentBrowser->setPlainText(value);
        } else if (currentEditType == "QWidget") {
            QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(value));
            QApplication::sendEvent(currentWidget, &keyPress);
        }
    } else {
        if (currentEditType == "QLineEdit") {
            currentLineEdit->insert(value);
        } else if (currentEditType == "QTextEdit") {
            currentTextEdit->insertPlainText(value);
        } else if (currentEditType == "QPlainTextEdit") {
            currentPlain->insertPlainText(value);
        } else if (currentEditType == "QTextBrowser") {
            currentBrowser->insertPlainText(value);
        } else if (currentEditType == "QWidget") {
            QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(value));
            QApplication::sendEvent(currentWidget, &keyPress);
        }
    }
}

void KeyBoardNumber::deleteValue()
{
    if (currentEditType == "QLineEdit") {
        currentLineEdit->backspace();
    } else if (currentEditType == "QTextEdit") {
        QTextCursor cursor = currentTextEdit->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType == "QPlainTextEdit") {
        QTextCursor cursor = currentPlain->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType == "QTextBrowser") {
        QTextCursor cursor = currentBrowser->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType == "QWidget") {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier, QString());
        QApplication::sendEvent(currentWidget, &keyPress);
    }
}

