#ifndef SAVESTRATEGYDIALOG_H
#define SAVESTRATEGYDIALOG_H

#include <QDialog>
#include "DrawAppDefs.h"
#include "ui_SaveStrategyDialog.h"

class SaveStrategyDialog : public QDialog, public Ui::SaveStrategyDialog
{
    Q_OBJECT
    
public:
    explicit SaveStrategyDialog(QWidget *parent = 0);
    SaveGraphPageType getLastChoose() const;

public slots:
    void slotCancel();
    void slotChoose();
    void slotChooseDoubleClicked(QListWidgetItem *);
    void slotItemClicked(QListWidgetItem *);

private:
    SaveGraphPageType lastChooseItem;
};

#endif // SAVESTRATEGYDIALOG_H
