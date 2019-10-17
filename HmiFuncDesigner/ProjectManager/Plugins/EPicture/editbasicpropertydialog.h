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

    QString filePicture() const;
    void setFilePicture(const QString &filePicture);

    bool showNoScale() const;
    void setShowNoScale(bool showNoScale);

    int borderWidth() const;
    void setBorderWidth(int borderWidth);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择图片按钮
    void on_btnSelectPic_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 图片名
    QString filePicture_;
    // 原尺寸显示
    bool showNoScale_;
    // 边框宽度
    int borderWidth_;
};

#endif // EDITBASICPROPERTYDIALOG_H
