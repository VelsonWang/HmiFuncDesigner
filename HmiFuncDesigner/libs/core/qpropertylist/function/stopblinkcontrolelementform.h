#ifndef STOPBLINKCONTROLELEMENTFORM_H
#define STOPBLINKCONTROLELEMENTFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class StopBlinkControlElementForm;
}

class StopBlinkControlElementForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit StopBlinkControlElementForm(QWidget *parent = nullptr);
    ~StopBlinkControlElementForm();

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
    Ui::StopBlinkControlElementForm *ui;
    QString m_arg;
    QString m_showArg;
};

#endif
