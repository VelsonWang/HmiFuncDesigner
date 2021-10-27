#ifndef INSERTFUNCTIONDLG_H
#define INSERTFUNCTIONDLG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QMap>

namespace Ui
{
class InsertFunctionDlg;
}

class InsertFunctionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit InsertFunctionDlg(QWidget *parent = NULL);
    ~InsertFunctionDlg();
    QString getSelectedFuncName(void) const;

private slots:
    void on_treeWidgetFunc_itemClicked(QTreeWidgetItem *item, int column);
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    void treeWidgetInit();

private:
    Ui::InsertFunctionDlg *ui;
    QString m_strSelectedFuncName;
    QMap<QString, QString> m_funcNameDescMap;
    QMap<QString, QString> m_funcNameNameMap;
};

#endif // INSERTFUNCTIONDLG_H
