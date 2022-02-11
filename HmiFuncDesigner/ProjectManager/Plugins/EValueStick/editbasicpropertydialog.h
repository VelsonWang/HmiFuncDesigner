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

    double maxValue() const;
    void setMaxValue(double maxValue);

    double minValue() const;
    void setMinValue(double minValue);

    int scaleNum() const;
    void setScaleNum(int scaleNum);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    QColor foregroundColor() const;
    void setForegroundColor(const QColor &foregroundColor);

    QColor scaleColor() const;
    void setScaleColor(const QColor &scaleColor);

    QString scaleDir() const;
    void setScaleDir(const QString &scaleDir);

    QString scalePos() const;
    void setScalePos(const QString &scalePos);

    QColor textColor() const;
    void setTextColor(const QColor &textColor);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();
    // 单击背景颜色按钮
    void on_btnBackgroundColor_clicked();
    // 单击前景颜色按钮
    void on_btnForeground_clicked();
    // 单击标尺颜色按钮
    void on_btnScaleColor_clicked();
    // 单击文本颜色按钮
    void on_btnTextColor_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 关联的变量
    QString szTagSelected_;
    // 刻度最大值。
    double maxValue_;
    // 刻度最小值。
    double minValue_;
    // 刻度个数
    int scaleNum_;
    // 背景颜色
    QColor backgroundColor_;
    // 前景颜色
    QColor foregroundColor_;
    // 标尺颜色
    QColor scaleColor_;
    // 标尺方向
    QString scaleDir_;
    // 标尺位置
    QString scalePos_;
    // 文本颜色
    QColor textColor_;
};

#endif // EDITBASICPROPERTYDIALOG_H
