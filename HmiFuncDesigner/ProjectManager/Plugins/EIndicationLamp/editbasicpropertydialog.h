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

    QString selectedTag() const;
    void setSelectedTag(const QString &szSelectedTag);

    bool stateOnInitial() const;
    void setStateOnInitial(bool stateOnInitial);

    QString resetFileIndicationLamp() const;
    void setResetFileIndicationLamp(const QString &resetFileIndicationLamp);

    QString setFileIndicationLamp() const;
    void setSetFileIndicationLamp(const QString &setFileIndicationLamp);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();
    // 选择复位图片
    void on_btnSelectPicReset_clicked();
    // 选择置位图片
    void on_btnSelectPicSet_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 选择的变量
    QString szSelectedTag_;
    // 初始状态
    bool stateOnInitial_;
    // 复位图片
    QString resetFileIndicationLamp_;
    // 置位图片
    QString setFileIndicationLamp_;
};

#endif // EDITBASICPROPERTYDIALOG_H
