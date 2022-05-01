#include "qlineeditex.h"
#include "keyboard/inputmethodnumber.h"
#include <QFocusEvent>
#include <QDebug>

QLineEditEx::QLineEditEx(QWidget *parent) : QLineEdit(parent)
{
    m_tagId = "";
    m_tag = NULL;
    m_editing = false;
}

void QLineEditEx::paintEvent(QPaintEvent *event)
{
    if(!m_editing) {
        if(m_tag) {
            this->setText(m_tag->toString());
        } else {
            m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
        }
    }
    QLineEdit::paintEvent(event);
}


QString QLineEditEx::tag()
{
    return m_tagId;
}

void QLineEditEx::setTag(const QString &szTag)
{
    if(m_tagId != szTag) {
        m_tagId = szTag;
        m_tag = RealTimeDB::instance()->tag(tagId(szTag));
    }
}

void QLineEditEx::focusInEvent(QFocusEvent *event)
{

}

void QLineEditEx::mousePressEvent(QMouseEvent *)
{
    m_editing = true;
    if(m_tag) {
        this->setText(m_tag->toString());
    } else {
        m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
    }
    if(tag() != "") {
        connect(InputMethodNumber::instance(), SIGNAL(enterPressed()), this, SLOT(enterPressed()));
        InputMethodNumber::instance()->setFocusWidget(this);
        this->setText(m_tag->toString());
        this->setFocus();
    }
}

void QLineEditEx::enterPressed()
{
    m_editing = false;
    if(tag() != "") {
        m_tag->fromString(this->text(), true);
    }
    disconnect(InputMethodNumber::instance(), SIGNAL(enterPressed()), this, SLOT(enterPressed()));
}







