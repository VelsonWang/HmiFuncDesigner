#ifndef RETURNGRAPHPAGEFORM_H
#define RETURNGRAPHPAGEFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class ReturnGraphPage;
}

class ReturnGraphPage : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit ReturnGraphPage(QWidget *parent = nullptr);
    ~ReturnGraphPage();

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
    Ui::ReturnGraphPage *ui;

};

#endif // RETURNGRAPHPAGEFORM_H
