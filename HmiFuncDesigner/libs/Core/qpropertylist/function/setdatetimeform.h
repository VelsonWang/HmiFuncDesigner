#ifndef SETDATETIMEFORM_H
#define SETDATETIMEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class SetDateTimeForm;
}

class SetDateTimeForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit SetDateTimeForm(QWidget *parent = nullptr);
    ~SetDateTimeForm();

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

private:
    void updateFromUi();

private:
    Ui::SetDateTimeForm *ui;
    QStringList m_args;
    QStringList m_showArgs;
};

#endif // SETDATETIMEFORM_H
