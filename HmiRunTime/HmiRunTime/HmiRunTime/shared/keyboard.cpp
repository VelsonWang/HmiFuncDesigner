#include <QtGui>
#include <QRect>
#include <QWidget>
#include <QWidgetList>
#include <QLineEdit>
#include <QTextBrowser>
#include <QComboBox>
#include <QPlainTextEdit>
#include "keyboard.h"

KeyBoard::KeyBoard(QWidget * parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi(this);

    this->setWindowFlags( Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
    this->setStyleSheet("QToolButton[autoRaise=\"false\"]{background-color:lightgrey;/*font-weight:bold;*/border:0px;border-radius:1px;}QToolButton::pressed{background-color:grey;}");

    signalMapper = new QSignalMapper(this);
    setMapper();
    connectMapper();

    connect(qApp,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(saveFocusWidget(QWidget*,QWidget*)));
    connect(signalMapper,SIGNAL(mapped(const QString&)),this,SLOT(setDispText(const QString&)));
    connect(toolButton_backspace,SIGNAL(clicked()),this,SLOT(onBackspace()));

    qApp->setProperty("keyboardnum", false);
    this->hide();
}

KeyBoard::~KeyBoard()
{

}

void KeyBoard::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void KeyBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pos = event->y();
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    } else if (event->button() == Qt::RightButton) {
        hide();
    }
}

void KeyBoard::showEvent(QShowEvent *e)
{
    setStyleSheet("QWidget{font: 75 12pt 'Arial';}");
    QWidget::showEvent(e);
}

void KeyBoard::saveFocusWidget(QWidget *oldWidget, QWidget *nowWidget)
{
    if(nowWidget != NULL && !this->isAncestorOf(nowWidget)) {
        if(nowWidget->inherits("QLineEdit") || nowWidget->inherits("QTextEdit") ||
                nowWidget->inherits("QSpinBox") || nowWidget->inherits("QPlainTextEdit") ||
                nowWidget->inherits("QDoubleSpinBox") || nowWidget->inherits("QTextBrowser") ||
                nowWidget->inherits("QComboBox") || nowWidget->inherits("QDateEdit") ||
                nowWidget->inherits("QTimeEdit") || nowWidget->inherits("QDateTimeEdit")) {
            if(nowWidget != lastFocusedWidget) {
                this->setVisible(true);
                int x = nowWidget->mapToGlobal(QPoint(0, 0)).x();
                int y = nowWidget->mapToGlobal(QPoint(0, 0)).y();
                this->move(x, y + nowWidget->height());
            }
        }
        lastFocusedWidget = nowWidget;
    }
    if(oldWidget) {
        if(oldWidget->inherits("QLineEdit") || oldWidget->inherits("QTextEdit") ||
                oldWidget->inherits("QSpinBox") || oldWidget->inherits("QPlainTextEdit") ||
                oldWidget->inherits("QDoubleSpinBox") || oldWidget->inherits("QTextBrowser") ||
                oldWidget->inherits("QComboBox") || oldWidget->inherits("QDateEdit") ||
                oldWidget->inherits("QTimeEdit") || oldWidget->inherits("QDateTimeEdit")) {
            lastFocusedWidget->clearFocus();
        }
    }
}

void KeyBoard::setMapper()
{
    signalMapper->setMapping(toolButton_0,toolButton_0->text());
    signalMapper->setMapping(toolButton_1,toolButton_1->text());
    signalMapper->setMapping(toolButton_2,toolButton_2->text());
    signalMapper->setMapping(toolButton_3,toolButton_3->text());
    signalMapper->setMapping(toolButton_4,toolButton_4->text());
    signalMapper->setMapping(toolButton_5,toolButton_5->text());
    signalMapper->setMapping(toolButton_6,toolButton_6->text());
    signalMapper->setMapping(toolButton_7,toolButton_7->text());
    signalMapper->setMapping(toolButton_8,toolButton_8->text());
    signalMapper->setMapping(toolButton_9,toolButton_9->text());
    signalMapper->setMapping(toolButton_minus,toolButton_minus->text());
    signalMapper->setMapping(toolButton_plus,toolButton_plus->text());
    signalMapper->setMapping(toolButton_dot,toolButton_dot->text());
    signalMapper->setMapping(toolButton_esc,"esc");
    signalMapper->setMapping(toolButton_enter,"enter");
}

