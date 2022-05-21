#ifndef SHOWCONTROLELEMENTFORM_H
#define SHOWCONTROLELEMENTFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class ShowControlElementForm;
}

class ShowControlElementForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit ShowControlElementForm(QWidget *parent = nullptr);
    ~ShowControlElementForm();

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
    Ui::ShowControlElementForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif
