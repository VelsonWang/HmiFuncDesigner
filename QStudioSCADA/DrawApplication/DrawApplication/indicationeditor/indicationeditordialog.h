#ifndef INDICATIONEDITORDIALOG_H
#define INDICATIONEDITORDIALOG_H

#include <QDialog>
#include "ui_indicationeditordialog.h"
#include "ruletablemodel.h"
#include "ruletableview.h"
#include "graphicselements/figure.h"

class IndicationEditorDialog : public QDialog, public Ui::IndicationEditorDialog
{
    Q_OBJECT
    
public:
    explicit IndicationEditorDialog(QWidget *parent = 0);
    void setCurrentEditElement(Figure *editFigure);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createView();
    void createConnections();
    void updateUi();
    void createRuleString();
    void createRulePreview(const QString &mRuleStr);

    RuleTableModel *ruleModel;
    RuleTableView  *ruleView;
    QString ruleStr;
    Figure *editElement;
    bool unsaved;
    QList <Features*> ruleList;

private slots:
    void slotAddRule();
    void slotDeleteRule();
    void slotApply();
    void slotOk();
    void linkingTypeChanged(const QString &);
    void deviceLinkChanged(const QString &);
    void slotRuleModelChanged();
};

#endif // INDICATIONEDITORDIALOG_H