void KeyBoard::connectMapper()
{
    connect(toolButton_0,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_1,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_2,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_3,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_4,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_5,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_6,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_7,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_8,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_9,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_minus,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_plus,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_dot,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_esc,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(toolButton_enter,SIGNAL(clicked()),signalMapper,SLOT(map()));
}

void KeyBoard::onCursorUp()
{
    QKeyEvent upPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    if(this->lastFocusedWidget)
        QApplication::sendEvent(this->lastFocusedWidget, &upPress);
}

void KeyBoard::onCursorDown()
{
    QKeyEvent downPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    if(this->lastFocusedWidget)
        QApplication::sendEvent(this->lastFocusedWidget, &downPress);
}

void KeyBoard::onCursorRight()
{
    QKeyEvent rightPress(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier);
    if(this->lastFocusedWidget)
        QApplication::sendEvent(this->lastFocusedWidget, &rightPress);
}

void KeyBoard::onCursorLeft()
{
    QKeyEvent leftPress(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
    if(this->lastFocusedWidget)
        QApplication::sendEvent(this->lastFocusedWidget, &leftPress);
}

void KeyBoard::onEsc()
{
    this->hide();
}

void KeyBoard::onBackspace()
{
    QKeyEvent delPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    if(this->lastFocusedWidget)
        QApplication::sendEvent(this->lastFocusedWidget, &delPress);
}

void KeyBoard::AdjustInputPos()
{
    QWidget *widget = QApplication::focusWidget();
    if (!widget)
        return;

    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);
    this->move(panelPos);
}


void KeyBoard::setDispText(const QString& text)
{
    if (!lastFocusedWidget)
        return;

    if (text.toLower().trimmed() == "esc") {
        this->setVisible(false);
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QApplication::sendEvent(lastFocusedWidget, &keyPress);
        if (!lastFocusedWidget) return;
        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Escape, Qt::NoModifier);
        QApplication::sendEvent(lastFocusedWidget, &keyRelease);
        lastFocusedWidget->setFocus();
        return;
    } else if (text.toLower().trimmed()  == "enter") {
        this->setVisible(false);
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QApplication::sendEvent(lastFocusedWidget, &keyPress);
        if (!lastFocusedWidget) return;
        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QApplication::sendEvent(lastFocusedWidget, &keyRelease);
        lastFocusedWidget->setFocus();
        return;
    } else if(text == "+" || text == "-") {
        if (lastFocusedWidget->inherits("QLineEdit")) {
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(lastFocusedWidget);
            if(lineEdit) {
                QString tmpText = lineEdit->text();
                if(tmpText.startsWith("+") || tmpText.startsWith("-")) tmpText.remove(0, 1);
                tmpText = text + tmpText;
                lineEdit->setText(tmpText);
            }
        } else if (lastFocusedWidget->inherits("QTextEdit")) {
            QTextEdit *edit = qobject_cast<QTextEdit *>(lastFocusedWidget);
            if(edit) {
                QString tmpText = edit->toPlainText();
                if(tmpText.startsWith("+") || tmpText.startsWith("-")) tmpText.remove(0, 1);
                tmpText = text + tmpText;
                edit->setText(tmpText);
            }
        } else if (lastFocusedWidget->inherits("QPlainTextEdit")) {
            QPlainTextEdit *edit = qobject_cast<QPlainTextEdit *>(lastFocusedWidget);
            if(edit) {
                QString tmpText = edit->toPlainText();
                if(tmpText.startsWith("+") || tmpText.startsWith("-")) tmpText.remove(0, 1);
                tmpText = text + tmpText;
                edit->setPlainText(tmpText);
            }
        } else if (lastFocusedWidget->inherits("QTextBrowser")) {
            QTextBrowser *edit = qobject_cast<QTextBrowser *>(lastFocusedWidget);
            if(edit) {
                QString tmpText = edit->toPlainText();
                if(tmpText.startsWith("+") || tmpText.startsWith("-")) tmpText.remove(0, 1);
                tmpText = text + tmpText;
                edit->setPlainText(tmpText);
            }
        }
        return;
    } else if(text == ".") {
        if (lastFocusedWidget->inherits("QLineEdit")) {
            QLineEdit *edit = qobject_cast<QLineEdit *>(lastFocusedWidget);
            if(edit) {
                if(edit->text().trimmed().length() < 1 || edit->text().indexOf(".") > 0) return;
            }
        } else if (lastFocusedWidget->inherits("QComboBox")) {
            QComboBox *edit = qobject_cast<QComboBox *>(lastFocusedWidget);
            if(edit) {
                if(edit->currentText().trimmed().length() < 1 || edit->currentText().indexOf(".") > 0) return;
            }
        } else if (lastFocusedWidget->inherits("QTextEdit")) {
            QTextEdit *edit = qobject_cast<QTextEdit *>(lastFocusedWidget);
            if(edit) {
                if(edit->toPlainText().trimmed().length() < 1 || edit->toPlainText().indexOf(".") > 0) return;
            }
        } else if (lastFocusedWidget->inherits("QPlainTextEdit")) {
            QPlainTextEdit *edit = qobject_cast<QPlainTextEdit *>(lastFocusedWidget);
            if(edit) {
                if(edit->toPlainText().trimmed().length() < 1 || edit->toPlainText().indexOf(".") > 0) return;
            }
        } else if (lastFocusedWidget->inherits("QTextBrowser")) {
            QTextBrowser *edit = qobject_cast<QTextBrowser *>(lastFocusedWidget);
            if(edit) {
                if(edit->toPlainText().trimmed().length() < 1 || edit->toPlainText().indexOf(".") > 0) return;
            }
        }
    }

    QKeyEvent keyPress(QEvent::KeyPress, text.at(0).unicode(), Qt::NoModifier, QString(text.at(0)));
    QApplication::sendEvent(lastFocusedWidget, &keyPress);

    if (!lastFocusedWidget)
        return;

    QKeyEvent keyRelease(QEvent::KeyRelease, text.at(0).unicode(), Qt::NoModifier, QString(text.at(0)));
    QApplication::sendEvent(lastFocusedWidget, &keyRelease);
    lastFocusedWidget->setFocus();
}

void KeyBoard::installInputMethod()
{
    if(!qApp->property("keyboardnum").toBool()) {
        connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(saveFocusWidget(QWidget *, QWidget *)));
        qApp->installEventFilter(this);
        qApp->setProperty("keyboardnum", true);
        this->hide();
    }
}

void KeyBoard::unInstallInputMethod()
{
    if(qApp->property("keyboardnum").toBool()) {
        disconnect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(saveFocusWidget(QWidget *, QWidget *)));
        qApp->removeEventFilter(this);
        qApp->setProperty("keyboardnum", false);
        this->hide();
    }
}
