#ifndef INSERTFUNCTIONDIALOG_H
#define INSERTFUNCTIONDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QMap>

namespace Ui {
class InsertFunctionDialog;
}

class InsertFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertFunctionDialog(QWidget *parent = 0);
    ~InsertFunctionDialog();
    QString getSelectedFuncName(void) const;

private slots:
    void on_treeWidgetFunc_itemClicked(QTreeWidgetItem *item, int column);
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    void treeWidgetInit();

private:
    Ui::InsertFunctionDialog *ui;
    QString m_strSelectedFuncName;
    QMap<QString, QString> m_funcNameDescMap;
    QMap<QString, QString> m_funcNameNameMap;
};

#endif // INSERTFUNCTIONDIALOG_H
