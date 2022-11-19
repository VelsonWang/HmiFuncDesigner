#ifndef WAITFORMILLISECFORM_H
#define WAITFORMILLISECFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class WaitForMillisecForm;
}

class WaitForMillisecForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit WaitForMillisecForm(QWidget *parent = nullptr);
    ~WaitForMillisecForm();

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
    Ui::WaitForMillisecForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif // WAITFORMILLISECFORM_H
