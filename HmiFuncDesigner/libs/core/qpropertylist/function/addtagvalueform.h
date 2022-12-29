#ifndef ADDTAGVALUEFORM_H
#define ADDTAGVALUEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class AddTagValueForm;
}

class AddTagValueForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit AddTagValueForm(QWidget *parent = nullptr);
    ~AddTagValueForm();

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
    Ui::AddTagValueForm *ui;
    QStringList m_args;
    QStringList m_showArgs;
};

#endif // SETTAGVALUEFORM_H
