#ifndef SWITCHGRAPHPAGEFORM_H
#define SWITCHGRAPHPAGEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class SwitchGraphPageForm;
}

class SwitchGraphPageForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit SwitchGraphPageForm(QWidget *parent = nullptr);
    ~SwitchGraphPageForm();

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
    Ui::SwitchGraphPageForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif // SWITCHGRAPHPAGEFORM_H
