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

    QStringList funcs() const;
    void setFuncs(const QStringList &funcs);

    bool showContentText() const;
    void setShowContentText(bool bShowContentText);

    QString hAlign() const;
    void setHAlign(const QString &szHAlign);

    QString vAlign() const;
    void setVAlign(const QString &szVAlign);

    QStringList supportEvents() const;
    void setSupportEvents(const QStringList &supportEvents);

    QString tagSelected() const;
    void setTagSelected(const QString &szTagSelected);

    QString resetPictureFile() const;
    void setResetPictureFile(const QString &resetPictureFile);

    QString setPictureFile() const;
    void setSetPictureFile(const QString &setPictureFile);

    QString resetText() const;
    void setResetText(const QString &resetText);

    QString setText() const;
    void setSetText(const QString &setText);

    bool stateOnInitial() const;
    void setStateOnInitial(bool stateOnInitial);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击执行功能按钮
    void on_btnFuncOpt_clicked();
    // 选择复位图片
    void on_btnSelectPicReset_clicked();
    // 选择置位图片
    void on_btnSelectPicSet_clicked();
    // 单击选择变量按钮
    void on_btnSelectTag_clicked();

    void on_rBtnText_clicked(bool checked);

    void on_rBtnPic_clicked(bool checked);

private:
    Ui::EditBasicPropertyDialog *ui;
    // 关联的变量
    QString szTagSelected_;
    // 执行功能
    QStringList funcs_;
    // 初始状态
    bool stateOnInitial_;
    // 显示内容(文本或图片)
    bool bShowContentText_;
    // 支持的处理事件
    QStringList supportEvents_;
    // 复位图片
    QString resetPictureFile_;
    // 置位图片
    QString setPictureFile_;
    // 复位显示文本
    QString resetText_;
    // 置位显示文本
    QString setText_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
};

#endif // EDITBASICPROPERTYDIALOG_H
