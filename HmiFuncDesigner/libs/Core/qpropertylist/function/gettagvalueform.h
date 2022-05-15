#ifndef GETTAGVALUEFORM_H
#define GETTAGVALUEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class GetTagValueForm;
}

class GetTagValueForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit GetTagValueForm(QWidget *parent = nullptr);
    ~GetTagValueForm();

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
    Ui::GetTagValueForm *ui;
    QStringList m_args;
    QStringList m_showArgs;
};

#endif
