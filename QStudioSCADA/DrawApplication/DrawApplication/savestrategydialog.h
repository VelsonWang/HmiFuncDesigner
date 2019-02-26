#ifndef SAVESTRATEGYDIALOG_H
#define SAVESTRATEGYDIALOG_H

#include <QDialog>
#include "DrawAppDefs.h"
#include "ui_savestrategydialog.h"

class SaveStrategyDialog : public QDialog, public Ui::SaveStrategyDialog
{
    Q_OBJECT
    
public:
    explicit SaveStrategyDialog(QWidget *parent = 0);
    SaveScreenType getLastChoose() const;

public slots:
    void slotCancel();
    void slotChoose();
    void slotChooseDoubleClicked(QListWidgetItem *);
    void slotItemClicked(QListWidgetItem *);

private:
    SaveScreenType lastChooseItem;
};

#endif // SAVESTRATEGYDIALOG_H
