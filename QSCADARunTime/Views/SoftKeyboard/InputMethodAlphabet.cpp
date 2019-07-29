#include "InputMethodAlphabet.h"
#include "ui_InputMethodAlphabet.h"
#include "qdesktopwidget.h"

QMutex InputMethodAlphabet::mutex_;
InputMethodAlphabet *InputMethodAlphabet::instance_ = 0;

InputMethodAlphabet::InputMethodAlphabet(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::InputMethodAlphabet) {
    ui_->setupUi(this);
    this->initProperty();
    this->initForm();
    this->changeStyle();
}

InputMethodAlphabet::~InputMethodAlphabet() {
    delete ui_;
}

void InputMethodAlphabet::init(QString position,
                               QString style,
                               int btnFontSize,
                               int labFontSize) {
    this->currentPosition_ = position;
    this->currentStyle_ = style;
    this->btnFontSize_ = btnFontSize;
    this->labFontSize_ = labFontSize;
    this->changeStyle();
    this->changeFont();
}

void InputMethodAlphabet::mouseMoveEvent(QMouseEvent *e) {
    if (mousePressed_ && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint_);
        e->accept();
    }
}

void InputMethodAlphabet::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        mousePressed_ = true;
        mousePoint_ = e->globalPos() - this->pos();
        e->accept();
    }
}

void InputMethodAlphabet::mouseReleaseEvent(QMouseEvent *) {
    mousePressed_ = false;
}

void InputMethodAlphabet::initForm() {
    this->mousePressed_ = false;
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint/* | Qt::FramelessWindowHint*/);

    QDesktopWidget w;
    deskWidth_ = w.availableGeometry().width();
    deskHeight_ = w.availableGeometry().height();
    frmWidth_ = this->width();
    frmHeight_ = this->height();

    QSqlDatabase dbConn;
    dbConn = QSqlDatabase::addDatabase("QSQLITE");
    dbConn.setDatabaseName(qApp->applicationDirPath() + "/py.db");
    dbConn.open();

    isFirst_ = true;
    isPress_ = false;
    timerPress_ = new QTimer(this);
    connect(timerPress_, SIGNAL(timeout()), this, SLOT(reClicked()));

    currentWidget_ = 0;
    currentLineEdit_ = 0;
    currentTextEdit_ = 0;
    currentPlain_ = 0;
    currentBrowser_ = 0;
    currentEditType_ = "";

    // 输入法面板的显示位置
    // control--显示在对应输入框的正下方
    // bottom--填充显示在底部
    // center--窗体居中显示
    currentPosition_ = "";

    // 输入法面板的样式
    // blue--淡蓝色
    // dev--dev风格
    // black--黑色
    // brown--灰黑色
    // lightgray--浅灰色
    // darkgray--深灰色
    // gray--灰色
    // silvery--银色
    currentStyle_ = "";

    // 输入法面板字体大小,如果需要更改面板字体大小,该这里即可
    btnFontSize_ = 10;
    labFontSize_ = 10;

    // 输入法初始模式
    // min--小写模式
    // max--大写模式
    // chinese--中文模式
    currentType_ = "min";
    changeType(currentType_);

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(btn_clicked()));
    }
}

void InputMethodAlphabet::installInputMethod() {
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(focusChanged(QWidget *, QWidget *)));
    qApp->installEventFilter(this);
}

void InputMethodAlphabet::unInstallInputMethod() {
    disconnect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
               this, SLOT(focusChanged(QWidget *, QWidget *)));
    qApp->removeEventFilter(this);
}

