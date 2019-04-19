#include "InputMethodNumber.h"
#include "ui_InputMethodNumber.h"

QMutex InputMethodNumber::mutex_;
InputMethodNumber *InputMethodNumber::instance_ = 0;

InputMethodNumber::InputMethodNumber(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::InputMethodNumber) {
    ui_->setupUi(this);
    this->initForm();
    this->initProperty();
    this->changeStyle();
}

InputMethodNumber::~InputMethodNumber() {
    delete ui_;
}

void InputMethodNumber::init(QString style, int fontSize) {
    this->currentStyle_ = style;
    this->currentFontSize_ = fontSize;
    this->changeStyle();
}

void InputMethodNumber::initForm() {
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint/* | Qt::FramelessWindowHint*/);
    isFirst_ = true;
    isPress_ = false;
    timerPress_ = new QTimer(this);
    connect(timerPress_, SIGNAL(timeout()), this, SLOT(reClicked()));
    currentWidget_ = 0;

    // blue--淡蓝色
    // dev--dev风格
    // black--黑色
    // brown--灰黑色
    // lightgray--浅灰色
    // darkgray--深灰色
    // gray--灰色
    // silvery--银色
    currentStyle_ = "";

    //输入法面板字体大小
    currentFontSize_ = 10;

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(btn_clicked()));
    }
}

void InputMethodNumber::installInputMethod() {
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(focusChanged(QWidget *, QWidget *)));
    qApp->installEventFilter(this);
}

void InputMethodNumber::unInstallInputMethod() {
    disconnect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
               this, SLOT(focusChanged(QWidget *, QWidget *)));
    qApp->removeEventFilter(this);
}

void InputMethodNumber::initProperty() {
    ui_->btn0->setProperty("btnNum", true);
    ui_->btn1->setProperty("btnNum", true);
    ui_->btn2->setProperty("btnNum", true);
    ui_->btn3->setProperty("btnNum", true);
    ui_->btn4->setProperty("btnNum", true);
    ui_->btn5->setProperty("btnNum", true);
    ui_->btn6->setProperty("btnNum", true);
    ui_->btn7->setProperty("btnNum", true);
    ui_->btn8->setProperty("btnNum", true);
    ui_->btn9->setProperty("btnNum", true);
    ui_->btn00->setProperty("btnNum", true);

    ui_->btnDot->setProperty("btnOther", true);
    ui_->btnSpace->setProperty("btnOther", true);
    ui_->btnDelete->setProperty("btnOther", true);
}

void InputMethodNumber::focusChanged(QWidget *oldWidget, QWidget *nowWidget) {
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget)) {
#ifndef __arm__
        if (oldWidget == 0x0 && !isFirst_) {
            return;
        }
#endif
        isFirst_ = false;
        if (nowWidget->inherits("QLineEdit")) {
            currentLineEdit_ = (QLineEdit *)nowWidget;
            currentEditType_ = "QLineEdit";
            this->setVisible(true);
        } else if (nowWidget->inherits("QTextEdit")) {
            currentTextEdit_ = (QTextEdit *)nowWidget;
            currentEditType_ = "QTextEdit";
            this->setVisible(true);
        } else if (nowWidget->inherits("QPlainTextEdit")) {
            currentPlain_ = (QPlainTextEdit *)nowWidget;
            currentEditType_ = "QPlainTextEdit";
            this->setVisible(true);
        } else if (nowWidget->inherits("QTextBrowser")) {
            currentBrowser_ = (QTextBrowser *)nowWidget;
            currentEditType_ = "QTextBrowser";
            this->setVisible(true);
        } else if (nowWidget->inherits("QComboBox")) {
            QComboBox *cbox = (QComboBox *)nowWidget;
            //只有当下拉选择框处于编辑模式才可以输入
            if (cbox->isEditable()) {
                currentLineEdit_ = cbox->lineEdit() ;
                currentEditType_ = "QLineEdit";
                this->setVisible(true);
            }
        } else if (nowWidget->inherits("QSpinBox") ||
                   nowWidget->inherits("QDoubleSpinBox") ||
                   nowWidget->inherits("QDateEdit") ||
                   nowWidget->inherits("QTimeEdit") ||
                   nowWidget->inherits("QDateTimeEdit")) {
            currentWidget_ = nowWidget;
            currentEditType_ = "QWidget";
            this->setVisible(true);
        } else {
            currentWidget_ = 0;
            currentLineEdit_ = 0;
            currentTextEdit_ = 0;
            currentPlain_ = 0;
            currentBrowser_ = 0;
            currentEditType_ = "";
            this->setVisible(false);
        }

        QRect rect = nowWidget->rect();
        QPoint pos = QPoint(rect.left(), rect.bottom() + 26);
        pos = nowWidget->mapToGlobal(pos);
        this->setGeometry(pos.x(), pos.y(), this->width(), this->height());
    }
}

bool InputMethodNumber::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (currentEditType_ != "") {
            if (obj != ui_->btnClose) {
                this->setVisible(true);
            }
            btnPress_ = (QPushButton *)obj;
            if (checkPress()) {
                isPress_ = true;
                timerPress_->start(500);
            }
        }
        return false;
    } else if (event->type() == QEvent::MouseButtonRelease) {
        btnPress_ = (QPushButton *)obj;
        if (checkPress()) {
            isPress_ = false;
            timerPress_->stop();
        }
        return false;
    }
    return QWidget::eventFilter(obj, event);
}

bool InputMethodNumber::checkPress() {
    bool num_ok = btnPress_->property("btnNum").toBool();
    bool other_ok = btnPress_->property("btnOther").toBool();
    if (num_ok || other_ok) {
        return true;
    }
    return false;
}

void InputMethodNumber::reClicked() {
    if (isPress_) {
        timerPress_->setInterval(30);
        btnPress_->click();
    }
}

void InputMethodNumber::btn_clicked() {
    if (currentEditType_ == "") {
        return;
    }

    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();
    if (objectName == "btnDelete") {
        deleteValue();
    } else if (objectName == "btnClose") {
        this->setVisible(false);
        emit closePressed();
    } else if (objectName == "btnEnter") {
        this->setVisible(false);
        emit enterPressed();
    } else if (objectName == "btnSpace") {
        insertValue(" ");
    } else {
        QString value = btn->text();
        insertValue(value);
    }
}

void InputMethodNumber::insertValue(QString value) {
    if (currentEditType_ == "QLineEdit") {
        currentLineEdit_->insert(value);
    } else if (currentEditType_ == "QTextEdit") {
        currentTextEdit_->insertPlainText(value);
    } else if (currentEditType_ == "QPlainTextEdit") {
        currentPlain_->insertPlainText(value);
    } else if (currentEditType_ == "QTextBrowser") {
        currentBrowser_->insertPlainText(value);
    } else if (currentEditType_ == "QWidget") {
        QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(value));
        QApplication::sendEvent(currentWidget_, &keyPress);
    }
}

void InputMethodNumber::deleteValue() {
    if (currentEditType_ == "QLineEdit") {
        currentLineEdit_->backspace();
    } else if (currentEditType_ == "QTextEdit") {
        QTextCursor cursor = currentTextEdit_->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType_ == "QPlainTextEdit") {
        QTextCursor cursor = currentPlain_->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType_ == "QTextBrowser") {
        QTextCursor cursor = currentBrowser_->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType_ == "QWidget") {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier, QString());
        QApplication::sendEvent(currentWidget_, &keyPress);
    }
}

void InputMethodNumber::changeStyle() {
    if (currentStyle_ == "blue") {
        changeStyle("#DEF0FE", "#C0DEF6", "#C0DCF2", "#386487");
    } else if (currentStyle_ == "dev") {
        changeStyle("#C0D3EB", "#BCCFE7", "#B4C2D7", "#324C6C");
    } else if (currentStyle_ == "gray") {
        changeStyle("#E4E4E4", "#A2A2A2", "#A9A9A9", "#000000");
    } else if (currentStyle_ == "lightgray") {
        changeStyle("#EEEEEE", "#E5E5E5", "#D4D0C8", "#6F6F6F");
    } else if (currentStyle_ == "darkgray") {
        changeStyle("#D8D9DE", "#C8C8D0", "#A9ACB5", "#5D5C6C");
    } else if (currentStyle_ == "black") {
        changeStyle("#4D4D4D", "#292929", "#D9D9D9", "#CACAD0");
    } else if (currentStyle_ == "brown") {
        changeStyle("#667481", "#566373", "#C2CCD8", "#E7ECF0");
    } else if (currentStyle_ == "silvery") {
        changeStyle("#E1E4E6", "#CCD3D9", "#B2B6B9", "#000000");
    }
}

void InputMethodNumber::changeStyle(QString topColor,
                                    QString bottomColor,
                                    QString borderColor,
                                    QString textColor) {
    QStringList qss;
    qss.append(QString("QWidget#InputMethodNumber{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}")
               .arg(topColor).arg(bottomColor));
    qss.append("QPushButton{padding:5px;border-radius:3px;}");
    qss.append(QString("QPushButton:hover{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}")
               .arg(topColor).arg(bottomColor));
    qss.append(QString("QLabel,QPushButton{font-size:%1pt;color:%2;}")
               .arg(currentFontSize_).arg(textColor));
    qss.append(QString("QPushButton#btnPre,QPushButton#btnNext,QPushButton#btnClose{padding:5px;}"));
    qss.append(QString("QPushButton{border:1px solid %1;}")
               .arg(borderColor));
    qss.append(QString("QLineEdit{border:1px solid %1;border-radius:5px;padding:2px;background:none;selection-background-color:%2;selection-color:%3;}")
               .arg(borderColor).arg(bottomColor).arg(topColor));
    this->setStyleSheet(qss.join(""));
}
