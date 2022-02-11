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

    QString script() const;
    void setScript(const QString &script);

    bool showContentText() const;
    void setShowContentText(bool bShowContentText);

    QString filePicture() const;
    void setFilePicture(const QString &filePicture);

    QString elementText() const;
    void setElementText(const QString &elementText);

    QString hAlign() const;
    void setHAlign(const QString &szHAlign);

    QString vAlign() const;
    void setVAlign(const QString &szVAlign);

    QStringList supportEvents() const;
    void setSupportEvents(const QStringList &supportEvents);

private slots:
    // 单击确定按钮
    void on_btnOk_clicked();
    // 单击取消按钮
    void on_btnCancel_clicked();
    // 单击执行功能按钮
    void on_btnFuncOpt_clicked();
    // 单击执行脚本按钮
    void on_btnExecScript_clicked();
    // 单击选择图片按钮
    void on_btnSelectedPic_clicked();

    void on_rBtnText_clicked(bool checked);

    void on_rBtnPic_clicked(bool checked);

private:
    Ui::EditBasicPropertyDialog *ui;
    // 执行功能
    QStringList funcs_;
    // 执行JavaScript脚本
    QString script_;
    // 显示内容(文本或图片)
    bool bShowContentText_;
    // 图片名
    QString filePicture_;
    // 文本
    QString elementText_;
    // 水平对齐
    QString szHAlign_;
    // 垂直对齐
    QString szVAlign_;
    // 支持的处理事件
    QStringList supportEvents_;
};

#endif // EDITBASICPROPERTYDIALOG_H
