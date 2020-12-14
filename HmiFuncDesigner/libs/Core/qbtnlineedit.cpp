#include "qbtnlineedit.h"

#include "qtexteditdialog.h"

#include <QPainter>
#include <QStyleOptionFrameV2>
#include <QPalette>

QBtnLineEdit::QBtnLineEdit(QWidget *parent) :
    QLineEdit(parent),
    m_button(new QPushButton(this))
{
    m_button->setText("...");
    m_button->setFixedSize(19,19);
    m_button->move(this->width()-21,1);
    m_button->setCursor(QCursor(Qt::ArrowCursor));
    m_button->setFocusProxy(this);
    connect(m_button,SIGNAL(clicked()),this,SLOT(btnClicked()));
    setTextMargins(0,0,21,0);
    connect(this,SIGNAL(textEdited(QString)),this,SLOT(text_changed()));
}

void QBtnLineEdit::resizeEvent(QResizeEvent *)
{
    m_button->move(this->width()-21,1);
}

void QBtnLineEdit::btnClicked()
{
    QTextEditDialog dlg(this->value(),this);

    dlg.exec();
    if(dlg.getRet()==1)
    {
        this->setValue(dlg.getText());
    }
}

void QBtnLineEdit::setValue(const QString &value)
{
    QString str=value;
    str.replace("\r\n"," ");
    str.replace("\n"," ");
    setText(str);
    m_value=value;
}

QString QBtnLineEdit::value()
{
    return m_value;
}

void QBtnLineEdit::text_changed()
{
    m_value=text();
}
