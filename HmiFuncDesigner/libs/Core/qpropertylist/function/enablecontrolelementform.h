#ifndef ENABLECONTROLELEMENTFORM_H
#define ENABLECONTROLELEMENTFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class EnableControlElementForm;
}

class EnableControlElementForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit EnableControlElementForm(QWidget *parent = nullptr);
    ~EnableControlElementForm();

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
    Ui::EnableControlElementForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif
