#ifndef FUNCTIONEDITORDIALOG_H
#define FUNCTIONEDITORDIALOG_H

#include <QDialog>
#include <QList>
#include <QVector>
#include <QMap>
#include <QDebug>
#include "../shared/projdata/deviceinfo.h"
#include "../shared/property/qabstractproperty.h"
#include "./qpropertylist/qpropertylistview.h"
#include "qabstractfunction.h"

class QListWidget;
class QListWidgetItem;

namespace Ui
{
class FunctionEditorDialog;
}

class FunctionEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FunctionEditorDialog(QWidget *parent = NULL);
    ~FunctionEditorDialog();
    QStringList getFunctions();
    void setFunctions(const QStringList &funcs);

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
    void on_tableEventFunc_clicked(const QModelIndex &index);

private:
    Ui::FunctionEditorDialog *ui;
    QString m_selectedFuncName;
    int m_selectedCurRow;
    QList<QListWidget *> m_listWidgetList;
    QStringList m_funcs;
    QMap<QString, QAbstractFunction *> m_nameToFuncsUi;
};

#endif // FUNCTIONEDITORDIALOG_H
