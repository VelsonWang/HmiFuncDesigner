#include "qnamevalidator.h"

QNameValidator::QNameValidator(const QString & reg, QObject* parent):
    QValidator(parent),
    m_reg(reg)
{

}

QValidator::State QNameValidator::validate(QString &input, int &) const
{
    if(!m_reg.exactMatch(input)) {
        return Intermediate;
    }
    if(m_intermediateList.indexOf(input) >= 0) {
        return Intermediate;
    }
    return Acceptable;
}

void QNameValidator::setList(QStringList list)
{
    m_intermediateList = list;
}

QNameEdit::QNameEdit(const QString &reg, QWidget* parent):
    QLineEdit(parent),
    m_validator(new QNameValidator(reg, this))
{
    setValidator(m_validator);
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));
}

void QNameEdit::markIntermediateState()
{
    if(this->hasAcceptableInput()) {
        setPalette(QPalette());
    } else {
        QPalette pal = this->palette();
        pal.setColor(QPalette::Active, QPalette::Text, Qt::red);
        setPalette(pal);
    }
}

void QNameEdit::setValue(const QString &str)
{
    m_text = str;
    markIntermediateState();
    setText(str);
}

QString QNameEdit::value() const
{
    if(hasAcceptableInput()) {
        return text();
    } else {
        return m_text;
    }
}

void QNameEdit::slotTextChanged(const QString &)
{
    markIntermediateState();
}

void QNameEdit::setIntermediateList(QStringList list)
{
    m_validator->setList(list);
}
