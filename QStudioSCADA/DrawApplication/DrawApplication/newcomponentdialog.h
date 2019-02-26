#ifndef NEWCOMPONENTDIALOG_H
#define NEWCOMPONENTDIALOG_H

#include <QDialog>
#include "DrawAppDefs.h"
#include "ui_newcomponentdialog.h"

class NewComponentDialog : public QDialog, public Ui::NewComponentDialog
{
    Q_OBJECT
    
public:
    explicit NewComponentDialog(QWidget *parent = 0);
    ComponentType getLastChoose() const;

public slots:
    void slotChoose();
    void slotCancel();
    void slotChooseDoubleClicked(QListWidgetItem *);

private:
    ComponentType lastChooseItem;
};

#endif // NEWCOMPONENTDIALOG_H
