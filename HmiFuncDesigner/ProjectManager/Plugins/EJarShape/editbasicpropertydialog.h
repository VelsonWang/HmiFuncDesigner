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

    QString jarShape() const;
    void setJarShape(const QString &jarShape);

    double upperLimitValue() const;
    void setUpperLimitValue(double upperLimitValue);

    double lowerLimitValue() const;
    void setLowerLimitValue(double lowerLimitValue);

    double maxValue() const;
    void setMaxValue(double maxValue);

    int scaleNum() const;
    void setScaleNum(int scaleNum);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 选择的变量
    QString szSelectedTag_;
    // 罐形容器标题
    QString jarShape_;
    // 上限值
    double upperLimitValue_;
    // 下限值
    double lowerLimitValue_;
    // 满量程值-刻度最大值。
    double maxValue_;
    // 刻度个数
    int scaleNum_;
};

#endif // EDITBASICPROPERTYDIALOG_H
