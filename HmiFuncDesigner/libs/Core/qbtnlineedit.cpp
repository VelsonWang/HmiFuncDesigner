#include "qbtnlineedit.h"

#include "qtexteditdialog.h"

#include <QPainter>
#include <QStyleOptionFrameV2>
#include <QPalette>

QBtnLineEdit::QBtnLineEdit(QWidget *parent) :
    QLineEdit(parent),
    button(new QPushButton(this))
{
    button->setText("...");
    button->setFixedSize(19, 19);
    button->move(this->width() - 21, 1);
    button->setCursor(QCursor(Qt::ArrowCursor));
    button->setFocusProxy(this);
    connect(button, SIGNAL(clicked()), this, SLOT(btnClicked()));
    setTextMargins(0, 0, 21, 0);
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(textChanged()));
}

void QBtnLineEdit::resizeEvent(QResizeEvent *)
{
    button->move(this->width() - 21, 1);
}

void QBtnLineEdit::btnClicked()
{
    QTextEditDialog dlg(this->value(), this);
    dlg.exec();
    if(dlg.getRet() == 1) {
        this->setValue(dlg.getText());
    }
}

void QBtnLineEdit::setValue(const QString &value)
{
    QString str = value;
    str.replace("\r\n", " ");
    str.replace("\n", " ");
    setText(str);
    textVal = value;
}

QString QBtnLineEdit::value()
{
    return textVal;
}

void QBtnLineEdit::textChanged()
{
    textVal = this->text();
}
