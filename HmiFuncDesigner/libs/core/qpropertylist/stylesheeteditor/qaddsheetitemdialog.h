#ifndef QADDSHEETITEMDIALOG_H
#define QADDSHEETITEMDIALOG_H

#include <QDialog>

class QStateCheckWidget;
class QAbstractProperty;
namespace Ui
{
class QAddSheetItemDialog;
}

class QAddSheetItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QAddSheetItemDialog(QAbstractProperty* property,
                                 const QString &title = "",
                                 QWidget *parent = 0);
    ~QAddSheetItemDialog();
    QString getTagName();

protected slots:
    void makeTitle();
    void checkChanged();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::QAddSheetItemDialog *ui;
    QStateCheckWidget *m_checkWidget;
    QString m_title;
};

#endif // QADDSHEETITEMDIALOG_H
