#ifndef MOVCONTROLELEMENTFORM_H
#define MOVCONTROLELEMENTFORM_H

#include <QWidget>
#include "qabstractfunction.h"

namespace Ui
{
class MoveControlElementForm;
}

class MoveControlElementForm : public QWidget, public QAbstractFunction
{
    Q_OBJECT
public:
    explicit MoveControlElementForm(QWidget *parent = nullptr);
    ~MoveControlElementForm();

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
    Ui::MoveControlElementForm *ui;
    QString m_arg;
    QString m_showArg;
    int m_x;
    int m_y;
};

#endif
