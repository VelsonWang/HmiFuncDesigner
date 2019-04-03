#ifndef FUNCTIONEDITORDIALOG_H
#define FUNCTIONEDITORDIALOG_H

#include "propertytableview.h"
#include "propertymodel.h"
#include <QDialog>
#include <QMap>

class QListWidgetItem;

namespace Ui {
class FunctionEditorDialog;
}

class FunctionEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionEditorDialog(QWidget *parent = 0);
    ~FunctionEditorDialog();
    QStringList getFunctions() const;

private:
    void initUI();
    void initListWidget();
    void initTableWidget();

private slots:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnMoveUp_clicked();
    void on_btnMoveDown_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();    
    void listItemClicked(QListWidgetItem *item);
    void listItemDoubleClicked(QListWidgetItem *item);


private:
    Ui::FunctionEditorDialog *ui;
    PropertyTableView *propertyView_;
    PropertyModel *propertyModel_;
    QString szSelectedFuncName_;
    QMap<QString, QString> funcNameDescMap_;
    QMap<QString, QString> funcNameNameMap_;
    QStringList funcs_;
};

#endif // FUNCTIONEDITORDIALOG_H
