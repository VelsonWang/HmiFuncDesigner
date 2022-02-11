#ifndef QNAMEVALIDATOR_H
#define QNAMEVALIDATOR_H

#include "corelibglobal.h"
#include <QValidator>
#include <QRegExp>
#include <QStringList>
#include <QLineEdit>

class  QNameValidator: public QValidator
{
    Q_OBJECT
public:
    QNameValidator(const QString & reg = "", QObject* parent = 0);
    virtual State validate(QString &, int &) const;
    void setList(QStringList list);

protected:
    QRegExp m_reg;
    QStringList m_intermediateList;
};

class CORELIB_EXPORT QNameEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString textValue READ value WRITE setValue USER true)
public:
    QNameEdit(const QString &reg = "", QWidget* parent = 0);

    QString value() const;

    void setIntermediateList(QStringList list);
    void markIntermediateState();

public slots:
    void setValue(const QString & text);

private slots:
    void slotTextChanged(const QString &text);

protected:
    QString m_text;
    QNameValidator *m_validator;
};

#endif // QNAMEVALIDATOR_H
