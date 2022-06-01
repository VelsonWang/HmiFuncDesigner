#ifndef BLINKCONTROLELEMENTFORM_H
#define BLINKCONTROLELEMENTFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class BlinkControlElementForm;
}

class BlinkControlElementForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit BlinkControlElementForm(QWidget *parent = nullptr);
    ~BlinkControlElementForm();

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
    Ui::BlinkControlElementForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif
