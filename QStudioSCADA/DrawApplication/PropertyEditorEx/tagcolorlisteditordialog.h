#ifndef TAGCOLORLISTEDITORDIALOG_H
#define TAGCOLORLISTEDITORDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

class ColorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ColorEditor(QWidget *parent = 0);
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
class TagColorListEditorDialog;
}

class TagColorListEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagColorListEditorDialog(QWidget *parent = 0);
    ~TagColorListEditorDialog();
    QStringList getValueColorList();
    void setValueColorList(const QStringList &list);

private slots:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    void initUi();
    void initTableWidget();

private:
    Ui::TagColorListEditorDialog *ui;
    QStringList valueColorList_;
};

#endif // TAGCOLORLISTEDITORDIALOG_H
