#ifndef SETTAGVALUEFORM_H
#define SETTAGVALUEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class SetTagValueForm;
}

class SetTagValueForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit SetTagValueForm(QWidget *parent = nullptr);
    ~SetTagValueForm();

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
    Ui::SetTagValueForm *ui;
    QStringList m_args;
    QStringList m_showArgs;
};

#endif // SETTAGVALUEFORM_H
