#ifndef INSERTTAGDLG_H
#define INSERTTAGDLG_H

#include <QDialog>
#include <QListWidget>
#include <QStringList>
#include <QString>

namespace Ui
{
class InsertTagDlg;
}

class InsertTagDlg : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTagDlg(QWidget *parent = NULL);
    ~InsertTagDlg();
    QString getSelectedTagName(void) const;

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_listWidgetTagIO_itemClicked(QListWidgetItem *item);
    void on_listWidgetTagTmp_itemClicked(QListWidgetItem *item);
    void on_listWidgetTagSys_itemClicked(QListWidgetItem *item);

private:
    Ui::InsertTagDlg *ui;
    QStringList m_varsList;
    QString m_selectedTagName;
};

#endif // INSERTTAGDLG_H
