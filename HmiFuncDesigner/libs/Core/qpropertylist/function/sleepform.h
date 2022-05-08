#ifndef SLEEPFORM_H
#define SLEEPFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class SleepForm;
}

class SleepForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit SleepForm(QWidget *parent = nullptr);
    ~SleepForm();

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
    Ui::SleepForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif // SLEEPFORM_H
