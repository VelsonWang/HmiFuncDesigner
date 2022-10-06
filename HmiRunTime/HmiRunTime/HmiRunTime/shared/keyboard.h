#ifndef  KeyBoard_H
#define  KeyBoard_H

#include <QtGui>
#include <QSignalMapper>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolButton>
#include "ui_keyboard.h"
#include <QMap>
#include <QMultiMap>
#include <QVector>
#include <QTextStream>

namespace Ui {
    class KeyBoard;
}

class KeyBoard : public QDialog, Ui::KeyBoard
{
    Q_OBJECT

public:
    static KeyBoard *instance() {
        static KeyBoard instance;
        return &instance;
    }

    void installInputMethod();
    void unInstallInputMethod();

    KeyBoard(QWidget * parent = 0, Qt::WindowFlags fl = Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
     ~KeyBoard();

     void mouseMoveEvent(QMouseEvent *);
     void mousePressEvent(QMouseEvent *);

protected:
     void showEvent(QShowEvent *e);

public:
     QString text;
	 QWidget *lastFocusedWidget;

private:
     QSignalMapper *signalMapper;
     QPoint			dragPosition;
     int            m_pos;
	 int			m_keyType;
     QString	    m_preText;

private:
     void connectMapper();

private slots:
     void setMapper();
     void onCursorRight();
     void onCursorLeft();
     void onCursorUp();
     void onCursorDown();
     void onBackspace();
     void onEsc();
	 void saveFocusWidget(QWidget *oldFocus,QWidget *newFocus);
     void setDispText(const QString& text);

public:
	 void AdjustInputPos();

};

#endif //  KeyBoard_H
