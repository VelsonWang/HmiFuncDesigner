#ifndef TAGTEXTLISTEDITORDIALOG_H
#define TAGTEXTLISTEDITORDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

namespace Ui
{
class TagTextListEditorDialog;
}

class TagTextListEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagTextListEditorDialog(QWidget *parent = NULL);
    ~TagTextListEditorDialog();
    QStringList getValueTextList();
    void setValueTextList(const QStringList &list);

private slots:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    void initUi();
    void initTableWidget();

private:
    Ui::TagTextListEditorDialog *ui;
    QStringList valueTextList;
};

#endif // TAGTEXTLISTEDITORDIALOG_H
