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

    QStringList tagTextList() const;
    void setTagTextList(const QStringList &tagTextList);

    QString moveDir() const;
    void setMoveDir(const QString &szMoveDir);

    int moveCharNum() const;
    void setMoveCharNum(int iMoveCharNum);

    double period() const;
    void setPeriod(double period);

    QString elementText() const;
    void setElementText(const QString &szElementText);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();
    // 单击变量文本列表按钮
    void on_btnTagTextList_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 选择的变量
    QString szSelectedTag_;
    // 变量文本列表
    QStringList tagTextList_;
    // 移动文本
    QString szElementText_;
    // 移动方向
    QString szMoveDir_;
    // 移动字符数
    int iMoveCharNum_;
    // 移动间隔
    double period_;
};

#endif // EDITBASICPROPERTYDIALOG_H
