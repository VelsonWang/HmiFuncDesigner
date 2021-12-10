#ifndef EDITBASICPROPERTYDIALOG_H
#define EDITBASICPROPERTYDIALOG_H

#include <QDialog>

namespace Ui {
class EditBasicPropertyDialog;
}

class EditBasicPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditBasicPropertyDialog(QWidget *parent = Q_NULLPTR);
    ~EditBasicPropertyDialog();

    QString text() const;
    void setText(const QString &szText);

    QString selectedTag() const;
    void setSelectedTag(const QString &szSelectedTag);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 文本属性
    QString szText_;
    // 选择的变量
    QString szSelectedTag_;
};

#endif // EDITBASICPROPERTYDIALOG_H
