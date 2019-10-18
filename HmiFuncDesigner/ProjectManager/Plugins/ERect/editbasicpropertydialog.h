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
    explicit EditBasicPropertyDialog(QWidget *parent = nullptr);
    ~EditBasicPropertyDialog();

    QString selectedTag() const;
    void setSelectedTag(const QString &szSelectedTag);

    QStringList tagColorList() const;
    void setTagColorList(const QStringList &tagColorList);

    QColor fillColor() const;
    void setFillColor(const QColor &fillColor);

    bool isFill() const;
    void setIsFill(bool isFill);

    int borderWidth() const;
    void setBorderWidth(int borderWidth);

    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();
    // 单击填充颜色列表按钮
    void on_btnTagColorList_clicked();
    // 单击填充颜色按钮
    void on_btnFillColor_clicked();
    // 单击边框颜色按钮
    void on_btnBorderColor_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagColorList_;
    // 填充颜色
    QColor fillColor_;
    // 是否填充颜色
    bool isFill_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
};

#endif // EDITBASICPROPERTYDIALOG_H
