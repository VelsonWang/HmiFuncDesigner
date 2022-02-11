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

    int borderWidth() const;
    void setBorderWidth(int borderWidth);

    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);

    QStringList tagTextList() const;
    void setTagTextList(const QStringList &tagTextList);

    QString elementText() const;
    void setElementText(const QString &elementText);

    QColor textColor() const;
    void setTextColor(const QColor &textColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    bool transparentBackground() const;
    void setTransparentBackground(bool transparentBackground);

    QString hAlign() const;
    void setHAlign(const QString &szHAlign);

    QString vAlign() const;
    void setVAlign(const QString &szVAlign);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();
    // 单击边框颜色按钮
    void on_btnBorderColor_clicked();
    // 单击变量文本颜色列表按钮
    void on_btnTagTextColorList_clicked();
    // 单击背景颜色按钮
    void on_btnBackgroundColor_clicked();
    // 单击文本颜色按钮
    void on_btnTextColor_clicked();

private:
    Ui::EditBasicPropertyDialog *ui;
    // 关联的变量
    QString szTagSelected_;
    // 填充颜色列表
    QStringList tagTextList_;
    // 文本
    QString elementText_;
    // 文本颜色
    QColor textColor_;
    // 背景颜色
    QColor backgroundColor_;
    // 透明背景颜色
    bool transparentBackground_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 边框宽度
    int borderWidth_;
    // 边框颜色
    QColor borderColor_;
};

#endif // EDITBASICPROPERTYDIALOG_H
