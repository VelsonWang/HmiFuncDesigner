#ifndef ADDRULEDIALOG_H
#define ADDRULEDIALOG_H

#include <QDialog>
#include "ui_addruledialog.h"

class AddRuleDialog : public QDialog, public Ui::AddRuleDialog
{
public:
    AddRuleDialog(QWidget *parent = 0);

    QString getState() const;
    QString getBackground() const;
    QString getText() const;
    QString getBorderColor() const;
    QString getSignColor() const;

private:
    void creaView();
};

#endif // ADDRULEDIALOG_H