void InputMethodAlphabet::initProperty() {
    ui_->btnOther1->setProperty("btnOther", true);
    ui_->btnOther2->setProperty("btnOther", true);
    ui_->btnOther3->setProperty("btnOther", true);
    ui_->btnOther4->setProperty("btnOther", true);
    ui_->btnOther5->setProperty("btnOther", true);
    ui_->btnOther6->setProperty("btnOther", true);
    ui_->btnOther7->setProperty("btnOther", true);
    ui_->btnOther8->setProperty("btnOther", true);
    ui_->btnOther9->setProperty("btnOther", true);
    ui_->btnOther10->setProperty("btnOther", true);
    ui_->btnOther11->setProperty("btnOther", true);
    ui_->btnOther12->setProperty("btnOther", true);
    ui_->btnOther13->setProperty("btnOther", true);
    ui_->btnOther14->setProperty("btnOther", true);
    ui_->btnOther15->setProperty("btnOther", true);
    ui_->btnOther16->setProperty("btnOther", true);
    ui_->btnOther17->setProperty("btnOther", true);
    ui_->btnOther18->setProperty("btnOther", true);
    ui_->btnOther19->setProperty("btnOther", true);
    ui_->btnOther20->setProperty("btnOther", true);
    ui_->btnOther21->setProperty("btnOther", true);

    ui_->btnDot->setProperty("btnOther", true);
    ui_->btnSpace->setProperty("btnOther", true);
    ui_->btnDelete->setProperty("btnOther", true);

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

    ui_->btna->setProperty("btnLetter", true);
    ui_->btnb->setProperty("btnLetter", true);
    ui_->btnc->setProperty("btnLetter", true);
    ui_->btnd->setProperty("btnLetter", true);
    ui_->btne->setProperty("btnLetter", true);
    ui_->btnf->setProperty("btnLetter", true);
    ui_->btng->setProperty("btnLetter", true);
    ui_->btnh->setProperty("btnLetter", true);
    ui_->btni->setProperty("btnLetter", true);
    ui_->btnj->setProperty("btnLetter", true);
    ui_->btnk->setProperty("btnLetter", true);
    ui_->btnl->setProperty("btnLetter", true);
    ui_->btnm->setProperty("btnLetter", true);
    ui_->btnn->setProperty("btnLetter", true);
    ui_->btno->setProperty("btnLetter", true);
    ui_->btnp->setProperty("btnLetter", true);
    ui_->btnq->setProperty("btnLetter", true);
    ui_->btnr->setProperty("btnLetter", true);
    ui_->btns->setProperty("btnLetter", true);
    ui_->btnt->setProperty("btnLetter", true);
    ui_->btnu->setProperty("btnLetter", true);
    ui_->btnv->setProperty("btnLetter", true);
    ui_->btnw->setProperty("btnLetter", true);
    ui_->btnx->setProperty("btnLetter", true);
    ui_->btny->setProperty("btnLetter", true);
    ui_->btnz->setProperty("btnLetter", true);

    labCh_.append(ui_->labCh0);
    labCh_.append(ui_->labCh1);
    labCh_.append(ui_->labCh2);
    labCh_.append(ui_->labCh3);
    labCh_.append(ui_->labCh4);
    labCh_.append(ui_->labCh5);
    labCh_.append(ui_->labCh6);
    labCh_.append(ui_->labCh7);
    labCh_.append(ui_->labCh8);
    labCh_.append(ui_->labCh9);
    for (int i = 0; i < 10; i++) {
        labCh_[i]->installEventFilter(this);
    }
}

void InputMethodAlphabet::showPanel() {
    this->setVisible(true);
    int width = ui_->btn0->width();
    width = width > 40 ? width : 40;
    ui_->btnPre->setMinimumWidth(width);
    ui_->btnPre->setMaximumWidth(width);
    ui_->btnNext->setMinimumWidth(width);
    ui_->btnNext->setMaximumWidth(width);
    ui_->btnClose->setMinimumWidth(width);
    ui_->btnClose->setMaximumWidth(width);
}

// 识别鼠标单击汉字标签处获取对应汉字
bool InputMethodAlphabet::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == ui_->labCh0) {
            setChinese(0);
        } else if (obj == ui_->labCh1) {
            setChinese(1);
        } else if (obj == ui_->labCh2) {
            setChinese(2);
        } else if (obj == ui_->labCh3) {
            setChinese(3);
        } else if (obj == ui_->labCh4) {
            setChinese(4);
        } else if (obj == ui_->labCh5) {
            setChinese(5);
        } else if (obj == ui_->labCh6) {
            setChinese(6);
        } else if (obj == ui_->labCh7) {
            setChinese(7);
        } else if (obj == ui_->labCh8) {
            setChinese(8);
        } else if (obj == ui_->labCh9) {
            setChinese(9);
        } else if (currentEditType_ != "" && obj != ui_->btnClose) {
            showPanel();
        }
        btnPress_ = (QPushButton *)obj;
        if (checkPress()) {
            isPress_ = true;
            timerPress_->start(500);
        }
        return false;
    } else if (event->type() == QEvent::MouseButtonRelease) {
        btnPress_ = (QPushButton *)obj;
        if (checkPress()) {
            isPress_ = false;
            timerPress_->stop();
        }
        return false;
    } else if (event->type() == QEvent::KeyPress) {
        if (!isVisible()) {
            return QWidget::eventFilter(obj, event);
        }

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Space) {
            if (ui_->labPY->text() != "") {
                setChinese(0);
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            insertValue(ui_->labPY->text());
            ui_->labPY->setText("");
            selectChinese();
            emit enterPressed();
            return true;
        } else if (keyEvent->key() == Qt::Key_Shift) {
            ui_->btnType->click();
            return true;
        } else if (keyEvent->key() == Qt::Key_Escape) {
            ui_->btnClose->click();
            return true;
        } else if (keyEvent->key() == Qt::Key_Backspace) {
            ui_->btnDelete->click();
            return true;
        } else if (keyEvent->text() == "+" || keyEvent->text() == "=") {
            if (ui_->labPY->text() != "") {
                ui_->btnNext->click();
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->text() == "-" || keyEvent->text() == "_") {
            if (ui_->labPY->text() != "") {
                ui_->btnPre->click();
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->key() == Qt::Key_CapsLock) {
            if (currentType_ != "max") {
                currentType_ = "max";
            } else {
                currentType_ = "min";
            }
            changeType(currentType_);
            return true;
        } else {
            if (currentEditType_ == "QWidget") {
                return false;
            }
            QString key;
            if (currentType_ == "chinese") {
                key = keyEvent->text();
            } else if (currentType_ == "min") {
                key = keyEvent->text().toLower();
            } else if (currentType_ == "max") {
                key = keyEvent->text().toUpper();
            }
            QList<QPushButton *> btn = this->findChildren<QPushButton *>();
            foreach (QPushButton * b, btn) {
                if (b->text() == key) {
                    b->click();
                    return true;
                }
            }
        }
        return false;
    }
    return QWidget::eventFilter(obj, event);
}

bool InputMethodAlphabet::checkPress() {
    bool num_ok = btnPress_->property("btnNum").toBool();
    bool other_ok = btnPress_->property("btnOther").toBool();
    bool letter_ok = btnPress_->property("btnLetter").toBool();
    if (num_ok || other_ok || letter_ok) {
        return true;
    }
    return false;
}

void InputMethodAlphabet::reClicked() {
    if (isPress_) {
        timerPress_->setInterval(30);
        btnPress_->click();
    }
}

void InputMethodAlphabet::focusChanged(QWidget *oldWidget, QWidget *nowWidget) {
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
            showPanel();
        } else if (nowWidget->inherits("QTextEdit")) {
            currentTextEdit_ = (QTextEdit *)nowWidget;
            currentEditType_ = "QTextEdit";
            showPanel();
        } else if (nowWidget->inherits("QPlainTextEdit")) {
            currentPlain_ = (QPlainTextEdit *)nowWidget;
            currentEditType_ = "QPlainTextEdit";
            showPanel();
        } else if (nowWidget->inherits("QTextBrowser")) {
            currentBrowser_ = (QTextBrowser *)nowWidget;
            currentEditType_ = "QTextBrowser";
            showPanel();
        } else if (nowWidget->inherits("QComboBox")) {
            QComboBox *cbox = (QComboBox *)nowWidget;
            if (cbox->isEditable()) {
                currentLineEdit_ = cbox->lineEdit() ;
                currentEditType_ = "QLineEdit";
                showPanel();
            }
        } else if (nowWidget->inherits("QSpinBox") ||
                   nowWidget->inherits("QDoubleSpinBox") ||
                   nowWidget->inherits("QDateEdit") ||
                   nowWidget->inherits("QTimeEdit") ||
                   nowWidget->inherits("QDateTimeEdit")) {
            currentWidget_ = nowWidget;
            currentEditType_ = "QWidget";
            showPanel();
        } else {
            currentWidget_ = 0;
            currentLineEdit_ = 0;
            currentTextEdit_ = 0;
            currentPlain_ = 0;
            currentBrowser_ = 0;
            currentEditType_ = "";
            currentType_ = "min";
            changeType(currentType_);
            this->setVisible(false);
            emit closePressed();
        }

        if (currentPosition_ == "center") {
            QPoint pos = QPoint(deskWidth_ / 2 - frmWidth_ / 2, deskHeight_ / 2 - frmHeight_ / 2);
            this->setGeometry(pos.x(), pos.y(), frmWidth_, frmHeight_);
        } else if (currentPosition_ == "bottom") {
            this->setGeometry(0, deskHeight_ - frmHeight_, deskWidth_, frmHeight_);
        } else if (currentPosition_ == "control") {
            QRect rect = nowWidget->rect();
            QPoint pos = QPoint(rect.left(), rect.bottom() + 26);
            pos = nowWidget->mapToGlobal(pos);
            this->setGeometry(pos.x(), pos.y(), frmWidth_, frmHeight_);
        }
    }
}

void InputMethodAlphabet::changeType(QString type) {
    if (type == "max") {
        changeLetter(true);
        ui_->btnType->setText("大写");
        ui_->labInfo->setText("中文输入法--大写");
        ui_->btnOther12->setText("/");
        ui_->btnOther14->setText(":");
        ui_->btnOther17->setText(",");
        ui_->btnOther18->setText("\\");
        ui_->btnOther21->setText("\"");
    } else if (type == "min") {
        changeLetter(false);
        ui_->btnType->setText("小写");
        ui_->labInfo->setText("中文输入法--小写");
        ui_->btnOther12->setText("/");
        ui_->btnOther14->setText(":");
        ui_->btnOther17->setText(",");
        ui_->btnOther18->setText("\\");
        ui_->btnOther21->setText("\"");
    } else {
        changeLetter(false);
        ui_->btnType->setText("中文");
        ui_->labInfo->setText("中文输入法--中文");
        ui_->btnOther12->setText("。");
        ui_->btnOther14->setText("：");
        ui_->btnOther17->setText("，");
        ui_->btnOther18->setText("；");
        ui_->btnOther21->setText("“");
    }
    clearChinese();
    ui_->labPY->setText("");
}

void InputMethodAlphabet::changeLetter(bool isUpper) {
    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        if (b->property("btnLetter").toBool()) {
            if (isUpper) {
                b->setText(b->text().toUpper());
            } else {
                b->setText(b->text().toLower());
            }
        }
    }
}

void InputMethodAlphabet::selectChinese() {
    clearChinese();
    QSqlQuery query;
    QString currentPY = ui_->labPY->text();
    QString sql = "select [word] from [pinyin] where [pinyin]='" + currentPY + "';";
    query.exec(sql);
    //逐个将查询到的字词加入汉字队列
    while(query.next()) {
        QString result = query.value(0).toString();
        QStringList text = result.split(" ");
        foreach (QString txt, text) {
            if (txt.length() > 0) {
                allPY_.append(txt);
                currentPY_count_++;
            }
        }
    }
    showChinese();
}

void InputMethodAlphabet::showChinese()
{
    //每个版面最多显示10个汉字
    int count = 0;
    currentPY_.clear();
    for (int i = 0; i < 10; i++) {
        labCh_[i]->setText("");
    }

    for (int i = currentPY_index_; i < currentPY_count_; i++) {
        if (count == 10) {
            break;
        }
        QString txt = QString("%1.%2").arg(count).arg(allPY_[currentPY_index_]);
        currentPY_.append(allPY_[currentPY_index_]);
        labCh_[count]->setText(txt);
        count++;
        currentPY_index_++;
    }
}

