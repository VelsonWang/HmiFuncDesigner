#ifndef TAGTEXTLISTEDITORDIALOG_H
#define TAGTEXTLISTEDITORDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

class ColorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ColorEditor(QWidget *parent = nullptr);
    ~ColorEditor();

    void setColor(const QColor &color);
    QColor getColor() const;

    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    void onToolButtonClicked();

private:
    QColor color_;
    QToolButton* toolButton_;
    QLabel* colorLabel_;
};

////////////////////////////////////////////////////////

namespace Ui {
class TagTextListEditorDialog;
}

class TagTextListEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagTextListEditorDialog(QWidget *parent = nullptr);
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
    QStringList valueTextList_;
};

#endif // TAGTEXTLISTEDITORDIALOG_H
