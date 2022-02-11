#ifndef INSERTTAGDIALOG_H
#define INSERTTAGDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QStringList>
#include <QString>



namespace Ui {
class InsertTagDialog;
}

class InsertTagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTagDialog(QWidget *parent = 0);
    ~InsertTagDialog();
    QString getSelectedTagName(void) const;

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_listWidgetTagIO_itemClicked(QListWidgetItem *item);
    void on_listWidgetTagTmp_itemClicked(QListWidgetItem *item);
    void on_listWidgetTagSys_itemClicked(QListWidgetItem *item);

private:
    Ui::InsertTagDialog *ui;
    QStringList m_listTags;
    QString m_szSelectedTagName;
};

#endif // INSERTTAGDIALOG_H
