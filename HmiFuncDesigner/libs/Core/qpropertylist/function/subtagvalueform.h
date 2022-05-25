#ifndef SUBTAGVALUEFORM_H
#define SUBTAGVALUEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class SubTagValueForm;
}

class SubTagValueForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit SubTagValueForm(QWidget *parent = nullptr);
    ~SubTagValueForm();

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
    void on_cboSelectTag_currentTextChanged(const QString &id);

private:
    void updateFromUi();

private:
    Ui::SubTagValueForm *ui;
    QStringList m_args;
    QStringList m_showArgs;
};

#endif // SETTAGVALUEFORM_H
