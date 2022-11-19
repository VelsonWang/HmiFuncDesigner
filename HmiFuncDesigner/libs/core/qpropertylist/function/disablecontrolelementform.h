#ifndef DISABLECONTROLELEMENTFORM_H
#define DISABLECONTROLELEMENTFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class DisableControlElementForm;
}

class DisableControlElementForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit DisableControlElementForm(QWidget *parent = nullptr);
    ~DisableControlElementForm();

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
    Ui::DisableControlElementForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif
