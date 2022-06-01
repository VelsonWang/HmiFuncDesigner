#ifndef STATECHANGEFORM_H
#define STATECHANGEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class StateChangeForm;
}

class StateChangeForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit StateChangeForm(QWidget *parent = nullptr);
    ~StateChangeForm();

public:
    virtual QString group();
    virtual QString name();
    virtual QString showName();
    virtual QStringList args();
    virtual QStringList showArgs();
    virtual QString description();
    virtual QString toString();
    virtual QString toShowString();
    virtual bool fromString(const QString func);

private slots:
    void on_cboSelectPage_currentTextChanged(const QString &page);

private:
    void updateFromUi();

private:
    Ui::StateChangeForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif // SLEEPFORM_H