void InputMethodAlphabet::btn_clicked() {
    if (currentEditType_ == "") {
        return;
    }

    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();
    if (objectName == "btnType") {
        if (currentType_ == "min") {
            currentType_ = "max";
        } else if (currentType_ == "max") {
            currentType_ = "chinese";
        } else if (currentType_ == "chinese") {
            currentType_ = "min";
        }
        changeType(currentType_);
    } else if (objectName == "btnDelete") {
        if (currentType_ == "chinese") {
            QString txt = ui_->labPY->text();
            int len = txt.length();
            if (len > 0) {
                ui_->labPY->setText(txt.left(len - 1));
                selectChinese();
            } else {
                deleteValue();
            }
        } else {
            deleteValue();
        }
    } else if (objectName == "btnPre") {
        if (currentPY_index_ >= 20) {
            //每次最多显示10个汉字,所以每次向前的时候索引要减20
            if (currentPY_index_ % 10 == 0) {
                currentPY_index_ -= 20;
            } else {
                currentPY_index_ = currentPY_count_ - (currentPY_count_ % 10) - 10;
            }
        } else {
            currentPY_index_ = 0;
        }
        showChinese();
    } else if (objectName == "btnNext") {
        if (currentPY_index_ < currentPY_count_ - 1) {
            showChinese();
        }
    } else if (objectName == "btnClose") {
        this->setVisible(false);
    } else if (objectName == "btnSpace") {
        insertValue(" ");
    } else {
        QString value = btn->text();
        //如果是&按钮，因为对应&被过滤,所以真实的text为去除前面一个&字符
        if (objectName == "btnOther7") {
            value = value.right(1);
        }
        //当前不是中文模式,则单击按钮对应text为传递参数
        if (currentType_ != "chinese") {
            insertValue(value);
        } else {
            //中文模式下,不允许输入特殊字符,单击对应数字按键取得当前索引的汉字
            if (btn->property("btnOther").toBool()) {
                if (ui_->labPY->text().length() == 0) {
                    insertValue(value);
                }
            } else if (btn->property("btnNum").toBool()) {
                if (ui_->labPY->text().length() == 0) {
                    insertValue(value);
                } else if (objectName == "btn0") {
                    setChinese(0);
                } else if (objectName == "btn1") {
                    setChinese(1);
                } else if (objectName == "btn2") {
                    setChinese(2);
                } else if (objectName == "btn3") {
                    setChinese(3);
                } else if (objectName == "btn4") {
                    setChinese(4);
                } else if (objectName == "btn5") {
                    setChinese(5);
                } else if (objectName == "btn6") {
                    setChinese(6);
                } else if (objectName == "btn7") {
                    setChinese(7);
                } else if (objectName == "btn8") {
                    setChinese(8);
                } else if (objectName == "btn9") {
                    setChinese(9);
                }
            } else if (btn->property("btnLetter").toBool()) {
                ui_->labPY->setText(ui_->labPY->text() + value);
                selectChinese();
            }
        }
    }
}

void InputMethodAlphabet::insertValue(QString value) {
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

void InputMethodAlphabet::deleteValue() {
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

void InputMethodAlphabet::setChinese(int index) {
    int count = currentPY_.count();
    if (count > index) {
        insertValue(currentPY_[index]);
        clearChinese();
        ui_->labPY->setText("");
    }
}

void InputMethodAlphabet::clearChinese()
{
    for (int i = 0; i < 10; i++) {
        labCh_[i]->setText("");
    }
    allPY_.clear();
    currentPY_.clear();
    currentPY_index_ = 0;
    currentPY_count_ = 0;
}

void InputMethodAlphabet::changeStyle() {
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

void InputMethodAlphabet::changeFont() {
    QFont btnFont(this->font().family(), btnFontSize_);
    QFont labFont(this->font().family(), labFontSize_);

    QList<QPushButton *> btns = ui_->widgetMain->findChildren<QPushButton *>();
    foreach (QPushButton * btn, btns) {
        btn->setFont(btnFont);
    }

    QList<QLabel *> labs = ui_->widgetTop->findChildren<QLabel *>();
    foreach (QLabel * lab, labs) {
        lab->setFont(labFont);
    }
    ui_->btnPre->setFont(labFont);
    ui_->btnNext->setFont(labFont);
    ui_->btnClose->setFont(labFont);
}

void InputMethodAlphabet::changeStyle(QString topColor,
                                      QString bottomColor,
                                      QString borderColor,
                                      QString textColor) {
    QStringList qss;
    qss.append(QString("QWidget#InputMethodAlphabet{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}")
               .arg(topColor).arg(bottomColor));
    qss.append("QPushButton{padding:5px;border-radius:3px;}");
    qss.append(QString("QPushButton:hover{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}")
               .arg(topColor).arg(bottomColor));
    qss.append(QString("QLabel,QPushButton{color:%1;}").arg(textColor));
    qss.append(QString("QPushButton#btnPre,QPushButton#btnNext,QPushButton#btnClose{padding:5px;}"));
    qss.append(QString("QPushButton{border:1px solid %1;}")
               .arg(borderColor));
    qss.append(QString("QLineEdit{border:1px solid %1;border-radius:5px;padding:2px;background:none;selection-background-color:%2;selection-color:%3;}")
               .arg(borderColor).arg(bottomColor).arg(topColor));
    this->setStyleSheet(qss.join(""));
}
