#ifndef HIDECONTROLELEMENTFORM_H
#define HIDECONTROLELEMENTFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class HideControlElementForm;
}

class HideControlElementForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit HideControlElementForm(QWidget *parent = nullptr);
    ~HideControlElementForm();

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
    Ui::HideControlElementForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif
