#include "inputmethodnumber.h"
#include "ui_inputmethodnumber.h"

InputMethodNumber::InputMethodNumber(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputMethodNumber)
{
    ui->setupUi(this);
    this->initForm();
    this->initProperty();
}

InputMethodNumber::~InputMethodNumber()
{
    delete ui;
}

void InputMethodNumber::initForm()
{
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    isPress = false;
    timerPress = new QTimer(this);
    connect(timerPress, SIGNAL(timeout()), this, SLOT(reClicked()));
    curWidget = 0;
    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(btn_clicked()));
    }
}

void InputMethodNumber::initProperty()
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
    ui->btn00->setProperty("btnNum", true);

    ui->btnDot->setProperty("btnOther", true);
    ui->btnSpace->setProperty("btnOther", true);
    ui->btnDelete->setProperty("btnOther", true);
}

void InputMethodNumber::setFocusWidget(QWidget *pObj)
{
    if (pObj && !this->isAncestorOf(pObj)) {
        if (pObj->inherits("QLineEdit")) {
            curLineEdit = (QLineEdit *)pObj;
            curEditType = "QLineEdit";
            this->setVisible(true);
        } else if (pObj->inherits("QTextEdit")) {
            curTextEdit = (QTextEdit *)pObj;
            curEditType = "QTextEdit";
            this->setVisible(true);
        } else if (pObj->inherits("QPlainTextEdit")) {
            curPlain = (QPlainTextEdit *)pObj;
            curEditType = "QPlainTextEdit";
            this->setVisible(true);
        } else if (pObj->inherits("QTextBrowser")) {
            curBrowser = (QTextBrowser *)pObj;
            curEditType = "QTextBrowser";
            this->setVisible(true);
        } else if (pObj->inherits("QComboBox")) {
            QComboBox *cbox = (QComboBox *)pObj;
            //只有当下拉选择框处于编辑模式才可以输入
            if (cbox->isEditable()) {
                curLineEdit = cbox->lineEdit() ;
                curEditType = "QLineEdit";
                this->setVisible(true);
            }
        } else if (pObj->inherits("QSpinBox") ||
                   pObj->inherits("QDoubleSpinBox") ||
                   pObj->inherits("QDateEdit") ||
                   pObj->inherits("QTimeEdit") ||
                   pObj->inherits("QDateTimeEdit")) {
            curWidget = pObj;
            curEditType = "QWidget";
            this->setVisible(true);
        } else {
            curWidget = 0;
            curLineEdit = 0;
            curTextEdit = 0;
            curPlain = 0;
            curBrowser = 0;
            curEditType = "";
            this->setVisible(false);
        }

        QRect rect = pObj->rect();
        QPoint pos = QPoint(rect.left() + rect.width(), rect.top());
        pos = pObj->mapToGlobal(pos);
        this->move(pos.x(), pos.y());
    }
    if(pObj) {
        if(this->isVisible()) {
            this->activateWindow();
        }
    }
}

bool InputMethodNumber::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (curEditType != "") {
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

bool InputMethodNumber::checkPress()
{
    bool num_ok = btnPress->property("btnNum").toBool();
    bool other_ok = btnPress->property("btnOther").toBool();
    if (num_ok || other_ok) {
        return true;
    }
    return false;
}

void InputMethodNumber::reClicked()
{
    if (isPress) {
        timerPress->setInterval(30);
        btnPress->click();
    }
}

void InputMethodNumber::btn_clicked()
{
    if (curEditType == "") {
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

void InputMethodNumber::insertValue(QString value)
{
    if (curEditType == "QLineEdit") {
        curLineEdit->insert(value);
    } else if (curEditType == "QTextEdit") {
        curTextEdit->insertPlainText(value);
    } else if (curEditType == "QPlainTextEdit") {
        curPlain->insertPlainText(value);
    } else if (curEditType == "QTextBrowser") {
        curBrowser->insertPlainText(value);
    } else if (curEditType == "QWidget") {
        QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(value));
        QApplication::sendEvent(curWidget, &keyPress);
    }
}

void InputMethodNumber::deleteValue()
{
    if (curEditType == "QLineEdit") {
        curLineEdit->backspace();
    } else if (curEditType == "QTextEdit") {
        QTextCursor cursor = curTextEdit->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (curEditType == "QPlainTextEdit") {
        QTextCursor cursor = curPlain->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (curEditType == "QTextBrowser") {
        QTextCursor cursor = curBrowser->textCursor();
        if(cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (curEditType == "QWidget") {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier, QString());
        QApplication::sendEvent(curWidget, &keyPress);
    }
}
